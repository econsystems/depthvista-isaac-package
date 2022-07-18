#include "DepthvistaCamera.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "engine/core/assert.hpp"
#include "engine/core/math/utils.hpp"
#include "engine/core/image/image.hpp"
#include "engine/gems/geometry/pinhole.hpp"
#include "engine/gems/image/conversions.hpp"
#include "engine/gems/image/utils.hpp"
#include "messages/camera.hpp"

#include "engine/gems/image/color.hpp"

namespace isaac {

namespace {

// Helper function to copy camera intrinsics to CameraIntrinsicsProto
void SetCameraProtoIntrinsics(const depthvista::CameraIntrinsics& intrinsics,
                              ::CameraIntrinsicsProto::Builder out) {
  // Pinhole camera model parameters
  auto pinhole = out.initPinhole();
  ToProto(Vector2d(intrinsics.fy, intrinsics.fx), pinhole.getFocal());
  ToProto(Vector2d(intrinsics.cy, intrinsics.cx), pinhole.getCenter());
  pinhole.setCols(intrinsics.w);
  pinhole.setRows(intrinsics.h);

  // Distortion parameters
  auto distortion = out.initDistortion();
  distortion.setModel(DistortionProto::DistortionModel::BROWN);
  Vector5d disto;
  disto << intrinsics.disto[0], intrinsics.disto[1], intrinsics.disto[2], intrinsics.disto[3], intrinsics.disto[4];
  ToProto(disto, distortion.getCoefficients());
}

// Extracts camera intrinsics and converts them into a geometry::PinholeD
Pose3d GetCameraExtrinsics(depthvista::CameraExtrinsics extrinsics) {
  Matrix3f rotation;
  rotation << extrinsics.rotation[0], extrinsics.rotation[3], extrinsics.rotation[6],
              extrinsics.rotation[1], extrinsics.rotation[4], extrinsics.rotation[7],
              extrinsics.rotation[2], extrinsics.rotation[5], extrinsics.rotation[8];
  Quaternionf quaternion(rotation);
  Pose3f pose;
  pose.rotation = SO3f::FromQuaternion(quaternion);
  pose.translation =
      Vector3f(extrinsics.translation[0], extrinsics.translation[1], extrinsics.translation[2]);
  return pose.cast<double>();
}

uint8_t Div256(int32_t val) {
  return static_cast<uint8_t>(val >> 8);
}

void YCbCrToRgb(uint8_t y, uint8_t cb, uint8_t cr, uint8_t* out) {
  const int32_t y_shifted = y - 16;
  const int32_t cr_shifted = cr - 128;
  const int32_t cb_shifted = cb - 128;

  out[0] = Div256(Clamp(298 * y_shifted + 409 * cr_shifted, 0, 0xffff));
  out[1] = Div256(Clamp(298 * y_shifted - 208 * cr_shifted - 100 * cb_shifted, 0, 0xffff));
  out[2] = Div256(Clamp(298 * y_shifted + 516 * cb_shifted, 0, 0xffff));
}

}

void DepthVistaCamera::ConvertUyvyToRgb(const Image2ub& uyvy, Image3ub& rgb) {
  ASSERT(uyvy.num_pixels() % 2 == 0, "invalid input image size");
  rgb.resize(uyvy.rows(), uyvy.cols());

  // Each 4 byte block encodes two pixels, so we read 4 bytes at a time.
  const uint32_t* raw_in = reinterpret_cast<const uint32_t*>(uyvy.element_wise_begin());
  const uint32_t* raw_end = reinterpret_cast<const uint32_t*>(uyvy.element_wise_end());
  uint8_t* raw_out = rgb.element_wise_begin();

  // Convert each 2 pixel block.
  for (; raw_in != raw_end; ++raw_in) {
    const uint32_t uyvy_block = *raw_in;
    const uint8_t y0 = (uyvy_block >> 8) & 0xff;
    const uint8_t cb = (uyvy_block >> 0) & 0xff;
    const uint8_t y1 = (uyvy_block >> 24) & 0xff;
    const uint8_t cr = (uyvy_block >> 16) & 0xff;

    // Encode first pixel.
    YCbCrToRgb(y0, cb, cr, raw_out);
    raw_out += 3;

    // Encode second pixel.
    YCbCrToRgb(y1, cb, cr, raw_out);
    raw_out += 3;
  }
}

void DepthVistaCamera::start() {
  	LOG_INFO("start");

	// ----> Create Video Capture
	depthvista::CameraStaticParams params;
	params.res.w = get_cols();
	params.res.h = get_rows();
	params.dev_id = get_dev_index();
	params.mode = get_enable_depth() ? depthvista::MODE::DEPTH : 0;
	params.mode |= get_enable_rgb() ? depthvista::MODE::RGB : 0;
	params.mode |= get_enable_ir() ? depthvista::MODE::IR : 0;
	//params.range = get_enable_long_range();
	params.verbose = depthvista::VERBOSITY::INFO;

	camera = std::make_unique<depthvista::Camera>();

	if (!camera->init(params)) {
		reportFailure("depthvista_camera: invalid parameters or failed to init camera");
	} else {
		mDynamicParams.range = get_enable_long_range();
		camera->update_config(mDynamicParams);
		camera->start();
	}

	mCalibParams = camera->get_calibration_details();

	set_depth_camera_T_rgb_camera(GetCameraExtrinsics(mCalibParams.extrinsics), 0.0);

	//tickPeriodically();
	tickBlocking();
}

void DepthVistaCamera::tick() {
	//LOG_INFO("tick");

	if (mDynamicParams.range != get_enable_long_range()) {
		mDynamicParams.range = get_enable_long_range();
		camera->update_config(mDynamicParams);
	}

	if (camera->wait_for_frames()) {
		const int64_t acq_time = node()->clock()->timestamp();

		if (get_enable_rgb()) { //rgb frame
			depthvista::Frame frame = camera->get_rgb_frame();
			Image2ub buffer_uyvy(frame.height, frame.width);
			Image3ub buffer_view(frame.height, frame.width);

			Copy(CreateImageConstView<uint8_t, 2>(frame.data, frame.height, frame.width), buffer_uyvy);
			ConvertUyvyToRgb(buffer_uyvy, buffer_view);

			ToProto(std::move(buffer_view), tx_rgb().initProto(), tx_rgb().buffers());
			SetCameraProtoIntrinsics(mCalibParams.rgb_intrinsics, tx_rgb_intrinsics().initProto());

			tx_rgb().publish(acq_time);
			tx_rgb_intrinsics().publish(acq_time);
		}

		if (get_enable_depth()) { //depth frame
			depthvista::Frame frame = camera->get_depth_frame();
			ImageConstView1ui16 buffer_depth = CreateImageConstView<uint16_t, 1>(reinterpret_cast<const uint16_t*>(&frame.data[0]), frame.height, frame.width);
			Image1f buffer_view(buffer_depth.dimensions());

			ConvertUi16ToF32(buffer_depth, buffer_view, 0.001);

			if (get_enable_long_range())
			ToProto(std::move(buffer_view), tx_depth_long().initProto(), tx_depth_long().buffers());
			else
			ToProto(std::move(buffer_view), tx_depth_short().initProto(), tx_depth_short().buffers());
			SetCameraProtoIntrinsics(mCalibParams.depth_intrinsics, tx_depth_intrinsics().initProto());

			if (get_enable_long_range())
			tx_depth_long().publish(acq_time);
			else
			tx_depth_short().publish(acq_time);
			tx_depth_intrinsics().publish(acq_time);
		}

		if (get_enable_ir()) { //ir frame
			depthvista::Frame frame = camera->get_ir_frame();
			Image1ub buffer_view(frame.height, frame.width);

			Copy(CreateImageConstView<uint8_t, 1>(frame.data, frame.height, frame.width), buffer_view);

			ToProto(std::move(buffer_view), tx_ir().initProto(), tx_ir().buffers());

			tx_ir().publish(acq_time);
		}
	}
}

void DepthVistaCamera::stop() {
  	LOG_INFO("stop");
	camera->stop();
}

}

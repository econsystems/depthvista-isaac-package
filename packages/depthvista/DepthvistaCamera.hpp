#pragma once

#include <memory>
#include <string>

#include "engine/alice/alice_codelet.hpp"
#include "engine/core/math/types.hpp"
#include "messages/camera.capnp.h"
#include "messages/math.capnp.h"

#include "camera.hpp"

namespace isaac {

class DepthVistaCamera : public isaac::alice::Codelet {
 public:
  void start() override;
  void tick() override;
  void stop() override;

  void ConvertUyvyToRgb(const Image2ub& yuyv, Image3ub& rgb);	// As not supporting UYVY to RGB convertion

  depthvista::Camera* getDepthVistaCamera() { return camera.get(); }

  // The rgb camera image, which can be of type Image3ub
  ISAAC_PROTO_TX(ImageProto, rgb);
  // The depth image (in meters)
  ISAAC_PROTO_TX(ImageProto, depth_short);
  ISAAC_PROTO_TX(ImageProto, depth_long);
  // The IR camera image
  ISAAC_PROTO_TX(ImageProto, ir);
  ISAAC_PROTO_TX(CameraIntrinsicsProto, rgb_intrinsics);
  ISAAC_PROTO_TX(CameraIntrinsicsProto, depth_intrinsics);
  ISAAC_PROTO_TX(Pose3dProto, extrinsics);
  ISAAC_POSE3(depth_camera, rgb_camera);

  // The vertical resolution for both rgb and depth images.
  ISAAC_PARAM(int, rows, 480);
  // The horizontal resolution for both rgb and depth images.
  ISAAC_PARAM(int, cols, 640);
  ISAAC_PARAM(int, dev_index, 0);
  ISAAC_PARAM(bool, enable_depth, true);
  ISAAC_PARAM(bool, enable_rgb, true);
  ISAAC_PARAM(bool, enable_ir, true);
  ISAAC_PARAM(bool, enable_long_range, false);

private:
  std::unique_ptr<depthvista::Camera> camera;

  depthvista::CameraDynamicParams mDynamicParams;
  depthvista::CalibrationParams mCalibParams;
};

}

ISAAC_ALICE_REGISTER_CODELET(isaac::DepthVistaCamera);

#pragma once

#include "cgp/interaction/camera/camera.hpp"
#include "cgp/interaction/glfw_inputs/glfw_inputs.hpp"

namespace cgp {

	void camera_standard_behavior_rotation_trackball(camera_around_center& camera, inputs_interaction_parameters& inputs);
	void camera_standard_behavior_rotation_spherical_coordinates(camera_spherical_coordinates& camera, inputs_interaction_parameters& inputs);

}
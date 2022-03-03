#pragma once

#include "cgp/math/math.hpp"
#include "cgp/interaction/camera/camera_base/camera_base.hpp"

namespace cgp
{
	struct camera_head : camera_base
	{
		camera_head();

		rotation_transform orientation_camera;
		vec3 position_camera;

		vec3 position() const;
		rotation_transform orientation() const;

	    void manipulator_rotate_trackball(vec2 const& p0, vec2 const& p1);
		void manipulator_rotate_roll_pitch_yaw(float roll, float pitch, float yaw);
		void manipulator_translate_in_plane(vec2 const& tr);
	};
}
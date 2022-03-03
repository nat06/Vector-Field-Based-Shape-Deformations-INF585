#include "camera_head.hpp"

#include "cgp/base/base.hpp"
#include "../common_functions/common_functions.hpp"

namespace cgp
{

	camera_head::camera_head()
		:orientation_camera(), position_camera()
	{}

	vec3 camera_head::position() const
	{
		return position_camera;
	}
	rotation_transform camera_head::orientation() const
	{
		return orientation_camera;
	}

	void camera_head::manipulator_rotate_trackball(vec2 const& p0, vec2 const& p1)
	{
		rotation_transform const r = trackball_rotation(p0,p1);
		orientation_camera = orientation_camera * r;
	}
	void camera_head::manipulator_rotate_roll_pitch_yaw(float roll, float pitch, float yaw)
	{
		rotation_transform r_roll  = rotation_transform::from_axis_angle({0,0,-1}, roll);
		rotation_transform r_pitch = rotation_transform::from_axis_angle({1,0,0}, pitch);
		rotation_transform r_yaw   = rotation_transform::from_axis_angle({0,1,0}, yaw);

		orientation_camera = orientation_camera * r_yaw * r_pitch * r_roll;
	}
	void camera_head::manipulator_translate_in_plane(vec2 const& tr)
	{
		position_camera -= translation_in_plane(tr, orientation());
	}


}
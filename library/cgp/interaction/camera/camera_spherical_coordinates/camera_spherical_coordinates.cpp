#include "camera_spherical_coordinates.hpp"

#include "cgp/base/base.hpp"
#include "../common_functions/common_functions.hpp"

namespace cgp
{
	camera_spherical_coordinates::camera_spherical_coordinates()
		:theta(0.0f), phi(0.0f), center_of_rotation(), distance_to_center(5.0f), axis(camera_spherical_coordinates_axis::y)
	{}

	vec3 camera_spherical_coordinates::position() const
	{
		return orientation()*vec3{0,0,distance_to_center}+center_of_rotation;
	}
	rotation_transform camera_spherical_coordinates::orientation() const
	{

		rotation_transform r_phi = rotation_transform::from_axis_angle({ 0,1,0 }, phi);
		rotation_transform r_theta = rotation_transform::from_axis_angle({ 1,0,0 }, theta);
		
		// Re-orient the camera depending on the reference axis
		if (axis == camera_spherical_coordinates_axis::z) 
			r_phi = r_phi * rotation_transform::from_axis_angle({ 1,0,0 }, -Pi / 2.0f);
		if (axis == camera_spherical_coordinates_axis::x) 
			r_phi = r_phi *rotation_transform::from_axis_angle({ 0,0,1 }, Pi / 2.0f);

		return inverse(r_theta * r_phi);
	}

	void camera_spherical_coordinates::look_at(vec3 const& eye, vec3 const& center)
	{
		center_of_rotation = center;
		distance_to_center = norm(eye - center);

		vec3 const u = normalize(eye - center);
		if (axis == camera_spherical_coordinates_axis::y) {
			phi = -std::atan2(u.x, u.z);// -std::atan2(u.y, u.x) - Pi / 2.0f;
			theta = std::asin(u.y);
		}
		else if (axis == camera_spherical_coordinates_axis::z) {
			phi = -std::atan2(u.y, u.x) - Pi/2.0f;
			theta = std::asin(u.z);
		}
		else if (axis == camera_spherical_coordinates_axis::x) {
			phi = std::atan2(u.y, u.z);
			theta = std::asin(u.x);
		}

	}

	void camera_spherical_coordinates::manipulator_rotate_spherical_coordinates(float d_phi, float d_theta)
	{
		theta += d_theta;
		phi += d_phi;
	}
	void camera_spherical_coordinates::manipulator_scale_distance_to_center(float magnitude)
	{
		distance_to_center *= (1.0f + magnitude);
		distance_to_center = std::max(distance_to_center, 0.01f);
	}
	void camera_spherical_coordinates::manipulator_translate_in_plane(vec2 const& tr)
	{
		center_of_rotation -= translation_in_plane(tr, orientation());
	}


}
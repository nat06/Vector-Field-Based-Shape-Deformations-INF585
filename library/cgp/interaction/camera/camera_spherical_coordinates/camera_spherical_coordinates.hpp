#pragma once

#include "cgp/math/math.hpp"
#include "cgp/interaction/camera/camera_base/camera_base.hpp"

namespace cgp
{
	// use to define around which axis the spherical coordinates will rotate
	enum struct camera_spherical_coordinates_axis {x, y, z}; 

	struct camera_spherical_coordinates: camera_base{

		camera_spherical_coordinates();

		float theta;
		float phi;
		vec3 center_of_rotation;
		float distance_to_center;

		camera_spherical_coordinates_axis axis; // default rotation is around y axis

		vec3 position() const;
		rotation_transform orientation() const;

		void manipulator_rotate_spherical_coordinates(float phi, float theta);
	    void manipulator_scale_distance_to_center(float magnitude);
		void manipulator_translate_in_plane(vec2 const& tr);

		void look_at(vec3 const& eye, vec3 const& center);

	};
}
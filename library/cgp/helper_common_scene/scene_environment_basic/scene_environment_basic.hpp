#pragma once

#include "cgp/display/display.hpp"
#include "cgp/math/math.hpp"
#include "cgp/interaction/interaction.hpp"

namespace cgp {

	struct scene_environment_basic
	{
		// Color of the background of the scene
		vec3 background_color;

		// The position/orientation of a camera that can rotates freely around a specific position
		camera_around_center camera;

		// A projection structure (perspective or orthogonal projection)
		camera_projection projection;

		// The position of a light
		vec3 light;

		scene_environment_basic();
	};

	struct scene_environment_basic_camera_spherical_coords
	{
		// Color of the background of the scene
		vec3 background_color;

		// The position/orientation of a camera that can rotates using spherical-coordinates system around a position
		camera_spherical_coordinates camera;

		// A projection structure (perspective or orthogonal projection)
		camera_projection projection;

		// The position of a light
		vec3 light;

		scene_environment_basic_camera_spherical_coords();
	};

	void opengl_uniform(GLuint shader, scene_environment_basic const& scene_environment);
	void opengl_uniform(GLuint shader, scene_environment_basic_camera_spherical_coords const& scene_environment);

}





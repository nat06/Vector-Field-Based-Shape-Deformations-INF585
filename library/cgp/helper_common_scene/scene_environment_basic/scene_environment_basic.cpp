#include "scene_environment_basic.hpp"

#include "cgp/base/base.hpp"
#include "cgp/display/display.hpp"


namespace cgp
{
	scene_environment_basic::scene_environment_basic()
	{
		background_color = { 1,1,1 };
		camera.look_at({ 3,1,2 }, { 0,0,0.0 }, { 0,0,1 });
		light = { 1,1,1 };

		projection = camera_projection::perspective(50.0f * Pi / 180, 1.0f, 0.1f, 500.0f);
	}

	scene_environment_basic_camera_spherical_coords::scene_environment_basic_camera_spherical_coords()
	{
		background_color = { 1,1,1 };
		camera.theta = Pi / 6.0f;
		camera.phi = 0;
		light = { 1,1,1 };

		projection = camera_projection::perspective(50.0f * Pi / 180, 1.0f, 0.1f, 500.0f);
	}

	void opengl_uniform(GLuint shader, scene_environment_basic const& environment)
	{
		opengl_uniform(shader, "projection", environment.projection.matrix());
		opengl_uniform(shader, "view", environment.camera.matrix_view());
		opengl_uniform(shader, "light", environment.light, false);
	}
	void opengl_uniform(GLuint shader, scene_environment_basic_camera_spherical_coords const& environment)
	{
		opengl_uniform(shader, "projection", environment.projection.matrix());
		opengl_uniform(shader, "view", environment.camera.matrix_view());
		opengl_uniform(shader, "light", environment.light, false);
	}
}


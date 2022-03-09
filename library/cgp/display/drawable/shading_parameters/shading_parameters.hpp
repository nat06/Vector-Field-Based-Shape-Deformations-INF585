#pragma once

#include "cgp/containers/buffer_stack/buffer_stack.hpp"
#include "cgp/display/opengl/glad/glad.hpp"

namespace cgp
{
	struct shading_parameters_phong
	{
		vec3 color  = vec3{1.0f,1.0f,1.0f};
		float alpha = 1.0f;
		bool use_texture = true;
		bool texture_inverse_y = false;

		struct phong_parameters{
			float ambient  = 0.3f;
			float diffuse  = 0.6f;
			float specular = 0.3f;
			float specular_exponent = 64.0f;
		} phong;
	};
	void opengl_uniform(GLuint shader, shading_parameters_phong const& phong, bool expected=true);
}
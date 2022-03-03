#pragma once

#include "cgp/display/image/image.hpp"
#include "cgp/display/opengl/opengl.hpp"
#include "cgp/containers/containers.hpp"

namespace cgp
{
	// Read an image from file and initialize an opengl texture image from it
	GLuint opengl_load_texture_image(std::string const& filename, GLint wrap_s = GL_CLAMP_TO_EDGE, GLint wrap_t = GL_CLAMP_TO_EDGE);

	GLuint opengl_load_texture_image(image_raw const& im, GLint wrap_s=GL_CLAMP_TO_EDGE, GLint wrap_t=GL_CLAMP_TO_EDGE);
	GLuint opengl_load_texture_image(grid_2D<vec3> const& im, GLint wrap_s=GL_CLAMP_TO_EDGE, GLint wrap_t=GL_CLAMP_TO_EDGE);
	void opengl_update_texture_image(GLuint texture_id, grid_2D<vec3> const& im);
    
}
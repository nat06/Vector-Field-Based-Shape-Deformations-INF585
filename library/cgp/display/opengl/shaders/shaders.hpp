#pragma once

#include "../glad/glad.hpp"

#include <string>

namespace cgp
{
	/** Load and compile shaders from glsl file sources
	* Display warnings and errors if the file cannot be accessed.
	* Display debug info when the shader is succesfully compiled. */
	GLuint opengl_load_shader(std::string const& vertex_shader_path, std::string const& fragment_shader_path);
	
	/** Compile shaders from direct text input.
	* Display no debug information in case of success */
	GLuint opengl_load_shader_from_text(std::string const& vertex_shader, std::string const& fragment_shader);
}
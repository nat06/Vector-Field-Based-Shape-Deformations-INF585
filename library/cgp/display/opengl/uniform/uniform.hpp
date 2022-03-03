#pragma once

#include <string>
#include "cgp/display/opengl/glad/glad.hpp"
#include "cgp/math/math.hpp"

namespace cgp
{
	void opengl_uniform(GLuint shader, std::string const& name, int value, bool expected=true);
	void opengl_uniform(GLuint shader, std::string const& name, GLuint value, bool expected = true);
	void opengl_uniform(GLuint shader, std::string const& name, float value, bool expected=true);
	void opengl_uniform(GLuint shader, std::string const& name, vec3 const& value, bool expected=true);
	void opengl_uniform(GLuint shader, std::string const& name, vec4 const& value, bool expected=true);
	void opengl_uniform(GLuint shader, std::string const& name, float x, float y, float z, bool expected=true);
	void opengl_uniform(GLuint shader, std::string const& name, float x, float y, float z, float w, bool expected=true);
	void opengl_uniform(GLuint shader, std::string const& name, mat4 const& m, bool expected=true);
	void opengl_uniform(GLuint shader, std::string const& name, mat3 const& m, bool expected=true);
}


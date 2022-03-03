#pragma once

#include "cgp/display/opengl/opengl.hpp"
#include "cgp/containers/containers.hpp"
#include <map>

namespace cgp
{
	struct segments_drawable
	{
		segments_drawable();
		segments_drawable& initialize(buffer<vec3> const& position, std::string const& object_name = "unset_name", GLuint shader = default_shader);
		segments_drawable& update(buffer<vec3> const& new_position);
		segments_drawable& clear();


		GLuint vbo_position;
		GLuint vao;

		GLuint number_position;
		GLuint shader;

		// Uniform
		affine_rts transform;
		vec3 color;

		std::string name;
		
		static GLuint default_shader;
		
	};
}


namespace cgp
{
	template <typename SCENE>
	void draw(segments_drawable const& drawable, SCENE const& scene)
	{
		// Setup shader
		assert_cgp(drawable.shader!=0, "Try to draw segments_drawable without shader ("+drawable.name+")");
		glUseProgram(drawable.shader); opengl_check;

		// Send uniforms for this shader
		opengl_uniform(drawable.shader, scene);
		opengl_uniform(drawable.shader, "color", drawable.color);
		opengl_uniform(drawable.shader, "model", drawable.transform.matrix());

		// Call draw function
		assert_cgp(drawable.number_position>0, "Try to draw segments_drawable with 0 position (" + drawable.name + ")"); opengl_check;
		glBindVertexArray(drawable.vao); opengl_check;
		glDrawArrays(GL_LINES, 0, drawable.number_position); opengl_check;

		// Clean buffers
		glBindVertexArray(0);
	}
}
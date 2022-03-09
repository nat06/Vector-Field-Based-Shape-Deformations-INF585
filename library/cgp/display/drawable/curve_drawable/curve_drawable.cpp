#include "cgp/base/base.hpp"
#include "curve_drawable.hpp"

namespace cgp
{
	GLuint curve_drawable::default_shader = 0;

	curve_drawable::curve_drawable()
		:vbo_position(0), vao(0), number_position(0), shader(0), transform(), color({0,0,0}), name("uninitialized")
	{}

	curve_drawable& curve_drawable::initialize(buffer<vec3> const& position, std::string const& object_name, GLuint shader_arg)
	{
		// Sanity check and warning on command line if initialize is called on some segments_drawable already associated to data
		if (vbo_position != 0)
			warning_cgp("Warning try to initialize curve_drawable [" + name + "] with existing data", "");

		// Initialize the data to standard values
		vbo_position = 0;
		vao = 0;
		number_position = 0;
		shader = shader_arg;
		transform = affine_rts();
		color = { 0,0,0 };
		name = object_name;

		// Send the data to the GPU
		opengl_create_gl_buffer_data(GL_ARRAY_BUFFER, vbo_position, position, GL_DYNAMIC_DRAW);
		number_position = static_cast<GLuint>(position.size());

		glGenVertexArrays(1, &vao); opengl_check
		glBindVertexArray(vao);     opengl_check
		opengl_set_vertex_attribute(vbo_position, 0, 3, GL_FLOAT);
		glBindVertexArray(0);       opengl_check

		return *this;
	}


	curve_drawable& curve_drawable::update(buffer<vec3> const& new_position)
	{
		opengl_update_gl_subbuffer_data(vbo_position, new_position);
		return *this;
	}

	curve_drawable& curve_drawable::clear()
	{
		glDeleteBuffers(1, &vbo_position ); 
		vbo_position = 0;

		glDeleteVertexArrays(1, &vao);
		vao = 0;
		opengl_check;
		
		number_position = 0;
		shader = 0;
		transform = affine_rts();
		color = {0,0,0};

		name = "uninitialized";

		return *this;
	}

}
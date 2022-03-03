#include "triangle_soup_drawable.hpp"

#include "cgp/base/base.hpp"

namespace cgp
{
	GLuint triangle_soup_drawable::default_shader = 0;


	triangle_soup_drawable::triangle_soup_drawable()
		:vbo(), vao(0), number_elements(0), shader(0), transform(), anisotropic_scale({ 1,1,1 }), shading()
	{}

	void triangle_soup_drawable::send_data_to_GPU(std::vector<vec3> const& position, std::vector<vec3> const& normal, GLuint shader_arg, GLuint draw_type)
	{
		// Sanity check OpenGL
		opengl_check;

		if (position.size() == 0) {
			warning_cgp("Warning try to generate triangle_soup_drawable with 0 vertex","");
			return;
		}

		shader = shader_arg;


		// Fill vbo for position
		opengl_create_gl_buffer_data(GL_ARRAY_BUFFER, vbo["position"], position, draw_type);
		opengl_create_gl_buffer_data(GL_ARRAY_BUFFER, vbo["normal"], normal, draw_type);

		// Store number of triangles
		number_elements = static_cast<GLuint>(position.size());

		// Generate VAO
		glGenVertexArrays(1,&vao); opengl_check
		glBindVertexArray(vao);    opengl_check
		opengl_set_vertex_attribute(vbo["position"], 0, 3, GL_FLOAT);
		opengl_set_vertex_attribute(vbo["normal"],   1, 3, GL_FLOAT);
		glBindVertexArray(0);      opengl_check
	}


	triangle_soup_drawable& triangle_soup_drawable::update_position(std::vector<vec3> const& new_position, size_t number_elements_arg)
	{
		glBindBuffer(GL_ARRAY_BUFFER,vbo["position"]); opengl_check;
		glBufferSubData(GL_ARRAY_BUFFER, 0, number_elements_arg*sizeof(float)*3, &new_position[0]);  opengl_check;
		return *this;
	}
	triangle_soup_drawable& triangle_soup_drawable::update_normal(std::vector<vec3> const& new_normals, size_t number_elements_arg)
	{
		glBindBuffer(GL_ARRAY_BUFFER,vbo["normal"]); opengl_check;
		glBufferSubData(GL_ARRAY_BUFFER,0, number_elements_arg * sizeof(float) * 3, &new_normals[0]);  opengl_check;
		return *this;
	}

	void triangle_soup_drawable::clear()
	{
		for(auto& buffer : vbo)
			glDeleteBuffers(1, &(buffer.second) ); 
		vbo.clear();

		glDeleteVertexArrays(1, &vao);
		vao = 0;
		opengl_check;
		
		number_elements = 0;
		shader = 0;
		transform = affine_rts();
		shading = shading_parameters_phong();
	}

}
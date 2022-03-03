#include "mesh_drawable.hpp"

#include "cgp/base/base.hpp"

namespace cgp
{
	GLuint mesh_drawable::default_shader = 0;
	GLuint mesh_drawable::default_texture = 0;


	mesh_drawable::mesh_drawable()
		:vbo(), vao(0), number_triangles(0), shader(0), texture(0), transform(), anisotropic_scale({ 1,1,1 }), shading(), name("uninitialized")
	{}

	static void warning_initialize(std::string const previous_name, std::string const current_name)
	{
		std::string warning="\n";
		warning += "  > The original mesh_drawable is called [" + previous_name + "]\n";
		warning += "  > The current mesh_drawble is called [" + current_name + "]\n";
		warning += "In normal condition, you should avoid initializing mesh_drawble without clearing the previous one - the allocated memory on the GPU is going to be lost.\n";
		warning += " - If you want to clear the memory, please call mesh_drawable.clear() before calling a new initialization\n";
		warning += " - If the vbo is shared between several instances of mesh_drawable and you don't want to clear this memory, you can create a new default mesh_drawable() before applying the initialization.\n";
		
		warning_cgp("Calling mesh_drawable.initialize() on a structure mesh_drawable with non zero VBOs", warning);
	}

	mesh_drawable& mesh_drawable::initialize(mesh const& data_to_send, std::string const& object_name, GLuint shader_arg, GLuint texture_arg)
	{
		// Error detection before sending the data to avoid unexpected behavior
		// *********************************************************************** //

		opengl_check;

		// Check if this mesh_drawable is already initialized
		if (vbo.size() != 0 || vao != 0)
			warning_initialize(name, object_name);

		if (data_to_send.position.size() == 0) {
			warning_cgp("Warning try to generate mesh_drawable ["+name+"] with 0 vertex","");
			return *this;
		}

		// Sanity check before sending mesh data to GPU
		assert_cgp(mesh_check(data_to_send), "Cannot send this mesh data to GPU in initializing mesh_drawable with name ["+name+"]");


		// Variable initialization
		// *********************************************************************** //

		name = object_name;
		shader = shader_arg;
		texture = texture_arg;
		transform = affine_rts();
		shading = shading_parameters_phong();


		// Send the data to the GPU
		// ******************************************** //

		// Fill VBOs
		opengl_create_gl_buffer_data(GL_ARRAY_BUFFER, vbo["position"], data_to_send.position, GL_DYNAMIC_DRAW);
		opengl_create_gl_buffer_data(GL_ARRAY_BUFFER, vbo["normal"], data_to_send.normal, GL_DYNAMIC_DRAW);
		opengl_create_gl_buffer_data(GL_ARRAY_BUFFER, vbo["color"], data_to_send.color, GL_DYNAMIC_DRAW);
		opengl_create_gl_buffer_data(GL_ARRAY_BUFFER, vbo["uv"], data_to_send.uv, GL_DYNAMIC_DRAW);
		opengl_create_gl_buffer_data(GL_ELEMENT_ARRAY_BUFFER, vbo["index"], data_to_send.connectivity, GL_DYNAMIC_DRAW);

		// Store number of triangles
		number_triangles = static_cast<GLuint>(data_to_send.connectivity.size());

		// Generate VAO
		glGenVertexArrays(1,&vao); opengl_check
		glBindVertexArray(vao);    opengl_check
		opengl_set_vertex_attribute(vbo["position"], 0, 3, GL_FLOAT);
		opengl_set_vertex_attribute(vbo["normal"],   1, 3, GL_FLOAT);
		opengl_set_vertex_attribute(vbo["color"],    2, 3, GL_FLOAT);
		opengl_set_vertex_attribute(vbo["uv"],       3, 2, GL_FLOAT);
		glBindVertexArray(0);      opengl_check

		return *this;
	}


	mesh_drawable& mesh_drawable::update_position(buffer<vec3> const& new_position)
	{
		glBindBuffer(GL_ARRAY_BUFFER,vbo["position"]); opengl_check;
		glBufferSubData(GL_ARRAY_BUFFER,0,size_in_memory(new_position),ptr(new_position));  opengl_check;
		return *this;
	}
	mesh_drawable& mesh_drawable::update_normal(buffer<vec3> const& new_normals)
	{
		glBindBuffer(GL_ARRAY_BUFFER,vbo["normal"]); opengl_check;
		glBufferSubData(GL_ARRAY_BUFFER,0,size_in_memory(new_normals),ptr(new_normals));  opengl_check;
		return *this;
	}
	mesh_drawable& mesh_drawable::update_color(buffer<vec3> const& new_color)
	{
		glBindBuffer(GL_ARRAY_BUFFER,vbo["color"]); opengl_check;
		glBufferSubData(GL_ARRAY_BUFFER,0,size_in_memory(new_color),ptr(new_color));  opengl_check;
		return *this;
	}
	mesh_drawable& mesh_drawable::update_uv(buffer<vec2> const& new_uv)
	{
		glBindBuffer(GL_ARRAY_BUFFER,vbo["uv"]); opengl_check;
		glBufferSubData(GL_ARRAY_BUFFER,0,size_in_memory(new_uv),ptr(new_uv));  opengl_check;
		return *this;
	}

	mesh_drawable& mesh_drawable::clear()
	{
		for(auto& buffer : vbo)
			glDeleteBuffers(1, &(buffer.second) ); 
		vbo.clear();

		glDeleteVertexArrays(1, &vao);
		vao = 0;
		opengl_check;
		
		number_triangles = 0;
		shader = 0;
		texture = 0;
		transform = affine_rts();
		anisotropic_scale = { 1,1,1 };
		shading = shading_parameters_phong();
		name = "uninitialized";

		return *this;
	}

	mat4 mesh_drawable::model_matrix() const
	{
		mat4 const model = transform.matrix() * mat4::diagonal(anisotropic_scale);
		return model;
	}

}
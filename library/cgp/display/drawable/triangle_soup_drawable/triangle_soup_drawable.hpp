#pragma once

#include <vector>
#include <string>
#include "cgp/display/opengl/opengl.hpp"
#include "../shading_parameters/shading_parameters.hpp"

namespace cgp
{
	struct triangle_soup_drawable
	{
		triangle_soup_drawable();
		// Send mesh data to GPU and store IDs into vbo. Set also shader and texture.
		//explicit triangle_soup_drawable(mesh const& data_to_send, GLuint shader=default_shader, GLuint texture=default_texture, GLuint draw_type=GL_DYNAMIC_DRAW);

		void send_data_to_GPU(std::vector<vec3> const& position, std::vector<vec3> const& normal, GLuint shader_arg = default_shader, GLuint draw_type = GL_DYNAMIC_DRAW);

		// Stores VBO ID in GPU_elements_id
		std::map<std::string, GLuint> vbo;
		GLuint vao;

		GLsizei number_elements;
		GLuint shader;

		// Uniform
		affine_rts transform;
		vec3 anisotropic_scale;
		shading_parameters_phong shading;

		static GLuint default_shader;

		void clear();
		triangle_soup_drawable& update_position(std::vector<vec3> const& new_position, size_t number_elements);
		triangle_soup_drawable& update_normal(std::vector<vec3> const& new_normal, size_t number_elements);
	};

	template <typename SCENE_ENVIRONMENT>
	void draw(triangle_soup_drawable const& drawable, SCENE_ENVIRONMENT const& environment);

	template <typename SCENE_ENVIRONMENT>
	void draw_wireframe(triangle_soup_drawable const& drawable, SCENE_ENVIRONMENT const& environment, vec3 const& color={0,0,1});
}


namespace cgp
{
	template <typename SCENE_ENVIRONMENT>
	void draw(triangle_soup_drawable const& drawable, SCENE_ENVIRONMENT const& environment)
	{
		if (drawable.number_elements == 0) return;

		// Setup shader
		assert_cgp(drawable.shader!=0, "Try to draw triangle_soup_drawable without shader");
		glUseProgram(drawable.shader); opengl_check;

		// Send uniforms for this shader
		opengl_uniform(drawable.shader, environment);
		opengl_uniform(drawable.shader, drawable.shading);

		mat4 const model = drawable.transform.matrix() * mat4::diagonal(drawable.anisotropic_scale);
		opengl_uniform(drawable.shader, "model", model);

		// Call draw function
		assert_cgp(drawable.number_elements>0, "Try to draw triangle_soup_drawable with 0 elements"); opengl_check;
		glBindVertexArray(drawable.vao);   opengl_check;
		glDrawArrays(GL_TRIANGLES, 0, drawable.number_elements); opengl_check;

		// Clean buffers
		glBindVertexArray(0);
	}

	template <typename SCENE_ENVIRONMENT>
	void draw_wireframe(triangle_soup_drawable const& drawable, SCENE_ENVIRONMENT const& scene, vec3 const& color)
	{
		triangle_soup_drawable wireframe = drawable;
		wireframe.shading.phong = {1.0f,0.0f,0.0f,64.0f};
		wireframe.shading.color = color;
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0, 1.0);
		draw(wireframe, scene);
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
}
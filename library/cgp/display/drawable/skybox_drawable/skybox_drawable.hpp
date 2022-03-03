#pragma once

#include "../mesh_drawable/mesh_drawable.hpp"

namespace cgp {

	struct skybox_drawable : mesh_drawable {

		skybox_drawable& initialize(std::string const& texture_directory_path, std::string const& object_name = "skybox");

	};

	template <typename SCENE_ENVIRONMENT>
	void draw(mesh_drawable const& drawable, SCENE_ENVIRONMENT const& environment);
}

namespace cgp {

	template <typename SCENE_ENVIRONMENT>
	void draw(skybox_drawable const& skybox, SCENE_ENVIRONMENT const& environment)
	{
		glDepthMask(GL_FALSE);
		

		if (skybox.number_triangles == 0) return;

		// Setup shader
		assert_cgp(skybox.shader != 0, "Try to draw skybox_drawable without shader [name:" + skybox.name + "]");
		assert_cgp(skybox.texture != 0, "Try to draw skybox_drawable without texture [name:" + skybox.name + "]");
		glUseProgram(skybox.shader); opengl_check;

		// Send uniforms for this shader
		opengl_uniform(skybox.shader, "projection", environment.projection.matrix());
		opengl_uniform(skybox.shader, "view", environment.camera.matrix_view());
		opengl_uniform(skybox.shader, "model", skybox.model_matrix());

		// Set texture as a cubemap (different from the 2D texture using in the "standard" draw call)
		glActiveTexture(GL_TEXTURE0); opengl_check;
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.texture); opengl_check;
		cgp::opengl_uniform(skybox.shader, "image_texture", 0);  opengl_check;

		// Call draw function
		assert_cgp(skybox.number_triangles > 0, "Try to draw mesh_drawable with 0 triangles [name:" + skybox.name + "]"); opengl_check;
		glBindVertexArray(skybox.vao);   opengl_check;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox.vbo.at("index")); opengl_check;
		glDrawElements(GL_TRIANGLES, GLsizei(skybox.number_triangles * 3), GL_UNSIGNED_INT, nullptr); opengl_check;

		// Clean buffers
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDepthMask(GL_TRUE);
	}
}


#include "skybox_drawable.hpp"

namespace cgp {

	static const std::string skybox_fragment_shader = R"(
		#version 330 core
		in struct fragment_data
		{
			vec3 position;
		} fragment;

		layout(location=0) out vec4 FragColor;

		uniform samplerCube image_texture;

		void main()
		{
			FragColor = texture(image_texture, fragment.position);
		}
		)";

	static const std::string skybox_vertex_shader = R"(
		#version 330 core
		layout (location = 0) in vec3 position;

		out struct fragment_data
		{
			vec3 position;
		} fragment;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			fragment.position = vec3( mat3(model) * position);
			gl_Position = projection * mat4(mat3(view)) * model * vec4(position, 1.0);
		}
		)";

	static GLuint load_cubemap_texture(std::string const& directory_path)
	{
		// Load images
		image_raw const x_neg = image_load_file(directory_path + "x_neg.jpg");
		image_raw const x_pos = image_load_file(directory_path + "x_pos.jpg");
		image_raw const y_pos = image_load_file(directory_path + "y_pos.jpg");
		image_raw const y_neg = image_load_file(directory_path + "y_neg.jpg");
		image_raw const z_pos = image_load_file(directory_path + "z_pos.jpg");
		image_raw const z_neg = image_load_file(directory_path + "z_neg.jpg");

		// Send images to GPU as cubemap
		GLuint cubemap;
		glGenTextures(1, &cubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB8, x_neg.width, x_neg.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(x_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB8, x_pos.width, x_pos.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(x_pos.data));

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB8, y_neg.width, y_neg.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(y_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB8, y_pos.width, y_pos.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(y_pos.data));

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB8, z_neg.width, z_neg.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(z_neg.data));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB8, z_pos.width, z_pos.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr(z_pos.data));


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return cubemap;
	}

	skybox_drawable& skybox_drawable::initialize(std::string const& texture_directory_path, std::string const& object_name)
	{
		GLuint const shader = opengl_load_shader_from_text(skybox_vertex_shader, skybox_fragment_shader);
		mesh const cube_mesh = mesh_primitive_cube({ 0,0,0 }, 2.0f);
		
		GLuint texture_id = load_cubemap_texture(texture_directory_path);

		static_cast<mesh_drawable*>(this)->initialize(cube_mesh, object_name, shader, texture_id);

		return *this;
		
	}

}
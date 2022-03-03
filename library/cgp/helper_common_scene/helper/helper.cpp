#include "helper.hpp"

#include "cgp/base/base.hpp"
#include "cgp/files/files.hpp"
#include "cgp/shape/shape.hpp"
#include "cgp/display/display.hpp"
#include "cgp/interaction/interaction.hpp"

namespace cgp {


	static void display_error_file_access();

	void helper_common_scene::initialize()
	{
		// Assume that the default shader file will be accessible in shaders/mesh/ and shaders/single_color/ directories
		if (check_file_exist("shaders/mesh/vert.glsl") == false) {
			display_error_file_access();
			exit(1);
		}


		//   - Shader used to display meshes
		GLuint const shader_mesh = opengl_load_shader("shaders/mesh/vert.glsl", "shaders/mesh/frag.glsl");
		//   - Shader used to display constant color (ex. for curves)
		GLuint const shader_uniform_color = opengl_load_shader("shaders/single_color/vert.glsl", "shaders/single_color/frag.glsl");

		// Set standard mesh shader for mesh_drawable
		mesh_drawable::default_shader = shader_mesh;
		// Set default white texture
		mesh_drawable::default_texture = opengl_load_texture_image(image_raw{ 1,1,image_color_type::rgba,{255,255,255,255} });

		// Set standard uniform color for curve/segment_drawable
		curve_drawable::default_shader    = shader_uniform_color;
		segments_drawable::default_shader = shader_uniform_color;


		fps_record.start();
	}

	static void display_error_file_access()
	{
		std::cout << "[ERROR File Access] The default initialization from helper_common_scene tried to load the shader file shaders/mesh/vert.glsl but cannot not find it" << std::endl;
		std::cout << "  => In most situation, the problem is the following: Your executable is not run from the root directory of this scene, and the directory shaders/ is therefore not accessible. " << std::endl;
		std::cout << "  => To solve this problem, you may need to adjust your IDE settings (or your placement in command line) such that your executable is run from the parent directory of shaders/. Then run again the program. " << std::endl;
		std::cout << "\n\nThe program will now exit" << std::endl;
	}

	void helper_common_scene::frame_begin(vec3 const& background_color, GLFWwindow* window, windows_dimension_structure const& window_dimension, bool& cursor_on_gui)
	{
		glViewport(0, 0, window_dimension.width, window_dimension.height);

		glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		fps_record.update();
		if (fps_record.event) {
			std::string const title = "CGP Display - " + str(fps_record.fps) + " fps";
			glfwSetWindowTitle(window, title.c_str());
		}

		imgui_create_frame();
		ImGui::Begin("GUI", NULL, ImGuiWindowFlags_AlwaysAutoResize);
		cursor_on_gui = ImGui::GetIO().WantCaptureMouse;
	}

	void helper_common_scene::frame_end(GLFWwindow* window)
	{
		ImGui::End();
		imgui_render_frame(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}



}
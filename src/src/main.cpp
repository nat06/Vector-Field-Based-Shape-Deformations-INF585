
#include "cgp/cgp.hpp" // Give access to the complete CGP library
#include <iostream> 

// Custom scene of this code
#include "scene.hpp"

// *************************** //
// Global Variables
// *************************** //

// A helper tool to factorize common default functions (Window initialization, code that starts frame, etc)
cgp::helper_common_scene helper_common;

// A storage for the current values of the inputs (mouse, keyboard, window dimension) that can be use for interaction purpose
cgp::inputs_interaction_parameters inputs;

// The custom structure of the current scene defined in "scene.hpp"
scene_structure scene;

// *************************** //
// Start of the program
// *************************** //

GLFWwindow* standard_window_initialization(int width, int height);

int main(int, char* argv[]){
	std::cout << "Run " << argv[0] << std::endl;

	// ************************ //
	//     INITIALISATION
	// ************************ //
	
	// Standard Initialization with dimension in pixels
	GLFWwindow* window = standard_window_initialization(1280, 1080); 
	
	// Custom scene initialization
	std::cout << "Initialize data of the scene ..." << std::endl;
	scene.initialize();                                              
	std::cout << "Initialization success" << std::endl;

	// ************************ //
	//     Animation Loop
	// ************************ //
	std::cout<<"Start animation loop ..."<<std::endl;
	while (!glfwWindowShouldClose(window))
	{
		// Reset the screen for a new frame
		helper_common.frame_begin(scene.environment.background_color, window, inputs.window, inputs.mouse.on_gui);
		scene.environment.projection.update_aspect_ratio(inputs.window.aspect_ratio());
		
		// Display the ImGUI interface (button, sliders, etc)
		scene.display_gui();

		// Call the display of the scene
		scene.display();
		
		// End of ImGui display and handle GLFW events
		helper_common.frame_end(window);
	}
	
	// Cleanup
	cgp::imgui_cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

// This function is called everytime the window is resized
void window_size_callback(GLFWwindow* , int width, int height){
	inputs.window = { width, height };
}

// This function is called everytime the mouse is moved
void mouse_move_callback(GLFWwindow* /*window*/, double xpos, double ypos){
	inputs.mouse_position_update( { xpos, ypos } );
	scene.mouse_move(inputs);
	if(!inputs.keyboard.shift)
		camera_standard_behavior_rotation_trackball(scene.environment.camera, inputs);	
}

// This function is called everytime a mouse button is clicked/released
void mouse_click_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/){
	inputs.mouse.click.update_from_glfw_click(button, action);

	if(button==GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		scene.mouse_left_released();
	else
		scene.mouse_click(inputs);
}

void mouse_scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset){
	//scene.mouse_scroll(yoffset);
	scene.environment.camera.manipulator_scale_distance_to_center(-yoffset/10);//zoom in/out
}

// This function is called everytime a keyboard touch is pressed/released
void keyboard_callback(GLFWwindow* /*window*/, int key, int , int action, int /*mods*/){
	inputs.keyboard.update_from_glfw_key(key, action);
}

// Standard initialization procedure
GLFWwindow* standard_window_initialization(int width, int height){
	// Update storage for window size for the scene
	inputs.window = { width, height };

	// Create the window using GLFW
	GLFWwindow* window = cgp::create_window(width, height);

	// Display debug information on command line
	std::cout << cgp::opengl_info_display() << std::endl;

	// Initialize ImGUI
	cgp::imgui_init(window);

	// Set the callback functions for the inputs
	glfwSetKeyCallback(window, keyboard_callback);            // Event called when a keyboard touch is pressed/released
	glfwSetMouseButtonCallback(window, mouse_click_callback); // Event called when a button of the mouse is clicked/released
	glfwSetCursorPosCallback(window, mouse_move_callback);    // Event called when the mouse is moved
	glfwSetWindowSizeCallback(window, window_size_callback);  // Event called when the window is rescaled              
	glfwSetScrollCallback(window, mouse_scroll_callback);

	// Load default shader and initialize default frame
	helper_common.initialize();

	return window;
}

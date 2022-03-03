#pragma once

#include <GLFW/glfw3.h>
#include "cgp/containers/containers.hpp"

namespace cgp {

struct windows_dimension_structure {
	int width  = 1280;
	int height = 1080;
	bool is_resized = false;
	float aspect_ratio() const;
};

struct inputs_mouse_cursor_position_parameters {
	vec2 current;
	vec2 previous;
};

enum class last_mouse_cursor_action { click_left, click_right, release_left, release_right };
struct inputs_mouse_cursor_click_parameters {
	bool left  = false;
	bool right = false;
	last_mouse_cursor_action last_action;
	void update_from_glfw_click(int button, int action);
};


struct inputs_mouse_parameters {
	inputs_mouse_cursor_position_parameters position; // Stores current and previous position of the mouse cursor in OpenGL relative coordinates [-1,1]
	inputs_mouse_cursor_click_parameters click;       // Information on clicked button (left/right)
	bool on_gui = false;                              // true if the cursor currently on ImGui GUI
};

struct inputs_keyboard_parameters {

	bool shift;
	bool ctrl;
	bool up, down, right, left;

	inputs_keyboard_parameters();
	void update_from_glfw_key(int key, int action);
};




struct inputs_interaction_parameters {
	windows_dimension_structure window;
	inputs_keyboard_parameters  keyboard;
	inputs_mouse_parameters     mouse;

	void mouse_position_update(vec2 const& new_mouse_position);
};

}
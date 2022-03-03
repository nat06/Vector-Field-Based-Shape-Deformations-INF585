#include "glfw_inputs.hpp"
#include "cgp/base/base.hpp"

namespace cgp {


static vec2 mouse_position_pixel_to_relative(vec2 const& p_pixel, int window_width, int window_height)
{
	float const x_rel = p_pixel.x / static_cast<float>(window_width);
	float const y_rel = p_pixel.y / static_cast<float>(window_height);

	return vec2{
		2.0f * x_rel - 1.0f,
		2.0f * (1.0f - y_rel) - 1.0f
	};
}


float windows_dimension_structure:: aspect_ratio() const
{
	return width / static_cast<float>(height);
}

void inputs_interaction_parameters::mouse_position_update(vec2 const& new_mouse_position) {
	mouse.position.previous = mouse.position.current;
	mouse.position.current = mouse_position_pixel_to_relative(new_mouse_position, window.width, window.height);
}

void inputs_mouse_cursor_click_parameters::update_from_glfw_click(int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		left = true;
		last_action = last_mouse_cursor_action::click_left;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		left = false;
		last_action = last_mouse_cursor_action::release_left;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		right = true;
		last_action = last_mouse_cursor_action::click_right;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		right = false;
		last_action = last_mouse_cursor_action::release_right;
	}
}

void inputs_keyboard_parameters::update_from_glfw_key(int key, int action)
{
	// Shift and CTRL
	if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_PRESS)
		shift = true;
	if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_RELEASE)
		shift = false;

	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) && action == GLFW_PRESS)
		ctrl = true;
	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) && action == GLFW_RELEASE)
		ctrl = false;

	// Up/Down/Left/Right
	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) up = true;
		if (action == GLFW_RELEASE) up = false;
	}

	if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) down = true;
		if (action == GLFW_RELEASE) down = false;
	}

	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) left = true;
		if (action == GLFW_RELEASE) left = false;
	}

	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) right = true;
		if (action == GLFW_RELEASE) right = false;
	}
}

inputs_keyboard_parameters::inputs_keyboard_parameters()
	:shift(false), ctrl(false), up(false), down(false), right(false), left(false)
{}


}
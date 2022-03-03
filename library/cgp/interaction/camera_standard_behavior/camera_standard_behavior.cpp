#include "camera_standard_behavior.hpp"

namespace cgp {

	void camera_standard_behavior_rotation_trackball(camera_around_center& camera, inputs_interaction_parameters& inputs)
	{
		vec2 const& p1 = inputs.mouse.position.current;
		vec2 const& p0 = inputs.mouse.position.previous;

		bool const event_valid = !inputs.mouse.on_gui;
		bool const click_left = inputs.mouse.click.left;
		bool const click_right = inputs.mouse.click.right;
		bool const ctrl = inputs.keyboard.ctrl;

		if (event_valid) { // If the mouse cursor is not on the ImGui area

			if (click_left && !ctrl)     // Rotation of the camera around its center
				camera.manipulator_rotate_trackball(p0, p1);
			else if (click_left && ctrl) // Translate/Pan the camera in the viewspace plane
				camera.manipulator_translate_in_plane(p1 - p0);
			else if (click_right)        // Move the camera closer/further with respect to its center
				camera.manipulator_scale_distance_to_center((p1 - p0).y);
		}
	}

	void camera_standard_behavior_rotation_spherical_coordinates(camera_spherical_coordinates& camera, inputs_interaction_parameters& inputs)
	{
		vec2 const& p1 = inputs.mouse.position.current;
		vec2 const& p0 = inputs.mouse.position.previous;

		bool const event_valid = !inputs.mouse.on_gui;
		bool const click_left = inputs.mouse.click.left;
		bool const click_right = inputs.mouse.click.right;
		bool const ctrl = inputs.keyboard.ctrl;

		if (event_valid) {
			if (click_left && !ctrl)
				camera.manipulator_rotate_spherical_coordinates((p1 - p0).x, -(p1 - p0).y);
			if (click_left &&  ctrl)
				camera.manipulator_translate_in_plane(p1 - p0);
			if (click_right)
				camera.manipulator_scale_distance_to_center((p1 - p0).y);
		}

	}




}
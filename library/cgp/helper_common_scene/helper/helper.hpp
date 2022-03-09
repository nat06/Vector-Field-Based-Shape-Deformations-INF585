#pragma once

#include "cgp/display/display.hpp"
#include "cgp/math/math.hpp"
#include "cgp/interaction/interaction.hpp"



namespace cgp {

	struct helper_common_scene {
		timer_fps fps_record;


		void initialize();
		void frame_begin(vec3 const& background_color, GLFWwindow* window, windows_dimension_structure const& window_dimension, bool& cursor_on_gui);
		void frame_end(GLFWwindow* window);
	};

}
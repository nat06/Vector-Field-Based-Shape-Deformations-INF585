#pragma once
#include "cgp/cgp.hpp"
#include "../deformers/deformers.hpp"

enum surface_type_enum {
	surface_plane,
	surface_cylinder,
	surface_sphere,
	surface_cube,
	surface_head,
	surface_camel,
	surface_armadillo, 
	surface_tyrannosaur,
	surface_spoon,
	surface_bunny,
	surface_body
};

struct gui_parameters {

	bool display_frame = false;
	bool display_wireframe = false;
	bool display_grid_edge = false;
	bool display_grid_box = true;
	bool display_constant_velocity = true;
	float gui_ri;
	float gui_r0;
	bool bool_laplacian_smoothing;
	int smoothing_steps;
	bool bool_trilinear_interpolation;
	bool bool_display_tool;

	constant_velocity_structure constant_velocity_parameters;
	surface_type_enum surface_type = surface_plane; // Type of surface to be created

	bool display();
};

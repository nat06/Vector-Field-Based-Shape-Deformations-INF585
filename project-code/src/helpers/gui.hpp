#pragma once

#include "cgp/cgp.hpp"
#include "../deformers/deformers.hpp"

enum surface_type_enum {
	surface_plane,
	surface_cylinder,
	surface_sphere,
	surface_cube,
	surface_mesh,
	surface_mesh_2,
	surface_mesh_3 //TO DO: CHANGE THESE NAMES TO FACE, CAMEL...
};


struct gui_parameters {
	bool display_frame = false;
	bool display_wireframe = false;

	//#
	//PROJECT
	bool display_grid_edge = false;
	bool display_velocity = true;
	bool display_constant_velocity = true;
	float gui_ri;
	float gui_r0;
	bool laplacian_smoothing = false;
	int smoothing_steps;

	constant_velocity_direction velocity_direction_type;

	//#

	surface_type_enum surface_type = surface_plane;    // Type of surface to be created
	deformer_parameters_structure deformer_parameters; // Type of deformation to apply

	bool display();
};

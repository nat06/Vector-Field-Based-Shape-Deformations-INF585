#pragma once

#include "cgp/cgp.hpp"

#include "helpers/gui.hpp"
#include "helpers/initialization.hpp"
#include "helpers/picking_visual.hpp"

#include "deformers/deformers.hpp"


// Helping structure that contains the deforming shape elements
struct deforming_shape_structure
{
	cgp::mesh shape;                       // Mesh structure of the deformed shape
	cgp::buffer<cgp::vec3> position_saved; // Extra storage of the shape position before the current deformation
	cgp::mesh_drawable visual;             // Visual representation of the deformed shape

	bool require_normal_update;            // indicator if the normals need to be updated

	void update_normal();
	void new_shape(surface_type_enum type_of_surface= surface_plane);
};


// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable global_frame;          // The standard global frame
	cgp::scene_environment_basic environment; // Standard environment controler
	gui_parameters gui;                       // Standard GUI element storage
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	deforming_shape_structure deforming_shape;

	cgp::picking_structure picking;
	picking_visual_parameters picking_visual;
	cgp::timer_event_periodic timer_update_normal; // timer with periodic events used to update the normals

	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop


	void mouse_move(cgp::inputs_interaction_parameters const& inputs);
	void mouse_scroll(float scroll_offset);
	void mouse_left_released();
};






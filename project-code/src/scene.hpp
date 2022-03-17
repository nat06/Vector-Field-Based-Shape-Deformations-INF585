#pragma once

#include "cgp/cgp.hpp"

#include "helpers/gui.hpp"
#include "helpers/initialization.hpp"
#include "helpers/picking_visual.hpp"

#include "deformers/deformers.hpp"
#include "utils.hpp"


// Helping structure that contains the deforming shape elements
struct deforming_shape_structure
{
	cgp::mesh shape;                       // Mesh structure of the deformed shape
	cgp::buffer<cgp::vec3> position_saved; // Extra storage of the shape position before the current deformation
	cgp::mesh_drawable visual;             // Visual representation of the deformed shape

	bool require_normal_update;            // indicator if the normals need to be updated //PROBABLY USELESS NOW
	bool require_deformation;
	bool require_smoothing;            
	cgp::buffer<cgp::buffer<int>> one_ring; 	   // represents one_ring neighborhoods

	cgp::buffer<cgp::vec3> base_normal; // normal of the shape without deformation

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
	
	deforming_shape_structure deforming_shape;
	cgp::picking_structure picking;
	picking_visual_parameters picking_visual;
	
	//cgp::timer_event_periodic timer_update_normal; // timer with periodic events used to update the normals

	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
	void initialize_velocity(int N); //TO DO: REMOVE INT N FROM HERE
	void display_grid(); // Display the 3D grid
	void display_velocity(); // Display the velocity field
	void display_arrow();
	void display_tool(); // display the sphere tool

	void mouse_move(cgp::inputs_interaction_parameters const& inputs);
	void mouse_scroll(float scroll_offset);
	void mouse_left_released();
	void mouse_click(cgp::inputs_interaction_parameters const& inputs);
	

	cgp::grid_3D<cgp::vec3> grid;
	cgp::grid_3D<cgp::vec3> velocity;
	cgp::buffer<cgp::vec3> velocity_grid_data;
	cgp::buffer<cgp::vec3> grid_segments; //edges representing the 3D grid //USEFUL ?
	cgp::segments_drawable grid_segments_visual; //visual representation of the edges of the grid
	cgp::buffer<cgp::vec3> grid_box;
	cgp::segments_drawable grid_box_visual;

	cgp::segments_drawable velocity_visual; //visual representation of the velocity vector field

	cgp::vec3 prev_direction; //USELESS ?
	cgp::vec3 constant_vel; //USELESS ?

	//sphere_tool elements -> might want to make a structure for this !
	sphere_tool_structure sphere_tool;
	cgp::mesh_drawable inner_sphere_visual;
	cgp::mesh_drawable outer_sphere_visual;
	cgp::mesh_drawable arrow_visual ;
	cgp::vec3 previous_tool_pos;


	//cgp::timer_event_periodic timer;//USELESS
	enum constant_velocity_direction velocity_dir_type;//USELESS

	bool previous_laplacian_smoothing;//gui button boolean at the previous display call
	bool previous_interactive_deformation;//deformatiyon with movement of the mouse
	cgp::vec2 previous_mouse_position;
	cgp::vec3 previous_tool_position;
	bool require_update_velocity;
};






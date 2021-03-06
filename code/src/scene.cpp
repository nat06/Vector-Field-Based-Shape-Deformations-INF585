#include "scene.hpp"

using namespace cgp;

void scene_structure::mouse_move(cgp::inputs_interaction_parameters const& inputs)
{
	// Current position of the mouse
	vec2 const& p = inputs.mouse.position.current;

	if (gui.constant_velocity_parameters.type == direction_view && inputs.mouse.click.left) {
		require_update_velocity = true; 
	}

	if (inputs.keyboard.shift){
		require_update_velocity = true; //TO DO: MAKE SURE ABOUT THIS !
		
		// If the mouse is not clicked, compute a picking on the vertices of the mesh
		if (!inputs.mouse.click.left)
			picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.shape.normal, 0.03f, environment.camera, environment.projection);

		// Apply Deformation: press on shift key + left click on the mouse when a vertex is already selected
		if (inputs.mouse.click.left && picking.active){
			if (!previous_interactive_deformation){ //FIX THIS
				previous_mouse_position = inputs.mouse.position.current;
				previous_tool_position = sphere_tool.c;
				//
				sphere_tool.previous_c = sphere_tool.c;
				//
				previous_interactive_deformation = true;
			}

			if (gui.constant_velocity_parameters.type == direction_mouse_movement){
				vec2 const tr_2D = inputs.mouse.position.current - inputs.mouse.position.previous; // translation in the screen plane
				vec3 const tr = environment.camera.orientation() * vec3(tr_2D, 0.0f);
				vec3 new_pos = sphere_tool.c + tr;
				set_tool_in_grid(new_pos, sphere_tool);

				// we compute the velocity field and deform only when the mouse has moved a certain amount
				vec2 const tr_2D_2 = inputs.mouse.position.current - inputs.mouse.position.previous;
				vec3 const tr_2 = environment.camera.orientation() * vec3(tr_2D_2, 0.0f);
				if (norm(tr_2) > 0.001) { // update only everytime it travels a given distance
					gui.constant_velocity_parameters.dir = normalize(tr_2);
					deforming_shape.require_deformation = true;
					if (gui.bool_laplacian_smoothing) deforming_shape.require_smoothing = true;
					gui.constant_velocity_parameters.magnitude = norm(tr_2);
					previous_mouse_position = inputs.mouse.position.current;
				}
			}

			//TO DO : WRITE BETTER
			else if (gui.constant_velocity_parameters.type == deformation_painting_normal || gui.constant_velocity_parameters.type == deformation_painting_inverse) //TO DO: FIX THIS !
			{
				
				picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.base_normal, 0.03f, environment.camera, environment.projection);
				set_tool_in_grid(picking.position, sphere_tool);

				if (norm(sphere_tool.previous_c - sphere_tool.c) > sphere_tool.r0/10) { // we only update every time it makes a cerain distance
					deforming_shape.require_deformation = true;
					if (gui.bool_laplacian_smoothing) deforming_shape.require_smoothing = true;
					sphere_tool.previous_c = sphere_tool.c;
					gui.constant_velocity_parameters.magnitude = 0.001;
				}
			}

			else if (gui.constant_velocity_parameters.type == deformation_painting_normal_bis) //TO DO: FIX THIS !
			{

				picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.shape.normal, 0.03f, environment.camera, environment.projection);
				set_tool_in_grid(picking.position, sphere_tool);

				if (norm(sphere_tool.previous_c - sphere_tool.c) > sphere_tool.r0 / 10) { // we only update every time it makes a cerain distance
					std::cout << "ayoo" << std::endl;
					deforming_shape.require_deformation = true;
					if (gui.bool_laplacian_smoothing) deforming_shape.require_smoothing = true;
					sphere_tool.previous_c = sphere_tool.c;
					gui.constant_velocity_parameters.magnitude = 0.001;
				}
			}
		}
		else {
			picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.shape.normal, 0.03f, environment.camera, environment.projection);
			set_tool_in_grid(picking.position, sphere_tool);
		}
	}

	else
		picking.active = false; // Unselect picking when shift is released
}

void scene_structure::mouse_click(cgp::inputs_interaction_parameters const& inputs) {
	//manage deformations that happen only when we click once

	//TO DO: WRITE BETTER
	if (inputs.keyboard.shift && gui.constant_velocity_parameters.type != direction_mouse_movement && gui.constant_velocity_parameters.type != deformation_painting_normal && gui.constant_velocity_parameters.type != deformation_painting_normal_bis && gui.constant_velocity_parameters.type != deformation_painting_inverse)
	{
		vec2 const& p = inputs.mouse.position.current;
		picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.shape.normal, 0.03f, environment.camera, environment.projection);
		deforming_shape.require_deformation = true;
	}
}

void scene_structure::initialize(){
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.look_at({ 3.0f,2.0f,2.0f }, { 0,0,0 }, { 0,0,1 });
	
	deforming_shape.new_shape();

	// initialize the grid
	int const N = 30;
	grid = initialize_grid(N);
	initialize_grid_segments(grid_segments, grid);
	grid_segments_visual.initialize(grid_segments, "grid");

	initialize_grid_box(grid_box, grid);
	grid_box_visual.initialize(grid_box, "grid_box");

	// initialize the velocity field
	initialize_velocity(N); 
	velocity_grid_data.resize(3 * N * N * N);
	velocity_visual.initialize(velocity_grid_data, "Velocity");
	velocity_visual.color = vec3(1, 0, 0);

	// initialize the tool
	sphere_tool.ri = 0.1f;
	sphere_tool.ci = {1,0.5,0};//?
	sphere_tool.r0 = 0.5f;
	sphere_tool.c0 = { 0,1,0 };//?
	gui.gui_ri = sphere_tool.ri;
	gui.gui_r0 = sphere_tool.r0;
	gui.smoothing_steps = 10;
	inner_sphere_visual.initialize(mesh_primitive_sphere(), "Sphere");
	outer_sphere_visual.initialize(mesh_primitive_sphere(), "Sphere");
	//arrow_visual.initialize(mesh_primitive_arrow(sphere_tool.c, sphere_tool.c+vec3(0.1, 0.1, 0.1), 0.01), "Arrow");//TO DO: USE VELOCITY INSTEAD OF 1, 1, 1

	deforming_shape.update_normal();
	gui.constant_velocity_parameters.type = direction_normal;
	gui.bool_trilinear_interpolation = true;
	gui.bool_display_tool = true;
}

void scene_structure::display(){
	// Basics common elements
	// ***************************************** //

	environment.light = environment.camera.position();
	
	//TO DO: WRITE BETTER
	if ((gui.constant_velocity_parameters.type == deformation_painting_normal)|| (gui.constant_velocity_parameters.type == deformation_painting_normal_bis) || (gui.constant_velocity_parameters.type == deformation_painting_inverse))
		gui.gui_ri = 0;
	
	// update tool position and force ri < ro
	if (gui.gui_r0 < gui.gui_ri) gui.gui_r0 = gui.gui_ri;
	if ((sphere_tool.ri != gui.gui_ri) || (sphere_tool.r0 != gui.gui_r0)) {
		//update the field if the tool paramters change
		update_velocity_field(velocity, grid, sphere_tool, gui.constant_velocity_parameters);
		update_velocity_visual(velocity_visual, velocity_grid_data, velocity, grid, 5);
	}
	sphere_tool.ri = gui.gui_ri;
	sphere_tool.r0 = gui.gui_r0;
	
	previous_interactive_deformation = (gui.constant_velocity_parameters.type == deformation_painting_normal)|| (gui.constant_velocity_parameters.type == deformation_painting_normal_bis) || (gui.constant_velocity_parameters.type == deformation_painting_inverse) || (gui.constant_velocity_parameters.type == direction_mouse_movement);

	// Update velocity field
	if (require_update_velocity){
		// TO DO: CHANGE
		if (gui.constant_velocity_parameters.type != direction_mouse_movement) gui.constant_velocity_parameters.magnitude = 0.01;

		if (gui.constant_velocity_parameters.type == direction_view){
			vec2 const tr_2D = {0,1}; // translation in the screen plane
			vec3 const tr = environment.camera.orientation() * vec3(tr_2D, 0.0f);
			gui.constant_velocity_parameters.dir = tr;
		}

		//rewrite
		//TO DO: WRITE BETTER
		if (gui.constant_velocity_parameters.type == direction_normal ) gui.constant_velocity_parameters.dir = picking.normal;
		if (gui.constant_velocity_parameters.type == direction_inverse_normal ) gui.constant_velocity_parameters.dir = -picking.normal;
		if (gui.constant_velocity_parameters.type == deformation_painting_normal) gui.constant_velocity_parameters.dir = picking.normal;
		if (gui.constant_velocity_parameters.type == deformation_painting_normal_bis) gui.constant_velocity_parameters.dir = picking.normal;
		if (gui.constant_velocity_parameters.type == deformation_painting_inverse) gui.constant_velocity_parameters.dir = -picking.normal;

		update_velocity_field(velocity, grid, sphere_tool, gui.constant_velocity_parameters);
		update_velocity_visual(velocity_visual, velocity_grid_data, velocity, grid, 5);
		require_update_velocity = false;
	}

	// Deform the mesh 
	if (deforming_shape.require_deformation){

		integrate(deforming_shape.shape, deforming_shape.position_saved, velocity, grid, sphere_tool, gui.constant_velocity_parameters, gui.bool_trilinear_interpolation);
		deforming_shape.visual.update_position(deforming_shape.shape.position);
		float scale = 5; // TO DO: SET SCALE AS A SCENE VARIABLE
		update_velocity_visual(velocity_visual, velocity_grid_data, velocity, grid, scale); 
		deforming_shape.position_saved = deforming_shape.shape.position;

		deforming_shape.update_normal();
		previous_tool_pos = sphere_tool.c; // update previous tool position 
		deforming_shape.require_deformation = false;

		if (gui.bool_laplacian_smoothing) {
			std::cout << "yolooo" << std::endl;
			deforming_shape.require_smoothing = true;
		}
	}
	
	// Laplacian smoothing
	if (!previous_laplacian_smoothing && gui.bool_laplacian_smoothing) deforming_shape.require_smoothing = true; //the button laplacian smoothing was just clicked, so we update
	previous_laplacian_smoothing = gui.bool_laplacian_smoothing;
													
	if (deforming_shape.require_smoothing) {
		std::cout << "laplaciaaan" << std::endl;
		for (int i = 0; i < gui.smoothing_steps; i++) {
			deforming_shape.shape = laplacian_smoothing(deforming_shape.shape, deforming_shape.one_ring);
			deforming_shape.visual.update_position(deforming_shape.shape.position);
		}
		deforming_shape.require_smoothing = false;
	}

	//draw different elements
	draw(deforming_shape.visual, environment);

	if (gui.display_wireframe)
		draw_wireframe(deforming_shape.visual, environment, { 0,0,0 });

	display_grid(); // 3D grid
	display_tool(); // deformation tool
	display_velocity(); // vector field

	previous_laplacian_smoothing = gui.bool_laplacian_smoothing;

}

void scene_structure::display_gui(){ // Display the gui and update the new shape if requested

	bool const is_new_surface = gui.display();
	if (is_new_surface)
		deforming_shape.new_shape(gui.surface_type);
}

void scene_structure::mouse_left_released(){
	// Releasing the left click indicates the end of the deformation: disable the picking, save the new position and update the normals
	picking.active = false;
	deforming_shape.require_deformation = false;
}

//USELESS
//void scene_structure::mouse_scroll(float scroll_offset){
//	//should be useless now -> already used for camera zoom
//}

void deforming_shape_structure::new_shape(surface_type_enum type_of_surface){
	// Create a new shape based on the gui indication type_of_surface
	// The detail of the initialization functions are in the file initialization.cpp
	switch (type_of_surface)
	{
	case surface_plane:
		shape = initialize_plane();
		break;
	case surface_cylinder:
		shape = initialize_cylinder();
		break;
	case surface_sphere:
		shape = initialize_sphere();
		break;
	case surface_cube:
		shape = initialize_cube();
		break;
	case surface_head:
		shape = initialize_mesh();
		break;
	case surface_camel:
		shape = initialize_mesh_2();
		break;
	case surface_armadillo:
		shape = initialize_mesh_3();
		break;
	case surface_tyrannosaur:
		shape = initialize_mesh_4();
		break;
	case surface_spoon:
		shape = initialize_mesh_5();
		break;
	case surface_bunny:
		shape = initialize_mesh_6();
		break;
	case surface_body:
		shape = initialize_mesh_7();
		break;
		// TO DO: only use one function initialize_mesh with a filename argument
	}

	// Clear previous surface before seting the values of the new one
	visual.clear();
	visual.initialize(shape, "Shape");
	std::cout << "Original shape volume: " << mesh_volume(shape) << std::endl;
	one_ring = connectivity_one_ring(shape.connectivity); // to do: extract & store so it doesn't get recomputed each time

	position_saved = shape.position;
	require_normal_update = false;

	base_normal = shape.normal;
}

void deforming_shape_structure::update_normal(){
	shape.compute_normal();
	visual.update_normal(shape.normal);
	require_normal_update = false;
}

void scene_structure::initialize_velocity(int N){//move to initialize.cpp
	velocity.resize(N, N, N); 
	velocity.fill({ 0, 0, 0 }); 
}


void scene_structure::display_grid(){
	if (gui.display_grid_edge)
		draw(grid_segments_visual, environment);

	if (gui.display_grid_box) {
		grid_box_visual.color = vec3(1, 0, 0);
		draw(grid_box_visual, environment);
	}
}

void scene_structure::display_velocity(){
	if (gui.display_constant_velocity)
		draw(velocity_visual, environment);
}

void scene_structure::display_tool(){
	if (gui.bool_display_tool) {
		inner_sphere_visual.shading.color = sphere_tool.ci;
		inner_sphere_visual.shading.alpha = 0.8;
		inner_sphere_visual.transform.translation = sphere_tool.c;
		inner_sphere_visual.transform.scaling = sphere_tool.ri;

		outer_sphere_visual.shading.color = sphere_tool.c0;
		outer_sphere_visual.shading.alpha = 0.1;
		outer_sphere_visual.transform.translation = sphere_tool.c;
		outer_sphere_visual.transform.scaling = sphere_tool.r0;

		glEnable(GL_BLEND); // Color Blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(false); // do not write on depth buffer

			//Draw all transparent objects from furthest to closest

		if (norm(gui.constant_velocity_parameters.dir) >= 0.1) {
			arrow_visual.clear();
			arrow_visual.initialize(mesh_primitive_arrow(sphere_tool.c, sphere_tool.c + gui.constant_velocity_parameters.dir / 2.5, 0.01), "Arrow");
			arrow_visual.shading.color = sphere_tool.ci;
			//inner_sphere_visual.shading.alpha = 0.9;
			draw(arrow_visual, environment);
		}
		draw(inner_sphere_visual, environment);
		draw(outer_sphere_visual, environment);

		glDisable(GL_BLEND);
		// do not forget to re-activate depth buffer write
		glDepthMask(true);

	}
	
}

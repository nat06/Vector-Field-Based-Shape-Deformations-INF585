#include "scene.hpp"


using namespace cgp;

void scene_structure::mouse_move(cgp::inputs_interaction_parameters const& inputs)//TO DO: function description
{
	// Current position of the mouse
	vec2 const& p = inputs.mouse.position.current;

	require_update_velocity = true;

	if (inputs.keyboard.shift)
	{
		
		// If the mouse is not clicked, compute a picking on the vertices of the mesh
		if (!inputs.mouse.click.left)
			picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.shape.normal, 0.03f, environment.camera, environment.projection);

		// Apply Deformation: press on shift key + left click on the mouse when a vertex is already selected
		if (inputs.mouse.click.left && picking.active && gui.constant_velocity_parameters.type == direction_mouse_movement) {

			//move the tool on the screen plane

			if (!previous_interactive_deformation) previous_mouse_position = inputs.mouse.position.current;// this just started!

			vec2 const tr_2D = inputs.mouse.position.current - inputs.mouse.position.previous; // translation in the screen plane
			vec3 const tr = environment.camera.orientation() * vec3(tr_2D, 0.0f); 
			vec3 new_pos = sphere_tool.c + tr; 

			//TO DO: WRTIE THIS BETTER !
			//make a function force_tool_in_grid
			if (new_pos.x + sphere_tool.r0 >= 1.0f) new_pos.x = 1.0 - sphere_tool.r0;
			if (new_pos.x - sphere_tool.r0 <= -1.0f) new_pos.x = -1.0 + sphere_tool.r0;
			if (new_pos.y + sphere_tool.r0 >= 1.0f) new_pos.y = 1.0 - sphere_tool.r0;
			if (new_pos.y - sphere_tool.r0 <= -1.0f) new_pos.y = -1.0 + sphere_tool.r0;
			if (new_pos.z + sphere_tool.r0 >= 1.0f) new_pos.z = 1.0 - sphere_tool.r0;
			if (new_pos.z - sphere_tool.r0 <= -1.0f) new_pos.z = -1.0 + sphere_tool.r0;

			sphere_tool.c = new_pos; //the tool is moved in the screen space

			//we compute the velocity field and deform only when the mouse has moved a certain amount
			vec2 const tr_2D_2 = inputs.mouse.position.current - inputs.mouse.position.previous; 
			if (norm(tr_2D_2) > 0.001) { // we only update  evrytime it makes a cerain distance
				vec3 const tr_2 = environment.camera.orientation() * vec3(tr_2D_2, 0.0f);
				gui.constant_velocity_parameters.dir = normalize(tr_2);
				deforming_shape.require_deformation = true;
				if (gui.laplacian_smoothing) deforming_shape.require_smoothing = true;
				previous_mouse_position = inputs.mouse.position.current;
			}
		}
		else {
			
			picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.shape.normal, 0.03f, environment.camera, environment.projection);

			//TO DO: -> adapt 2.0 to grid size
			// write this better !!!! +  make a function
			//stay in its place when outside the grid
			if (picking.position.x + sphere_tool.r0 >= 1.0f) picking.position.x = 1.0 - sphere_tool.r0;
			if (picking.position.x - sphere_tool.r0 <= -1.0f) picking.position.x = -1.0 + sphere_tool.r0;
			if (picking.position.y + sphere_tool.r0 >= 1.0f) picking.position.y = 1.0 - sphere_tool.r0;
			if (picking.position.y - sphere_tool.r0 <= -1.0f) picking.position.y = -1.0 + sphere_tool.r0;
			if (picking.position.z + sphere_tool.r0 >= 1.0f) picking.position.z = 1.0 - sphere_tool.r0;
			if (picking.position.z - sphere_tool.r0 <= -1.0f) picking.position.z = -1.0 + sphere_tool.r0;

			sphere_tool.c = picking.position; //the tool is moved on the shape surface
		}
	}

	else
		picking.active = false; // Unselect picking when shift is released
}

void scene_structure::mouse_click(cgp::inputs_interaction_parameters const& inputs) {
	//manage deformations that happen only when we click once
	if (inputs.keyboard.shift && gui.constant_velocity_parameters.type != direction_mouse_movement)
	{
		vec2 const& p = inputs.mouse.position.current;
		picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.shape.normal, 0.03f, environment.camera, environment.projection);
		deforming_shape.require_deformation = true;
	}
}


void scene_structure::initialize()//TO DO: CLEAN THIS ONE
{
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.look_at({ 3.0f,2.0f,2.0f }, { 0,0,0 }, { 0,0,1 });
	
	//timer_update_normal.event_period = 0.15f;
	deforming_shape.new_shape();
	picking_visual.initialize();

	// GRID
	int const N = 20;
	grid = initialize_grid(N);
	initialize_grid_segments(grid_segments, grid);
	grid_segments_visual.initialize(grid_segments, "grid");

	initialize_grid_box(grid_box, grid);
	grid_box_visual.initialize(grid_box, "grid_box");

	//VELOCITY //TO DO: -> to clean
	initialize_velocity(N); // useless ?
	velocity_grid_data.resize(3 * N * N * N);
	velocity_visual.initialize(velocity_grid_data, "Velocity");
	velocity_visual.color = vec3(1, 0, 0);

	// TOOL
	//-> TO DO: might want to create a functio  for this
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
}
	


void scene_structure::display()
{
	// Basics common elements
	// ***************************************** //

	environment.light = environment.camera.position();
	
	//update tool postion and force ri < ro
	if (gui.gui_r0 < gui.gui_ri) gui.gui_r0 = gui.gui_ri;
	sphere_tool.ri = gui.gui_ri;
	sphere_tool.r0 = gui.gui_r0;

	float scale = 5; //wtf ?

	// Update velocity field
	if (require_update_velocity) {
		//TO DO: CHANGE
		if (gui.constant_velocity_parameters.type == direction_normal) gui.constant_velocity_parameters.dir = picking.normal;
		if (gui.constant_velocity_parameters.type == direction_inverse_normal) gui.constant_velocity_parameters.dir = -picking.normal;
		update_velocity_field(velocity, grid, sphere_tool, gui.constant_velocity_parameters);
		update_velocity_visual(velocity_visual, velocity_grid_data, velocity, grid, 5);
		require_update_velocity = false;
	}

	// Deform the mesh 
	if (deforming_shape.require_deformation) {
		//update_velocity_field(velocity, grid, sphere_tool, previous_tool_pos, previous_tool_pos);
		integrate(deforming_shape.shape, deforming_shape.position_saved, velocity, grid, sphere_tool, gui.constant_velocity_parameters, gui.bool_trilinear_interpolation);
		deforming_shape.visual.update_position(deforming_shape.shape.position);
		float scale = 5; //TO DO: SET SCALE AS A SCENE VARIABLE
		update_velocity_visual(velocity_visual, velocity_grid_data, velocity, grid, scale); //HERE ???
		deforming_shape.position_saved = deforming_shape.shape.position;
		deforming_shape.update_normal();
		previous_tool_pos = sphere_tool.c;//update previous tool position //make sure this is useful
		deforming_shape.require_deformation = false;

		if (gui.laplacian_smoothing) deforming_shape.require_smoothing = true;
	}
	
	// Laplacian smoothing: TO DO: FIX THIS
	if (!previous_laplacian_smoothing && gui.laplacian_smoothing) deforming_shape.require_smoothing = true; //the button laplacian smoothing was just clicked, so we update
	previous_laplacian_smoothing = gui.laplacian_smoothing;
													
	if (deforming_shape.require_smoothing) {
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

	display_grid(); //3D grid
	display_tool(); 
	display_velocity(); //vector field

}


void scene_structure::display_gui()
{// Display the gui and update the new shape if requested

	bool const is_new_surface = gui.display();
	if (is_new_surface)
		deforming_shape.new_shape(gui.surface_type);
}

void scene_structure::mouse_left_released()
{
	// Releasing the left click indicates the end of the deformation: disable the picking, save the new position and update the normals
	picking.active = false;
	/*deforming_shape.position_saved = deforming_shape.shape.position;
	deforming_shape.update_normal();*/
	deforming_shape.require_deformation = false;
}

void scene_structure::mouse_scroll(float scroll_offset)
{
	//should be useless now -> already used for camera zoom
}


void deforming_shape_structure::new_shape(surface_type_enum type_of_surface)
{
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
	case surface_mesh:
		shape = initialize_mesh();
		break;
	case surface_mesh_2:
		shape = initialize_mesh_2();
		break;
	case surface_mesh_3:
		shape = initialize_mesh_3();
		break;
	//TO DO: only use one function initialize_mesh with a filename argument
	}

	// Clear previous surface before seting the values of the new one
	visual.clear();
	visual.initialize(shape, "Shape");

	one_ring = connectivity_one_ring(shape.connectivity); // to do: extract & store so it doesn't get recomputed each time

	position_saved = shape.position;
	require_normal_update = false;
}

void deforming_shape_structure::update_normal()
{
	shape.compute_normal();
	visual.update_normal(shape.normal);
	require_normal_update = false;
}



void scene_structure::initialize_velocity(int N){//not sure about this
	//TO DO: MOVE THIS FUNCTION TO INITIALIZE ?
	velocity.resize(N, N, N); 
	velocity.fill({ 0, 0, 0 }); 
}



//TO DO: MOVE THESE TO A FILE DISPLAY.CPP
void scene_structure::display_grid()
{
	if (gui.display_grid_edge)
		draw(grid_segments_visual, environment);

	if (gui.display_grid_box)
		grid_box_visual.color = vec3( 1, 0, 0 );
		draw(grid_box_visual, environment);
}


void scene_structure::display_velocity()
{
	if (gui.display_constant_velocity)
		draw(velocity_visual, environment);
}

void scene_structure::display_tool() 
{
	inner_sphere_visual.shading.color = sphere_tool.ci;
	inner_sphere_visual.shading.alpha = 0.9;
	inner_sphere_visual.transform.translation = sphere_tool.c;
	inner_sphere_visual.transform.scaling = sphere_tool.ri;

	outer_sphere_visual.shading.color = sphere_tool.c0;
	outer_sphere_visual.shading.alpha = 0.2;
	outer_sphere_visual.transform.translation = sphere_tool.c;
	outer_sphere_visual.transform.scaling = sphere_tool.r0;

	glEnable(GL_BLEND); // Color Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false); // do not write on depth buffer

		//Draw all transparent objects from furthest to closest
		draw(inner_sphere_visual, environment);
		draw(outer_sphere_visual, environment);

		glDisable(GL_BLEND);
	// do not forget to re-activate depth buffer write
	glDepthMask(true);

	display_arrow();
}

void scene_structure::display_arrow(){ //TO DO: JUST PUT THIS IN THE TOOL FUNCTION
	//display velocity vector in the center of the tool as a 3d arrow

	arrow_visual.clear();
	//if (norm(constant_vel) >= 0.1) {
	if (norm(gui.constant_velocity_parameters.dir) >= 0.1) {
		
		//arrow_visual.initialize(mesh_primitive_arrow(sphere_tool.c, sphere_tool.c + constant_vel / 3, 0.01), "Arrow");
		arrow_visual.initialize(mesh_primitive_arrow(sphere_tool.c, sphere_tool.c + gui.constant_velocity_parameters.dir / 3, 0.01), "Arrow");
		//std::cout << gui.constant_velocity_parameters.type << std::endl;

		//TO DO: use rotations instead of always creating a new object

		/*if (picking.normal.x != 0 && picking.normal.y != 0 && picking.normal.z != 0 && prev_direction.x!=0 && prev_direction.y != 0 && prev_direction.z != 0)
			arrow_visual.transform.rotation = rotation_transform().between_vector(prev_direction, picking.normal);*/

		arrow_visual.shading.color = sphere_tool.ci;
		inner_sphere_visual.shading.alpha = 0.9;

		glEnable(GL_BLEND); // Color Blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(false); 
			draw(arrow_visual, environment);
		glDisable(GL_BLEND);
		// do not forget to re-activate depth buffer write
		glDepthMask(true);

	}

}

//probably possible to use a function from cgp instead !




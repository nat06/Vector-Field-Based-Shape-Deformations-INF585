#include "scene.hpp"


using namespace cgp;


// The deformation is applied when the user move the mouse
void scene_structure::mouse_move(cgp::inputs_interaction_parameters const& inputs)
{
	// Current position of the mouse
	vec2 const& p = inputs.mouse.position.current;

	//##############################
	//move the tool on the shape surface
	picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.shape.normal, 0.03f, environment.camera, environment.projection);

	//TO DO: -> adapt 2.0 to grid size
	// write this better !!!!
	//stay in its place when outside the grid
	if (picking.position.x + sphere_tool.r0 >= 1.0f) picking.position.x = 1.0- sphere_tool.r0;
	if (picking.position.x - sphere_tool.r0 <= -1.0f) picking.position.x = -1.0 + sphere_tool.r0;
	if (picking.position.y + sphere_tool.r0 >= 1.0f) picking.position.y = 1.0 - sphere_tool.r0;
	if (picking.position.y - sphere_tool.r0 <= -1.0f) picking.position.y = -1.0 + sphere_tool.r0;
	if (picking.position.z + sphere_tool.r0 >= 1.0f) picking.position.z = 1.0 - sphere_tool.r0;
	if (picking.position.z - sphere_tool.r0 <= -1.0f) picking.position.z = -1.0 + sphere_tool.r0;

	sphere_tool.c = picking.position;
	
	//##############################

	// The picking and deformation is only applied when pressing the shift key
	if (inputs.keyboard.shift)
	{
		// If the mouse is not clicked, compute a picking on the vertices of the mesh
		if (!inputs.mouse.click.left)
			picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.shape.normal, 0.03f, environment.camera, environment.projection);


		// Apply Deformation: press on shift key + left click on the mouse when a vertex is already selected
		if (inputs.mouse.click.left && picking.active) {

			// Current translation in 2D window coordinates
			//vec2 const translation_screen = p - picking.screen_clicked;

			//// Apply the deformation on the surface
			//apply_deformation(deforming_shape.shape, deforming_shape.position_saved, translation_screen, picking.position, picking.normal, environment.camera.orientation(), gui.deformer_parameters);

			//// Update the visual model
			//deforming_shape.visual.update_position(deforming_shape.shape.position);
			//deforming_shape.require_normal_update = true;

		}

	}

	else
		picking.active = false; // Unselect picking when shift is released
}

//##############################################################
void scene_structure::mouse_click(cgp::inputs_interaction_parameters const& inputs) {

	//TO DO: REMOVE THE PICKING POSITION FROM THE FUNCTIONS (USELESS NOW)

	if (inputs.keyboard.shift)//the deformation is only applied when shift + click
	{
		vec2 const& p = inputs.mouse.position.current;
		picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.shape.normal, 0.03f, environment.camera, environment.projection);
		
		integrate(deforming_shape.shape, deforming_shape.position_saved, picking.position, gui.deformer_parameters, velocity, grid, sphere_tool);

		deforming_shape.visual.update_position(deforming_shape.shape.position);
	}

}
//##############################################################



void scene_structure::initialize()
{
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.look_at({ 3.0f,2.0f,2.0f }, { 0,0,0 }, { 0,0,1 });
	
	timer_update_normal.event_period = 0.15f;
	deforming_shape.new_shape();
	picking_visual.initialize();

	//################################################################
	//PROJECT

	// Initialization of the grid
	// int const Nx = 20, Ny = 20, Nz = 20;
	int const N = 20; // grid size (cubic grid)

    grid = initialize_grid(N);

	////////
	/*
	std::cout << "grid(0,0,0) : " << grid(0, 0, 0) << std::endl;
	std::cout << "grid(0,1,0) : " << grid(0, 1, 0) << std::endl;
	std::cout << "grid(0,2,0) : " << grid(0, 2, 0) << std::endl;
	std::cout << "grid(0,3,0) : " << grid(0, 3, 0) << std::endl;
	std::cout << "grid(0,4,0) : " << grid(0, 4, 0) << std::endl;
	std::cout << "grid(0,5,0) : " << grid(0, 5, 0) << std::endl;
	std::cout << "grid(0,6,0) : " << grid(0, 6, 0) << std::endl;
	std::cout << "grid(0,7,0) : " << grid(0, 7, 0) << std::endl;
	std::cout << "grid(0,8,0) : " << grid(0, 8, 0) << std::endl;
	std::cout << "grid(0,9,0) : " << grid(0, 9, 0) << std::endl;
	std::cout << "grid(0,10,0) : " << grid(0, 10, 0) << std::endl;
	std::cout << "grid(0,11,0) : " << grid(0, 11, 0) << std::endl;
	std::cout << "grid(0,12,0) : " << grid(0, 12, 0) << std::endl;
	std::cout << "grid(0,13,0) : " << grid(0, 13, 0) << std::endl;
	std::cout << "grid(0,14,0) : " << grid(0, 14, 0) << std::endl;
	std::cout << "grid(0,15,0) : " << grid(0, 15, 0) << std::endl;
	std::cout << "grid(0,16,0) : " << grid(0, 16, 0) << std::endl;
	std::cout << "grid(0,17,0) : " << grid(0, 17, 0) << std::endl;
	std::cout << "grid(0,18,0) : " << grid(0, 18, 0) << std::endl;
	std::cout << "grid(0,19,0) : " << grid(0, 19, 0) << std::endl;

	vec3 pointTest = {0.5, 0.5, 0.5};
	vec3 temp = get_cell(pointTest, Nx);
	std::cout << "pointToGridCell({" << pointTest << "} = " << temp << std::endl;
	std::cout << "grid(temp.x, temp.y, temp.z) --> " << grid(temp.x, temp.y, temp.z) << std::endl;
	*/
	vec3 pointTest = {-2, 0.5, 0.5};
	vec3 temp = get_cell(pointTest, N);
	std::cout << "get_cell({" << pointTest << "} = " << temp << std::endl;
	std::cout << "--------" << std::endl;
    laplacian_smoothing(deforming_shape.shape, grid);
	////////

	update_grid_segments(grid_segments, grid);
	grid_segments_visual.initialize(grid_segments, "grid");

	initialize_velocity(N); // useless ?
	velocity_grid_data.resize(3 * N * N * N);
	velocity_visual.initialize(velocity_grid_data, "Velocity");
	velocity_visual.color = vec3(1, 0, 0);

	//initialize the tool
	//-> might want to create a function for this
	
	sphere_tool.c = { 0.5,0.5,0.5 };//TO DO: use mouse position (to do later)
	sphere_tool.ri = 0.1f;
	sphere_tool.ci = {1,0.5,0};//?
	sphere_tool.r0 = 0.5f;
	sphere_tool.c0 = { 0,1,0 };//?

	gui.gui_ri = sphere_tool.ri;
	gui.gui_r0 = sphere_tool.r0;
	
	inner_sphere_visual.initialize(mesh_primitive_sphere(), "Sphere");
	outer_sphere_visual.initialize(mesh_primitive_sphere(), "Sphere");

	update_velocity_field(velocity, grid, sphere_tool); //not sure this goes there...

	//################################################################

}


void scene_structure::display()
{
	// Basics common elements
	// ***************************************** //

	environment.light = environment.camera.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	// Display the deformed shape
	draw(deforming_shape.visual, environment);
	if (gui.display_wireframe)
		draw_wireframe(deforming_shape.visual, environment, { 0,0,0 });

	// Periodically update the normal
	//  Doesn't do it all the time as the computation is costly
	timer_update_normal.update();
	if (timer_update_normal.event && deforming_shape.require_normal_update == true)
		deforming_shape.update_normal();
	
	// Display of the circle of influence oriented along the local normal of the surface
	if (picking.active)
		picking_visual.draw(environment, deforming_shape.shape.position[picking.index], picking.normal, gui.deformer_parameters.falloff);

	//###############################################
	//PROJECT

	//update the tool -> might want to create a function for this
	if (gui.gui_r0 < gui.gui_ri) {
		gui.gui_r0 = gui.gui_ri;
	}
	sphere_tool.ri = gui.gui_ri;
	sphere_tool.r0 = gui.gui_r0;

	//update_velocity_field(grid, sphere_tool); //not sure this goes there...
	float scale = 0.000001;
	scale = 5;
	update_velocity_field(velocity, grid, sphere_tool);
	update_velocity_visual(velocity_visual, velocity_grid_data, velocity, grid, scale); 

	
	display_grid(); //3D grid
	display_velocity(); //vector field
	display_tool(); //sphere tool (to be displayed last)
	display_velocity(); //vector field

	//###############################################
	
}


void scene_structure::display_gui()
{
	// Display the gui and update the new shape if requested
	bool const is_new_surface = gui.display();
	if (is_new_surface)
		deforming_shape.new_shape(gui.surface_type);
}

void scene_structure::mouse_left_released()
{
	// Releasing the left click indicates the end of the deformation: disable the picking, save the new position and update the normals
	picking.active = false;
	deforming_shape.position_saved = deforming_shape.shape.position;
	deforming_shape.update_normal();
}

void scene_structure::mouse_scroll(float scroll_offset)
{
	// Increase/decrease falloff distance when scrolling the mouse
	if (picking.active)
		gui.deformer_parameters.falloff = std::max(gui.deformer_parameters.falloff + float(scroll_offset) * 0.01f, 1e-6f);
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
	}

	// Clear previous surface before seting the values of the new one
	visual.clear();
	visual.initialize(shape, "Shape");

	position_saved = shape.position;
	require_normal_update = false;
}

void deforming_shape_structure::update_normal()
{
	shape.compute_normal();
	visual.update_normal(shape.normal);
	require_normal_update = false;
}


//#####################################################################################
//PROJECT

void scene_structure::initialize_velocity(int N){//not sure about this
	//TO DO: MOVE THIS FUNCTION TO INITIALIZE ?

	velocity.resize(N, N, N); 
	velocity.fill({ 1, 1, 1 }); 
	velocity_previous = velocity;
}


void scene_structure::display_grid()
{
	if (gui.display_grid_edge)
		draw(grid_segments_visual, environment);
}


void scene_structure::display_velocity()
{
	if (gui.display_velocity)
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
	//... Sort transparent objects by depth ...
		//... Draw all transparent objects from furthest to closest ...
		draw(inner_sphere_visual, environment);
		draw(outer_sphere_visual, environment);

		glDisable(GL_BLEND);
	// do not forget to re-activate depth buffer write
	glDepthMask(true);

	//TO DO: DISPLAY THE CONSTANT VELOCITY ARROW
	
}

bool areVec3vectorsSame(const cgp::vec3 &v1, const cgp::vec3 &v2){
	if (v1.size() != v2.size()){return false;}
	for (int i=0; i<v1.size(); i++){
		if (v1(i) != v2(i)){return false;}
	}
	return true;
}


//#####################################################################################


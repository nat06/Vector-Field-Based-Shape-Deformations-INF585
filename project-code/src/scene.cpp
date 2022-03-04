#include "scene.hpp"


using namespace cgp;


// The deformation is applied when the user move the mouse
void scene_structure::mouse_move(cgp::inputs_interaction_parameters const& inputs)
{
	// Current position of the mouse
	vec2 const& p = inputs.mouse.position.current;

	// The picking and deformation is only applied when pressing the shift key
	if (inputs.keyboard.shift)
	{
		// If the mouse is not clicked, compute a picking on the vertices of the mesh
		if (!inputs.mouse.click.left)
			picking = picking_mesh_vertex_as_sphere(p, deforming_shape.shape.position, deforming_shape.shape.normal, 0.03f, environment.camera, environment.projection);


		// Apply Deformation: press on shift key + left click on the mouse when a vertex is already selected
		if (inputs.mouse.click.left && picking.active) {

			// Current translation in 2D window coordinates
			vec2 const translation_screen = p - picking.screen_clicked;

			// Apply the deformation on the surface
			apply_deformation(deforming_shape.shape, deforming_shape.position_saved, translation_screen, picking.position, picking.normal, environment.camera.orientation(), gui.deformer_parameters);

			// Update the visual model
			deforming_shape.visual.update_position(deforming_shape.shape.position);
			deforming_shape.require_normal_update = true;
		}

	}
	else
		picking.active = false; // Unselect picking when shift is released
}



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
	int const Nx = 20, Ny = 20, Nz = 20;

	grid = initialize_grid(Nx, Ny, Nz);

    // ######### some tests... #########
//    std::cout << grid << std::endl;
    std::cout << "grid(0,0,0) : " << grid(0,0,0) << std::endl;
    std::cout << "grid(0,1,0) : " << grid(0,1,0) << std::endl;
    std::cout << "grid(0,2,0) : " << grid(0,2,0) << std::endl;
    std::cout << "grid(0,3,0) : " << grid(0,3,0) << std::endl;
    std::cout << "grid(0,4,0) : " << grid(0,4,0) << std::endl;
    std::cout << "grid(0,5,0) : " << grid(0,5,0) << std::endl;
    std::cout << "grid(0,6,0) : " << grid(0,6,0) << std::endl;
    std::cout << "grid(0,7,0) : " << grid(0,7,0) << std::endl;
    std::cout << "grid(0,8,0) : " << grid(0,8,0) << std::endl;
    std::cout << "grid(0,9,0) : " << grid(0,9,0) << std::endl;
    std::cout << "grid(0,10,0) : " << grid(0,10,0) << std::endl;
    std::cout << "grid(0,11,0) : " << grid(0,11,0) << std::endl;
    std::cout << "grid(0,12,0) : " << grid(0,12,0) << std::endl;
    std::cout << "grid(0,13,0) : " << grid(0,13,0) << std::endl;
    vec3 temp = pointToGridCell({-0.6f,-0.6f,1.0f}, Nx);
    std::cout << "grid(7,0,0) : " << grid(1,0,0) << std::endl;
    std::cout << temp << std::endl;
    // #################################

	update_grid_segments(grid_segments, grid);
	grid_segments_visual.initialize(grid_segments, "grid");

	initialize_velocity(Nx, Ny, Nz); // useless ?
	velocity_grid_data.resize(3 * Nx * Ny * Nz);
	velocity_visual.initialize(velocity_grid_data, "Velocity");
    velocity_visual.color = vec3(1, 0, 0);

	//initialize the tool
	//-> might want to create a function  for this
	
	sphere_tool.c = { 0.5,0.5,0.5 };//TO DO: use mouse position (to do later)
	sphere_tool.ri = 0.1f;
	sphere_tool.ci = {1,0.5,0};//?
	sphere_tool.r0 = 0.5f;
	sphere_tool.c0 = { 0,1,0 };//?
	
	inner_sphere_visual.initialize(mesh_primitive_sphere(), "Sphere");
	outer_sphere_visual.initialize(mesh_primitive_sphere(), "Sphere");

    mini_testing_sphere.initialize(mesh_primitive_sphere(), "Sphere");

	update_velocity_field(velocity, grid, sphere_tool); //not sure this goes there...

	
	//################################################################

}

//################# PROJECT ########################
vec3 pointToGridCell(const vec3& p, int N){
// function that converts from a point in space to the associated 3D grid cell it belongs to.
// i.e. { x,y,z } -> { kx, ky, kz }
// ! function returns the *lower bound* of the cell (i.e. the smallest of the 2 points that define the lower edge of a given cell along its axis)
// https://math.stackexchange.com/questions/3135977/which-cell-in-a-grid-a-point-belongs-to#comment6460585_3136016
    float gridCellSize = 2.0f / (N-1);
    int sign_x; int sign_y; int sign_z; int index_x; int index_y; int index_z;
    if (p.x >= 0){sign_x = 1.;} else{sign_x = -1.;}; if (p.y >= 0){sign_y = 1.;} else{sign_y = -1.;}; if (p.z >= 0){sign_z = 1.;} else{sign_z = -1.;}
    float px_shifted = p.x - sign_x*gridCellSize/2.0; float py_shifted = p.y - sign_y*gridCellSize/2.0; float pz_shifted = p.z - sign_z*gridCellSize/2.0;

    // x
    if (std::abs(p.x) < (gridCellSize/2.0)){
        index_x = N / 2 - 1;
    }
    else{ // use shifted values
        if (sign_x == 1.){
            index_x = int(px_shifted / gridCellSize) + N / 2;
        }
        else{
            index_x = int(px_shifted / gridCellSize) + N / 2 - 2;
        }
    }
    // y
    if (std::abs(p.y) < (gridCellSize/2.0)){
        index_y = N / 2 - 1;
    }
    else{ // use shifted values
        if (sign_y == 1.){
            index_y = int(py_shifted / gridCellSize) + N / 2;
        }
        else{
            index_y = int(py_shifted / gridCellSize) + N / 2 - 2;
        }
    }
    // z
    if (std::abs(p.z) < (gridCellSize/2.0)){
        index_z = N / 2 - 1;
    }
    else{ // use shifted values
        if (sign_z == 1.){
            index_z = int(pz_shifted / gridCellSize) + N / 2;
        }
        else{
            index_z = int(pz_shifted / gridCellSize) + N / 2 - 2;
        }
    }
    return {index_x, index_y, index_z};
}

//################################################################

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

	//update_velocity_field(grid, sphere_tool); //not sure this goes there...
	float scale = 0.000001;
	scale = 5;
	update_velocity_visual(velocity_visual, velocity_grid_data, velocity, grid, scale); // what the fuck is this error

	
	display_grid(); //3D grid
	//display_velocity(); //vector field
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

void scene_structure::initialize_velocity(int Nx, int Ny, int Nz){//not sure about this
	//TO DO: MOVE THIS FUNCTION TO INITIALIZE ?

	velocity.resize(Nx, Ny, Nz); 
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

    mini_testing_sphere.shading.color = { 1,0,0 };
    mini_testing_sphere.shading.alpha = 0.6;
    mini_testing_sphere.transform.translation = { -1,-1,-1 };
    mini_testing_sphere.transform.scaling = 0.025f;

	glEnable(GL_BLEND); // Color Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false); // do not write on depth buffer
	//... Sort transparent objects by depth ...
		//... Draw all transparent objects from furthest to closest ...
		draw(inner_sphere_visual, environment);
        draw(outer_sphere_visual, environment);

        draw(mini_testing_sphere, environment);

		glDisable(GL_BLEND);
	// do not forget to re-activate depth buffer write
	glDepthMask(true);
	
}




//#####################################################################################


#include "deformers.hpp"
#include "scene.hpp"
// #include "../helpers/gui.hpp"
using namespace cgp;
#include <typeinfo>

void apply_deformation(mesh& shape, buffer<vec3> const& position_before_deformation, vec2 const& translate_screen, vec3 const& picked_position, vec3 const& picked_normal, rotation_transform const& camera_orientation, deformer_parameters_structure const& deformer_parameters)
{

	/** 
		shape:  The position of shape are the one to be deformed
		position_before_deformation:  Reference position of the mesh before applying this deformation
		translate_screen:   Input translation of the user in the 2D-screen coordinates - tr must be converted into a 3D transformation applied to the positions of shape
		picked_position:    3D Position of the picked vertex
		picked_normal:      Normal of the surface at the picked vertex position
		camera_orientation: Current camera orientation - allows to convert the 2D-screen coordinates into 3D coordinates
	*/


	float const r = deformer_parameters.falloff; // radius of influence of the deformation
	size_t const N = shape.position.size();
	for (size_t k = 0; k < N; ++k)
	{
		vec3& p_shape = shape.position[k];                             // position to deform
		vec3 const& p_shape_original = position_before_deformation[k]; // reference position before deformation
		vec3 const& p_clicked = picked_position;                       // 3D position of picked position
		vec3 const& n_clicked = picked_normal;                         // normal of the surface (before deformation) at the picked position

		float const dist = norm(p_clicked - p_shape_original);         // distance between the picked position and the vertex before deformation

		// TO DO: Implement the deformation models
		// **************************************************************** //
		// ...
		if (deformer_parameters.type == deform_translate) // Case of translation
		{
			// Hint: You can convert the 2D translation in screen space into a 3D translation in the view plane in multiplying 
			//       camera_orientation * vec3(translate_screen, 0)
			vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);

			// Fake deformation (linear translation in the screen space) 
			//   the following lines should be modified to get the expected smooth deformation
			if (dist < r)
				p_shape = p_shape_original + (1 - dist / r) * translation;

		}
		if (deformer_parameters.type == deform_twist)
		{
			// Deformation to implement
		}
		if (deformer_parameters.type == deform_scale)
		{
			// Deformation to implement"
		}

	}


}

//idea:
//void numerical_integration(grid_2<vec3>& position, grid_2D<vec3>& velocity, grid_2D<vec3> const& force, float mass, float dt)
//{
//	size_t const N = position.size();
//
//	for (size_t k = 0; k < N; ++k)
//	{
//		velocity[k] = velocity[k] + dt * force[k] / mass;
//		position[k] = position[k] + dt * velocity[k];
//	}
//
//}

void integrate(mesh &shape, buffer<vec3> const &position_before_deformation, vec3 const &picked_position, deformer_parameters_structure const &deformer_parameters, grid_3D<vec3> &velocity, grid_3D<vec3> const &grid, sphere_tool_structure const &sphere_tool, buffer<buffer<int> > one_ring)
{
	//TO DO: find out where ad how this is called ???

	//fins some inspiration from 08_cloth main.cpp -> understand the code and adapt it !
	
	size_t const N = shape.position.size();
	vec3 ref = {-100, -100, -100}; // to check if point is in [-1, 1] grid 
	//intergation steps
	size_t const N_substeps = 10;
	for (size_t k_substep = 0; k_substep < N_substeps; ++k_substep) {

		for (size_t k = 0; k < N; ++k)
		{
			vec3& p_shape = shape.position[k];                             // position to deform
			vec3 const& p_shape_original = position_before_deformation[k]; // reference position before deformation

			//get the grid_cell corresponding to the point
			vec3 cell = get_cell(p_shape, velocity.dimension[0]);
			if (!areVec3vectorsSame(cell, ref)){ // if point is in grid

				// int kx, ky, kz;
				// kx = cell[0];
				// ky = cell[1];
				// kz = cell[2];

				//apply path line integration of v starting from p_shape
				//p_shape = p_shape + dt * velocity(kx, ky, kz);

				//WHAT DO WE DO WITH DT HERE ????
				//float const dt = 0.005f * timer.scale;

				float const dt = 0.005f;

				// size_t const N_substeps = 10;
				// for (size_t k_substep = 0; k_substep < N_substeps; ++k_substep) {
				// p_shape = p_shape + dt * get_interpolated_velocity(p_shape, velocity, velocity.dimension[0]);
				// p_shape = p_shape + dt * velocity(kx, ky, kz);
				p_shape = p_shape + dt * trilinear_interpolation(p_shape, grid, velocity, velocity.dimension[0]);

				// now update the velocity
				// update_velocity_field(velocity, grid, sphere_tool); //-> do it somewere else !!!
				// std::cout << "update velocity field !" << std::endl;
				// ???????????????????

				//update the velocity !!! here ??

				/*bool simulation_diverged = detect_simulation_divergence(cloth.forces, cloth.position);
				if (simulation_diverged == true)
				{
					std::cerr << " **** Simulation has diverged **** " << std::endl;
					std::cerr << " > Stop simulation iterations" << std::endl;
					user.gui.run = false;
					break;
				}*/
				//}
			}
		}
		//now update the velocity
		update_velocity_field(velocity, grid, sphere_tool);// -> do it somewere else !!!
		// std::cout << "update velocity field" << std::endl;
		// if (gui.laplacian_smoothing){
		// 	shape = laplacian_smoothing(shape, one_ring);
		// }
		//get some inspiration from td 08_cloth -> numerial_integration()
	}
}

//vec3 get_cell(vec3 point) {
//	//draw and think a little bit
//	return vec3(0.1, 0.1, 0.1);
//}


vec3 get_interpolated_velocity(const vec3& p, grid_3D<vec3> const& v, int N) {
	vec3 cell = get_cell(p, N); // get cell point belongs to 
	vec3 v_p = v(cell.x, cell.y, cell.z); // cell vector field value of the cell
	float vnew_x = v_p.x; float vnew_y = v_p.y; float vnew_z = v_p.z; vec3 v_p_new = v_p;
	float gridCellSize = 2.0f / (N - 1);

	/*std::cout << "p : " << p << std::endl;
	std::cout << "cell : " << cell << std::endl;
	std::cout << "v(p.x, p.y, p.z) : " << v_p << std::endl;*/

	int x_index1; int x_index2; int y_index1; int y_index2; int z_index1; int z_index2; vec3 next_cell1_v; vec3 next_cell2_v;
	vec3 centroid_cell1; vec3 centroid_cell2; float dist1; float dist2;

	for (int i = 0; i < 3; i++) {	 // iterate through cell
		if ((cell(i) != 0) && (cell(i) != 19)) { // if not on the boundary of the grid
			if (i == 0) { x_index1 = cell.x - 1; x_index2 = cell.x + 1; }
			else { x_index1 = cell.x; x_index2 = cell.x; };
			if (i == 1) { y_index1 = cell.y - 1; y_index2 = cell.y + 1; }
			else { y_index1 = cell.y; y_index2 = cell.y; };
			if (i == 2) { z_index1 = cell.z - 1; z_index2 = cell.z + 1; }
			else { z_index1 = cell.z; z_index2 = cell.z; };
			next_cell1_v = v(x_index1, y_index1, z_index1); next_cell2_v = v(x_index2, y_index2, z_index2); // get adjacent cells' vector fields
			centroid_cell1 = { -1.0 + x_index1 * gridCellSize + 0.5 * gridCellSize, -1.0 + y_index1 * gridCellSize + 0.5 * gridCellSize, -1.0 + z_index1 * gridCellSize + 0.5 * gridCellSize };
			centroid_cell2 = { -1.0 + x_index2 * gridCellSize + 0.5 * gridCellSize, -1.0 + y_index2 * gridCellSize + 0.5 * gridCellSize, -1.0 + z_index2 * gridCellSize + 0.5 * gridCellSize };
			dist1 = distance_3D(p, centroid_cell1); dist2 = distance_3D(p, centroid_cell2);
			v_p_new += (dist1 * next_cell1_v + dist2 * next_cell2_v) / (dist1 + dist2);
		}
	}
	return v_p_new;
}

float distance_3D(const vec3& p1, const vec3& p2) {
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
}

vec3 get_cell(const vec3& p, int N) {
	// returns lower left corner indices of the cell point p belongs to
	// function that converts from a point in space to the associated 3D grid cell it belongs to.
	// i.e. { x,y,z } -> { kx, ky, kz }
	// ! function returns the *lower bound* of the cell (i.e. the smallest of the 2 points that define the lower edge of a given cell along its axis)
	// https://math.stackexchange.com/questions/3135977/which-cell-in-a-grid-a-point-belongs-to#comment6460585_3136016

	// NOTE: current function is adapted for a grid contained in [-1;1] in each direction
	float gridCellSize = 2.0f / (N - 1);
	int sign_x; int sign_y; int sign_z; int index_x; int index_y; int index_z;
	if (p.x >= 0) { sign_x = 1.; }
	else { sign_x = -1.; }; if (p.y >= 0) { sign_y = 1.; }
	else { sign_y = -1.; }; if (p.z >= 0) { sign_z = 1.; }
	else { sign_z = -1.; }
	float px_shifted = p.x - sign_x * gridCellSize / 2.0; float py_shifted = p.y - sign_y * gridCellSize / 2.0; float pz_shifted = p.z - sign_z * gridCellSize / 2.0;

	// handle case where point is out of grid)
	for (int i=0; i<3; i++){
		if ( (p(i) < -1.0) || (p(i) > 1.0) ){
			return {-100,-100,-100};
		}
	}

	// x
	if (std::abs(p.x) < (gridCellSize / 2.0)) {
		index_x = N / 2 - 1;
	}
	else { // use shifted values
		if (sign_x == 1.) {
			index_x = int(px_shifted / gridCellSize) + N / 2;
		}
		else {
			index_x = int(px_shifted / gridCellSize) + N / 2 - 2;
		}
		if (index_x == (-1)) { index_x++; }; if (index_x == (N - 1)) { index_x--; }
	}
	// y
	if (std::abs(p.y) < (gridCellSize / 2.0)) {
		index_y = N / 2 - 1;
	}
	else { // use shifted values
		if (sign_y == 1.) {
			index_y = int(py_shifted / gridCellSize) + N / 2;
		}
		else {
			index_y = int(py_shifted / gridCellSize) + N / 2 - 2;
		}
		if (index_y == (-1)) { index_y++; }; if (index_y == (N - 1)) { index_y--; }
	}
	// z
	if (std::abs(p.z) < (gridCellSize / 2.0)) {
		index_z = N / 2 - 1;
	}
	else { // use shifted values
		if (sign_z == 1.) {
			index_z = int(pz_shifted / gridCellSize) + N / 2;
		}
		else {
			index_z = int(pz_shifted / gridCellSize) + N / 2 - 2;
		}
		if (index_z == (-1)) { index_z++; }; if (index_z == (N - 1)) { index_z--; }
	}

	return { int(index_x), int(index_y), int(index_z) };
}

vec3 trilinear_interpolation(cgp::vec3 const &p, cgp::grid_3D<cgp::vec3> const &grid, cgp::grid_3D<cgp::vec3> const &v, int N)
{
	// https://spie.org/samples/PM159.pdf, https://www.wikiwand.com/en/Trilinear_interpolation 
	vec3 cell = get_cell(p, N); // get lower left index of cell the point belongs to 
	float gridCellSize = 2.0f / (N - 1);
	vec3 c0; vec3 c1; vec3 c2; vec3 c3; vec3 c4; vec3 c5; vec3 c6; vec3 c7;

	// compute necessary adjacent cells & velocity values
	float x0 = grid(cell.x, cell.y, cell.z).x; float x1 = grid(cell.x + 1, cell.y, cell.z).x;
    float y0 = grid(cell.x, cell.y, cell.z).y; float y1 = grid(cell.x, cell.y+1, cell.z).y;
	float z0 = grid(cell.x, cell.y, cell.z).z; float z1 = grid(cell.x, cell.y, cell.z + 1).z;

	float deltaX = (p.x - x0)/(x1-x0); float deltaY = (p.y - y0)/(y1-y0); float deltaZ = (p.z - z0)/(z1-z0);
	
	c0 = v(cell.x, cell.y, cell.z);
	c1 = v(cell.x + 1, cell.y, cell.z) - v(cell.x, cell.y, cell.z);
	c2 = v(cell.x, cell.y+1, cell.z) - v(cell.x, cell.y, cell.z);
	c3 = v(cell.x, cell.y, cell.z+1) - v(cell.x, cell.y, cell.z);
	c4 = v(cell.x+1, cell.y+1, cell.z) - v(cell.x, cell.y+1, cell.z) - v(cell.x+1, cell.y, cell.z) + v(cell.x, cell.y, cell.z);
	c5 = v(cell.x, cell.y+1, cell.z+1) - v(cell.x, cell.y, cell.z+1) - v(cell.x, cell.y+1, cell.z) + v(cell.x, cell.y, cell.z);
	c6 = v(cell.x+1, cell.y, cell.z+1) - v(cell.x, cell.y, cell.z+1) - v(cell.x+1, cell.y, cell.z) + v(cell.x, cell.y, cell.z);
	c7 = v(cell.x + 1, cell.y + 1, cell.z + 1) - v(cell.x, cell.y + 1, cell.z + 1) - v(cell.x + 1, cell.y, cell.z + 1) - v(cell.x + 1, cell.y + 1, cell.z) +
		 v(cell.x + 1, cell.y, cell.z) + v(cell.x, cell.y, cell.z + 1) + v(cell.x, cell.y+1, cell.z) - v(cell.x, cell.y, cell.z);

	vec3 v_new = c0 + c1*deltaX + c2*deltaY + c3*deltaZ + c4*deltaX*deltaY + c5*deltaY*deltaZ + c6*deltaZ*deltaX + c7*deltaX*deltaY*deltaZ;
//	std::cout << "v_new : " << v_new << std::endl;
	return v_new;
// ! watch out for rightmost corner.
}

// lissage Laplacian
cgp::mesh laplacian_smoothing(cgp::mesh &shape, buffer<buffer<int>> one_ring){
	// complex implementation: http://rodolphe-vaillant.fr/entry/70/laplacian-smoothing-c-code-to-smooth-a-mesh, gave unstable results using the Explicit scheme.
	// simpler implementation based on: https://www.sciencedirect.com/topics/computer-science/laplacian-smoothing, implemented here
	// input: shape (mesh) of size 10 000 (10 000 vertices)

	cgp::mesh newShape = shape; float alpha = 0.05;
    // buffer<buffer<int>> one_ring = connectivity_one_ring(shape.connectivity); // to do: extract & store so it doesn't get recomputed each time

	int num_iter = 20;
	for (int iter = 0; iter < num_iter; iter++){
		for (int i = 0; i < shape.position.size(); i++){ // iterate over all vertices
			vec3 vertex = shape.position(i);
			vec3 v_s = {0., 0., 0.};
			buffer<int> neighbors_i = one_ring(i);

			for (int j = 0; j < neighbors_i.size(); j++){ // iterate over all neighbors of vertex i
				int neighborID = neighbors_i[j]; // get neighbor number
				vec3 neighbor = shape.position(neighborID);
				v_s += neighbor;
			}
			v_s = v_s / neighbors_i.size();
			v_s = vertex + alpha * (v_s - vertex);
			newShape.position(i) = v_s;
		}
	}
	return newShape;
}

#include "deformers.hpp"


using namespace cgp;

void integrate(mesh & shape, buffer<vec3> const& position_before_deformation, grid_3D<vec3>&velocity, grid_3D<vec3> const& grid, sphere_tool_structure const& sphere_tool, constant_velocity_structure const& constant_velocity) {
	
	size_t const N = shape.position.size();
	vec3 ref = { -100, -100, -100 }; // to check if point is in [-1, 1] grid
	size_t const N_substeps = 10; // integration substeps
	
	for (size_t k_substep = 0; k_substep < N_substeps; ++k_substep) {
		for (size_t k = 0; k < N; ++k)
		{
			vec3& p_shape = shape.position[k];                             // position to deform
			vec3 const& p_shape_original = position_before_deformation[k]; // reference position before deformation

			vec3 cell = get_cell(p_shape, velocity.dimension[0]);
			if (!are_equal(cell, ref)) { // if point is in grid

				float const dt = 0.005f;
				float a = 1;
				if (constant_velocity.type == direction_mouse_movement) a = 1;//probably useless

				p_shape = p_shape + a* dt * trilinear_interpolation(p_shape, cell, grid, velocity, velocity.dimension[0]);

				/*bool simulation_diverged = detect_simulation_divergence(cloth.forces, cloth.position);
				if (simulation_diverged == true)
				{
					std::cerr << " **** Simulation has diverged **** " << std::endl;
					std::cerr << " > Stop simulation iterations" << std::endl;
					user.gui.run = false;
					break;
				}*/
			}
		}
		update_velocity_field(velocity, grid, sphere_tool, constant_velocity); //now update the velocity
	}
}

void update_velocity_field(grid_3D<vec3>& velocity, grid_3D<vec3> const& grid, sphere_tool_structure const& sphere_tool, constant_velocity_structure const& constant_velocity)
{
	//see appendix of the paper for the formulas derivation
	int const N = int(velocity.dimension.x);
	vec3 nabla_p;
	vec3 nabla_q;
	vec3 u, w;

	for (int kx = 0; kx < N; ++kx) {
		for (int ky = 0; ky < N; ++ky) {
			for (int kz = 0; kz < N; ++kz) {
				//float c = 1.0 / (2.0 * N);//to do: put ny and nz
				//vec3 const p0 = grid(kx, ky, kz) + vec3(c, c, c);

				vec3 const p0 = grid(kx, ky, kz); 
				float r_x = norm(sphere_tool.c - p0);

				if (r_x > sphere_tool.r0) //enforce 0 velocity outside the sphere tool
				{
					velocity(kx, ky, kz) = vec3(0, 0, 0);
				}
				else 
				{
					if (constant_velocity.type == direction_view) {//along z axis
						//TO DO: USE THE DIRECTION VIEW INSTEAD OF Z AXIS
						u = vec3(1, 0, 0);
						w = vec3(0, 1, 0);
					}
					else if (constant_velocity.type == direction_normal) {
						u = vec3(1, 0, 0);
						if (norm(constant_velocity.dir) > 0) u = orthogonal_vector(normalize(constant_velocity.dir));
						w = cross(constant_velocity.dir, u);
					}
					else if (constant_velocity.type == direction_mouse_movement) {
						u = vec3(1, 0, 0);
						if (norm(constant_velocity.dir) > 0) u = orthogonal_vector(normalize(constant_velocity.dir));
						w = cross(constant_velocity.dir, u);
					}
					
					float e = dot(u, (p0 - sphere_tool.c));
					float f = dot(w, (p0 - sphere_tool.c));
					vec3 nabla_e = u;
					vec3 nabla_f = w;
					float tmp = (r_x - sphere_tool.ri) / (sphere_tool.r0 - sphere_tool.ri);
					float B3_4 = bernstein(3, 4, tmp);//to optimize: this is useless to recompute these all the time
					float B4_4 = bernstein(4, 4, tmp);
					float b = (B3_4 + B4_4);
					float db_dr = 2 * r_x / (sphere_tool.r0 - sphere_tool.ri);
					//for the report: 
					//https://www.wolframalpha.com/input?i2d=true&i=Partial%5BSqrt%5BPower%5B%5C%2840%29x-
					//5C%2841%29%2C2%5D%2BPower%5B%5C%2840%29y-1%5C%2841%29%2C2%5D%2BPower%5B%5C%2840%29z-1%5C%2841%29%2C2%5D%5D%2Cx%5D
					float nx = (p0.x - sphere_tool.c.x) / r_x;
					float ny = (p0.y - sphere_tool.c.y) / r_x;
					float nz = (p0.z - sphere_tool.c.z) / r_x;
					vec3 nabla_r = vec3(nx, ny, nz); 
					vec3 nabla_b = db_dr * nabla_r;
					vec3 nabla_1 = (1 - b) * nabla_e - e * nabla_b;
					vec3 nabla_2 = (1 - b) * nabla_f - f * nabla_b;

					velocity(kx, ky, kz) = cross(nabla_1, nabla_2);
				}
			}
		}
	}
}

//TO DO: might want to put these in utilities.cpp
float bernstein(int n, int k, float x) {
	return float(C(n, k)) * pow((1 - x), (n - k)) * pow(x, k);
}

int C(int n, int k)
{
	if (k > n) return 0;
	if (k == 0 || k == n) return 1;
	int nCr = C(n - 1, k - 1) + C(n - 1, k);
	return nCr;
}


void update_velocity_visual(segments_drawable& velocity_visual, buffer<vec3>& velocity_grid_data, grid_3D<vec3> const& velocity, grid_3D<vec3>& grid, float scale)
{
	//TO DO: put it at the center of each cube
	int const N = int(velocity.dimension.x);
	float const dL = 2.0f / (N - 1.0f);
	float const lambda = 0.01f * scale;

	float offset = 0;
	for (int kx = 0; kx < N; ++kx) {
		for (int ky = 0; ky < N; ++ky) {
			for (int kz = 0; kz < N; ++kz) {
				//vec3 const p0 = { -1 + kx * dL, -1 + ky * dL, 1e-4f };
				float c = 1.0 / (2.0 * N);
				vec3 const p0 = grid(kx, ky, kz) + vec3(c, c, c);
				//size_t const offset = velocity.index_to_offset(kx, ky);
				//size_t const offset = velocity.index_to_offset(kx, ky, kz);
				velocity_grid_data[2*offset + 0] = p0;
				velocity_grid_data[2*offset + 1] = p0 + lambda * velocity(kx, ky, kz);
				offset++;
			}
		}
	}

	velocity_visual.update(velocity_grid_data);
}



//TO DO: put this in utilities.cpp
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
	for (int i = 0; i < 3; i++) {
		if ((p(i) < -1.0) || (p(i) > 1.0)) {
			return { -100,-100,-100 };
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


//TO DO: put this in utilities.cpp
//vec3 trilinear_interpolation(cgp::vec3 const& p, cgp::grid_3D<cgp::vec3> const& grid, cgp::grid_3D<cgp::vec3> const& v, int N)
vec3 trilinear_interpolation(cgp::vec3 const& p, cgp::vec3 const& cell, cgp::grid_3D<cgp::vec3> const& grid, cgp::grid_3D<cgp::vec3> const& v, int N)
{
	// https://spie.org/samples/PM159.pdf, https://www.wikiwand.com/en/Trilinear_interpolation 
	//vec3 cell = get_cell(p, N); // get lower left index of cell the point belongs to 
	float gridCellSize = 2.0f / (N - 1);
	vec3 c0; vec3 c1; vec3 c2; vec3 c3; vec3 c4; vec3 c5; vec3 c6; vec3 c7;

	// compute necessary adjacent cells & velocity values
	float x0 = grid(cell.x, cell.y, cell.z).x; float x1 = grid(cell.x + 1, cell.y, cell.z).x;
	float y0 = grid(cell.x, cell.y, cell.z).y; float y1 = grid(cell.x, cell.y + 1, cell.z).y;
	float z0 = grid(cell.x, cell.y, cell.z).z; float z1 = grid(cell.x, cell.y, cell.z + 1).z;

	float deltaX = (p.x - x0) / (x1 - x0); float deltaY = (p.y - y0) / (y1 - y0); float deltaZ = (p.z - z0) / (z1 - z0);

	c0 = v(cell.x, cell.y, cell.z);
	c1 = v(cell.x + 1, cell.y, cell.z) - v(cell.x, cell.y, cell.z);
	c2 = v(cell.x, cell.y + 1, cell.z) - v(cell.x, cell.y, cell.z);
	c3 = v(cell.x, cell.y, cell.z + 1) - v(cell.x, cell.y, cell.z);
	c4 = v(cell.x + 1, cell.y + 1, cell.z) - v(cell.x, cell.y + 1, cell.z) - v(cell.x + 1, cell.y, cell.z) + v(cell.x, cell.y, cell.z);
	c5 = v(cell.x, cell.y + 1, cell.z + 1) - v(cell.x, cell.y, cell.z + 1) - v(cell.x, cell.y + 1, cell.z) + v(cell.x, cell.y, cell.z);
	c6 = v(cell.x + 1, cell.y, cell.z + 1) - v(cell.x, cell.y, cell.z + 1) - v(cell.x + 1, cell.y, cell.z) + v(cell.x, cell.y, cell.z);
	c7 = v(cell.x + 1, cell.y + 1, cell.z + 1) - v(cell.x, cell.y + 1, cell.z + 1) - v(cell.x + 1, cell.y, cell.z + 1) - v(cell.x + 1, cell.y + 1, cell.z) +
		v(cell.x + 1, cell.y, cell.z) + v(cell.x, cell.y, cell.z + 1) + v(cell.x, cell.y + 1, cell.z) - v(cell.x, cell.y, cell.z);

	vec3 v_new = c0 + c1 * deltaX + c2 * deltaY + c3 * deltaZ + c4 * deltaX * deltaY + c5 * deltaY * deltaZ + c6 * deltaZ * deltaX + c7 * deltaX * deltaY * deltaZ;
	//	std::cout << "v_new : " << v_new << std::endl;
	return v_new;

	// ! watch out for rightmost corner.
}

//TO DO: put this in utilities.cpp
cgp::mesh laplacian_smoothing(cgp::mesh& shape, buffer<buffer<int>> one_ring) {
	// complex implementation: http://rodolphe-vaillant.fr/entry/70/laplacian-smoothing-c-code-to-smooth-a-mesh, gave unstable results using the Explicit scheme
	// simpler implementation based on: https://www.sciencedirect.com/topics/computer-science/laplacian-smoothing, implemented here
	// input: shape (mesh) of size 10 000 (10 000 vertices)
	cgp::mesh newShape = shape; float alpha = 0.05;
	// buffer<buffer<int>> one_ring = connectivity_one_ring(shape.connectivity); // to do: extract & store so it doesn't get recomputed each time

	int num_iter = 20;
	for (int iter = 0; iter < num_iter; iter++) {
		for (int i = 0; i < shape.position.size(); i++) { // iterate over all vertices
			vec3 vertex = shape.position(i);
			vec3 v_s = { 0., 0., 0. };
			buffer<int> neighbors_i = one_ring(i);

			for (int j = 0; j < neighbors_i.size(); j++) { // iterate over all neighbors of vertex i
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

//to do: put this in utilities
bool are_equal(const cgp::vec3& v1, const cgp::vec3& v2) {
	if (v1.size() != v2.size()) { return false; }
	for (int i = 0; i < v1.size(); i++) {
		if (v1(i) != v2(i)) return false;
	}
	return true;
}


//TO DO (AT THE END): REMOVE THIS FUNCTION
//void apply_deformation(mesh& shape, buffer<vec3> const& position_before_deformation, vec2 const& translate_screen, vec3 const& picked_position, vec3 const& picked_normal, rotation_transform const& camera_orientation, deformer_parameters_structure const& deformer_parameters)
//{
//
//	/** 
//		shape:  The position of shape are the one to be deformed
//		position_before_deformation:  Reference position of the mesh before applying this deformation
//		translate_screen:   Input translation of the user in the 2D-screen coordinates - tr must be converted into a 3D transformation applied to the positions of shape
//		picked_position:    3D Position of the picked vertex
//		picked_normal:      Normal of the surface at the picked vertex position
//		camera_orientation: Current camera orientation - allows to convert the 2D-screen coordinates into 3D coordinates
//	*/
//
//
//	float const r = deformer_parameters.falloff; // radius of influence of the deformation
//	size_t const N = shape.position.size();
//	for (size_t k = 0; k < N; ++k)
//	{
//		vec3& p_shape = shape.position[k];                             // position to deform
//		vec3 const& p_shape_original = position_before_deformation[k]; // reference position before deformation
//		vec3 const& p_clicked = picked_position;                       // 3D position of picked position
//		vec3 const& n_clicked = picked_normal;                         // normal of the surface (before deformation) at the picked position
//
//		float const dist = norm(p_clicked - p_shape_original);         // distance between the picked position and the vertex before deformation
//
//		// TO DO: Implement the deformation models
//		// **************************************************************** //
//		// ...
//		if (deformer_parameters.type == deform_translate) // Case of translation
//		{
//			// Hint: You can convert the 2D translation in screen space into a 3D translation in the view plane in multiplying 
//			//       camera_orientation * vec3(translate_screen, 0)
//			vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
//
//			// Fake deformation (linear translation in the screen space) 
//			//   the following lines should be modified to get the expected smooth deformation
//			if (dist < r)
//				p_shape = p_shape_original + (1 - dist / r) * translation;
//
//		}
//		if (deformer_parameters.type == deform_twist)
//		{
//			// Deformation to implement
//		}
//		if (deformer_parameters.type == deform_scale)
//		{
//			// Deformation to implement"
//		}
//	}
//}
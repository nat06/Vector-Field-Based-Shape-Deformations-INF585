#pragma once
#include "utils.hpp"

using namespace cgp;

//TO DO: COMMENT THE FONCTIONS


bool are_equal(const cgp::vec3& v1, const cgp::vec3& v2) {
	if (v1.size() != v2.size()) { return false; }
	for (int i = 0; i < v1.size(); i++) {
		if (v1(i) != v2(i)) return false;
	}
	return true;
}

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

void set_tool_in_grid(vec3& new_pos, sphere_tool_structure& sphere_tool) {
	//set the tool position to new_pos and enforce it to be inside the grid boundaries
	if (new_pos.x + sphere_tool.r0 >= 1.0f) new_pos.x = 1.0 - sphere_tool.r0;
	if (new_pos.x - sphere_tool.r0 <= -1.0f) new_pos.x = -1.0 + sphere_tool.r0;
	if (new_pos.y + sphere_tool.r0 >= 1.0f) new_pos.y = 1.0 - sphere_tool.r0;
	if (new_pos.y - sphere_tool.r0 <= -1.0f) new_pos.y = -1.0 + sphere_tool.r0;
	if (new_pos.z + sphere_tool.r0 >= 1.0f) new_pos.z = 1.0 - sphere_tool.r0;
	if (new_pos.z - sphere_tool.r0 <= -1.0f) new_pos.z = -1.0 + sphere_tool.r0;
	sphere_tool.c = new_pos;
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


vec3 trilinear_interpolation(vec3 const& p, vec3 const& cell, grid_3D<vec3> const& grid, grid_3D<vec3> const& v, int N)
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
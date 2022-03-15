
#include "initialization.hpp"

using namespace cgp;

mesh initialize_plane()
{
	int const N = 100;
	return mesh_primitive_grid({-1,-1,0}, {1,-1,0}, {1,1,0}, {-1,1,0}, N, N);
}
mesh initialize_cylinder()
{
	float const h = 1.5f;
    float const radius = 0.4f;
    float const N_sample_circumferential = 80;
    float const N_sample_length = int( h/(2*3.14f*radius)*(N_sample_circumferential-1) + 1 + 0.5f );
    return mesh_primitive_cylinder(radius, {0,-h/2,0}, {0,h/2,0}, N_sample_circumferential, N_sample_length);
}
mesh initialize_sphere()
{
    int const N = 100;
    float const radius = 0.75f;
    return mesh_primitive_sphere(radius, {0,0,0}, N, 2*N);
}
mesh initialize_cube()
{
    int const N=40;
    return mesh_primitive_cubic_grid({0,0,0},{1,0,0},{1,1,0},{0,1,0}, {0,0,1},{1,0,1},{1,1,1},{0,1,1}, N, N, N);
}
mesh initialize_mesh()
{    
    std::string const filename = "assets/face.obj";
    mesh shape = mesh_load_file_obj(filename);
    for(auto& p : shape.position) 
        p *= 0.5f;
    return shape;
}

mesh initialize_mesh_2()
{
	//std::string const filename = "assets/camel.obj";
	std::string const filename = "assets/face.obj";
	mesh shape = mesh_load_file_obj(filename);
	for (auto& p : shape.position)
		p *= 0.5f;
	return shape;
}

mesh initialize_mesh_3() // this one does not work so well
{
	//std::string const filename = "assets/spoon.obj";
	std::string const filename = "assets/face.obj";
	mesh shape = mesh_load_file_obj(filename);
	for (auto& p : shape.position) {
	p *= 0.5f;
	p += vec3(0.5, -0.7, 0);
	//TO DO : might want to rotate it too
	}

	return shape;
}



//############################################################################
//PROJECT


grid_3D<vec3> initialize_grid(int N)
{
	//to do: do not initialize it as unit square

	grid_3D<vec3> grid;
	grid.resize(N, N, N);
	for (int kx = 0; kx < N; ++kx) {
		for (int ky = 0; ky < N; ++ky) {
			for (int kz = 0; kz < N; ++kz) {

				float const x = (2*kx / (N - 1.0f))-1.0f;
				float const y = (2 * ky / (N - 1.0f)) - 1.0f;
				float const z = (2 * kz / (N - 1.0f)) - 1.0f;
				//TO DO (idea) : grid adapted to mesh or mesh normalized to fit and have space in the grid

				grid(kx, ky, kz) = { x,y,z };//what is this ???

			}
		}
	}
	return grid;
}


void update_grid_segments(buffer<vec3>& segments_grid, grid_3D<vec3> const& grid)
{
	int const N = int(grid.dimension.x);

	int N_edge = 2 * (N - 1) * N * N + 2 * N * (N - 1) * N + 2 * N * N * (N - 1);
	if (segments_grid.size() < N_edge)
		segments_grid.resize(N_edge);

	int offset = 0;
	for (int kx = 0; kx < N-1; ++kx) {
		for (int ky = 0; ky < N; ++ky) {
			for (int kz = 0; kz < N; ++kz) {
				vec3 const p0 = grid(kx, ky, kz);
				vec3 const p1 = grid(kx + 1, ky, kz);
				segments_grid[offset] = p0; offset++;
				segments_grid[offset] = p1; offset++;
			}
		}
	}

	for (int kx = 0; kx < N; ++kx) {
		for (int ky = 0; ky < N-1; ++ky) {
			for (int kz = 0; kz < N; ++kz) {
				vec3 const p0 = grid(kx, ky, kz);
				vec3 const p1 = grid(kx, ky + 1, kz);
				segments_grid[offset] = p0; offset++;
				segments_grid[offset] = p1; offset++;
			}
		}
	}

	for (int kx = 0; kx < N; ++kx) {
		for (int ky = 0; ky < N; ++ky) {
			for (int kz = 0; kz < N - 1; ++kz) {
				vec3 const p0 = grid(kx, ky, kz);
				vec3 const p1 = grid(kx, ky, kz + 1);
				segments_grid[offset] = p0; offset++;
				segments_grid[offset] = p1; offset++;
			}
		}
	}

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







//############################################################################
//############################################################################


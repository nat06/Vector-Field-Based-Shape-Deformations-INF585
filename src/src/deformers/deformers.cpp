#include "deformers.hpp"
#include "../helpers/utils.hpp"

using namespace cgp;

void integrate(mesh & shape, buffer<vec3> const& position_before_deformation, grid_3D<vec3>&velocity, grid_3D<vec3> const& grid, sphere_tool_structure const& sphere_tool, constant_velocity_structure const& constant_velocity, bool const& bool_trilinear_interpolation) {
	
	size_t const N = shape.position.size();
	vec3 ref = { -100, -100, -100 }; // to check if point is in [-1, 1] grid
	size_t const N_substeps = 1; // integration substeps
	
	for (size_t k_substep = 0; k_substep < N_substeps; ++k_substep) {
		for (size_t k = 0; k < N; ++k)
		{
			vec3& p_shape = shape.position[k];                             // position to deform
			vec3 const& p_shape_original = position_before_deformation[k]; // reference position before deformation

			vec3 cell = get_cell(p_shape, velocity.dimension[0]);
			if (!are_equal(cell, ref)) { // if point is in grid

				float const dt = 0.005f;

				vec3 v;
				if (bool_trilinear_interpolation) { 
					v = trilinear_interpolation(p_shape, cell, grid, velocity, velocity.dimension[0]);
				}
				else {
					v = velocity(cell.x, cell.y, cell.z);
				}
				p_shape = p_shape +  constant_velocity.magnitude*2 * v;
			}
		}
	}
	std::cout << "Volume after deformation: " << mesh_volume(shape) << std::endl;
}

void update_velocity_field(grid_3D<vec3>& velocity, grid_3D<vec3> const& grid, sphere_tool_structure const& sphere_tool, constant_velocity_structure const& constant_velocity){
	//see appendix of the paper for the formulas derivation
	int const N = int(velocity.dimension.x);
	vec3 nabla_p;
	vec3 nabla_q;
	vec3 u, w;

	for (int kx = 0; kx < N; ++kx) {
		for (int ky = 0; ky < N; ++ky) {
			for (int kz = 0; kz < N; ++kz) {

				vec3 const p0 = grid(kx, ky, kz); 
				float r_x = norm(sphere_tool.c - p0);

				if (r_x > sphere_tool.r0){ //enforce 0 velocity outside the sphere tool
					velocity(kx, ky, kz) = vec3(0, 0, 0);
				}
				else{
					u = vec3(1, 0, 0);
					if (norm(constant_velocity.dir) > 0) u = orthogonal_vector(normalize(constant_velocity.dir));
					w = cross(constant_velocity.dir, u);
					
					float e = dot(u, (p0 - sphere_tool.c));
					float f = dot(w, (p0 - sphere_tool.c));
					vec3 nabla_e = u;
					vec3 nabla_f = w;
					float tmp = (r_x - sphere_tool.ri) / (sphere_tool.r0 - sphere_tool.ri);
					float B3_4 = bernstein(3, 4, tmp);//to optimize: this is useless to recompute these all the time
					float B4_4 = bernstein(4, 4, tmp);
					float b = (B3_4 + B4_4);
					float db_dr = 2 * r_x / (sphere_tool.r0 - sphere_tool.ri);
					float nx = (p0.x - sphere_tool.c.x) / r_x;
					float ny = (p0.y - sphere_tool.c.y) / r_x;
					float nz = (p0.z - sphere_tool.c.z) / r_x;
					vec3 nabla_r = vec3(nx, ny, nz); 
					vec3 nabla_b = db_dr* nabla_r;
					vec3 nabla_1 = (1 - b) * nabla_e - e * nabla_b;
					vec3 nabla_2 = (1 - b) * nabla_f - f * nabla_b;

					velocity(kx, ky, kz) = cross(nabla_1, nabla_2);
				}
			}
		}
	}
}

void update_velocity_visual(segments_drawable& velocity_visual, buffer<vec3>& velocity_grid_data, grid_3D<vec3> const& velocity, grid_3D<vec3>& grid, float scale){

	int const N = int(velocity.dimension.x);
	float const dL = 2.0f / (N - 1.0f);
	float const lambda = 0.01f * scale;
	float offset = 0;

	for (int kx = 0; kx < N; ++kx) {
		for (int ky = 0; ky < N; ++ky) {
			for (int kz = 0; kz < N; ++kz){
				float c = 1.0 / (2.0 * N);
				vec3 const p0 = grid(kx, ky, kz) + vec3(c, c, c);
				velocity_grid_data[2*offset + 0] = p0;
				velocity_grid_data[2*offset + 1] = p0 + lambda * velocity(kx, ky, kz);
				offset++;
			}
		}
	}
	velocity_visual.update(velocity_grid_data);
}

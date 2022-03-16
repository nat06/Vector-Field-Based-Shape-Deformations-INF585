#include "deformers.hpp"
#include "../utils.hpp"


using namespace cgp;

void integrate(mesh & shape, buffer<vec3> const& position_before_deformation, grid_3D<vec3>&velocity, grid_3D<vec3> const& grid, sphere_tool_structure const& sphere_tool, constant_velocity_structure const& constant_velocity, bool const& bool_trilinear_interpolation) {
	
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

				vec3 v;
				if (bool_trilinear_interpolation) { 
					v = trilinear_interpolation(p_shape, cell, grid, velocity, velocity.dimension[0]);
				}
				else {
					v = velocity(cell.x, cell.y, cell.z);
				}

				p_shape = p_shape + a * dt * v;
				//p_shape = p_shape + a* dt * trilinear_interpolation(p_shape, cell, grid, velocity, velocity.dimension[0]);

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
		//update_velocity_field(velocity, grid, sphere_tool, constant_velocity); //now update the velocity
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
					else if (constant_velocity.type == direction_normal || constant_velocity.type == direction_inverse_normal) {
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
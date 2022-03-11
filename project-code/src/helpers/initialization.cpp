
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


//TO DO: MOVE THIS SOMEWERE ELSE (EITHER SCENE OR DEFORMERS)
void update_velocity_field(grid_3D<vec3>& velocity, grid_3D<vec3> const& grid, sphere_tool_structure const& sphere_tool, vec3 const& previous_tool_pos, vec3 const& tr, vec3& constant_vel, vec3 const& picked_normal, enum constant_velocity_direction velocity_direction_type)
//void update_velocity_field(grid_3D<vec3>& velocity, grid_3D<vec3> const& grid, sphere_tool_structure const& sphere_tool, vec3 const& previous_tool_pos, vec3 const& tr)
{

	int const N = int(velocity.dimension.x);

	vec3 nabla_p;
	vec3 nabla_q;

	//see appendix of the paper for the formulas

	for (int kx = 0; kx < N; ++kx) {
		for (int ky = 0; ky < N; ++ky) {
			for (int kz = 0; kz < N; ++kz) {

				//float c = 1.0 / (2.0 * N);//to do: put ny and nz
				//vec3 const p0 = grid(kx, ky, kz) + vec3(c, c, c);
				//float r_x = norm(sphere_tool.c - p0 );

				vec3 const p0 = grid(kx, ky, kz); //+ vec3(c, c, c);
				float r_x = norm(sphere_tool.c - p0);
				
				//TO DO: update this

				//case 1: constant vector field (translation in the region)
				/*if (r_x < sphere_tool.ri) {
					nabla_p = vec3(1, 5, 6);
					nabla_q = vec3(0, 3, 2);
				}
				else if (r_x < sphere_tool.r0) {
					nabla_p = vec3(0, 0, 0);
					nabla_q = vec3(0, 0, 0);
				}
				else if (r_x < sphere_tool.r0) {
					nabla_p = vec3(0, 0, 0);
					nabla_q = vec3(0, 0, 0);
				}*/

				//velocity(kx, ky, kz) = cross(nabla_p, nabla_q);

				//TO DO: 
				//WHERE TO FIND e, f, r and their first order partials ??? 
				// 
				if (r_x > sphere_tool.r0) {//should this be enforced ?? 
					velocity(kx, ky, kz) = vec3(0, 0, 0);
				}
				else {

					//TO DO: CHANGE PLACE OF THIS (THEY DONT HAVE TO BE HERE)
					//now we have to change this !!!
					/*vec3 u(1, 0, 0);
					vec3 w(0, 1, 0);*/

					//use the tool movement as a direction for the contant velocity field
					/*vec3 u = cgp::normalize(previous_tool_pos - sphere_tool.c);
					vec3 w = cgp::orthogonal_vector(u);*/

					//NORMAL
					vec3 u, w;

					if (velocity_direction_type == 0) {//along z axis

						u = vec3(1, 0, 0);
						w = vec3(0, 1, 0);
						constant_vel = cross(u, w);//changge this ! -> not always the picked normal
						//std::cout << "constant vel const" << constant_vel << std::endl;

					}
					else if (velocity_direction_type == 1) {//direction_normal

						u = vec3(1, 0, 0);
						if (norm(picked_normal) > 0) {
							//std::cout << "oyea" << std::endl;
							u = orthogonal_vector(normalize(picked_normal));
						}
						w = cross(picked_normal, u);
						constant_vel = picked_normal;//changge this ! -> not always the picked normal
						//std::cout << "constant vel normal" << constant_vel << std::endl;

					}
					else if (velocity_direction_type == 2) {//along mouse movement

						u = vec3(0, 0, 0);
						if (norm(previous_tool_pos - sphere_tool.c) > 0) {
							//std::cout << "oyea" << std::endl;
							u = orthogonal_vector(normalize(previous_tool_pos - sphere_tool.c));
						}
						w = cross(picked_normal, u);
						constant_vel = normalize(cross(u, w));

					}

					//maybe this should be enough
					constant_vel - cross(u, w);

					
					

					/*std::cout << "picked_normal = " << picked_normal<<std::endl;
					std::cout << "u = " << u << std::endl;
					std::cout << "w = " << w << std::endl;
					std::cout << "constant velo direction" << constant_velocity_direction << std::endl;*/

					//vec3 w = { 0, 0, 0 };
					/*vec3 w = orthogonal_vector(normalize(previous_tool_pos - sphere_tool.c));
					vec3 u = cgp::orthogonal_vector(w);*/
					
					
					//std::cout << "constant velo direction" << constant_velocity_direction << std::endl;
					/////////////////////////////////////////////////////////
					
					//std::cout << "u = " << u << std::endl;

					//case 1: constant vector field (translation in the region)
					float e = dot(u, (p0 - sphere_tool.c));//not sure
					float f = dot(w, (p0 - sphere_tool.c));
					vec3 nabla_e = u;
					vec3 nabla_f = w;

					//std::cout << "e = " << e << std::endl;
					//std::cout << "f = " << f << std::endl;

					float tmp = (r_x - sphere_tool.ri) / (sphere_tool.r0 - sphere_tool.ri);
					float B3_4 = bernstein(3, 4, tmp);//to optimize: this is useless to recompute these all the time
					float B4_4 = bernstein(4, 4, tmp);
					float b = (B3_4 + B4_4); //not sure about this...

					//std::cout << "b = " << b << std::endl;

					float db_dr = 2 * r_x / (sphere_tool.r0 - sphere_tool.ri);

					//std::cout << "db_dr = " << db_dr << std::endl;

					//https://www.wolframalpha.com/input?i2d=true&i=Partial%5BSqrt%5BPower%5B%5C%2840%29x-1%5C%2841%29%2C2%5D%2BPower%5B%5C%2840%29y-1%5C%2841%29%2C2%5D%2BPower%5B%5C%2840%29z-1%5C%2841%29%2C2%5D%5D%2Cx%5D
					float nx = (p0.x - sphere_tool.c.x) / r_x;
					float ny = (p0.y - sphere_tool.c.y) / r_x;
					float nz = (p0.z - sphere_tool.c.z) / r_x;
					vec3 nabla_r = vec3(nx, ny, nz); // not sure about this
					vec3 nabla_b = db_dr * nabla_r;

					/*std::cout << "nabla_r = " << nabla_r << std::endl;
					std::cout << "nabla_b = " << nabla_b << std::endl;*/

					vec3 nabla_1 = (1 - b) * nabla_e - e * nabla_b;
					vec3 nabla_2 = (1 - b) * nabla_f - f * nabla_b;

					/*std::cout << "nabla_1 = " << nabla_1 << std::endl;
					std::cout << "nabla_2 = " << nabla_2 << std::endl;*/


					velocity(kx, ky, kz) = cross(nabla_1, nabla_2);
				}
				
				//to do: change names of nabla_1, nabla_2 to nabla_e, f
			}
		}
	}

}

//put these somewhere else ?
float bernstein(int n, int k, float x) {
	return float(C(n, k))* pow((1 - x),(n - k))* pow(x, k);
}

int C(int n, int k)
{//https://prepinsta.com/competitive-advanced-coding/binomial-coefficient/
	if (k > n) return 0;
	if (k == 0 || k == n) return 1;
	int nCr = C(n-1, k-1) + C(n-1, k);
	return nCr;
}


//############################################################################
//############################################################################


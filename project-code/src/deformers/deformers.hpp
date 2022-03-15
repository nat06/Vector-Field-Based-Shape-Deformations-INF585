#pragma once


#include "cgp/cgp.hpp"

enum constant_velocity_direction {
	direction_view,
	direction_normal,
	direction_mouse_movement
};

struct constant_velocity_structure {//might want to change the name
	constant_velocity_direction type = direction_normal;   
	cgp::vec3 dir = cgp::vec3(0, 0, 0);
	//might want to add some stuff
};

//TO DO: MOVE THIS TO ANOTHER FILE
struct sphere_tool_structure{
	cgp::vec3 c; // Position
	cgp::vec3 c0; //colors
	cgp::vec3 ci;
	float r0; //radiuses 
	float ri;
};


//void integrate(cgp::mesh& shape, cgp::buffer<cgp::vec3> const& position_before_deformation, cgp::vec3 const& picked_position, cgp::grid_3D<cgp::vec3>& velocity, cgp::grid_3D<cgp::vec3> const& grid, sphere_tool_structure const& sphere_tool, cgp::vec3 const& previous_tool_pos, cgp::vec3 const& tr, cgp::vec3& constant_vel, cgp::vec3 const& picked_normal, cgp::buffer<cgp::buffer<int> > one_ring, constant_velocity_structure const& constant_velocity);
void integrate(cgp::mesh& shape, cgp::buffer<cgp::vec3> const& position_before_deformation, cgp::grid_3D<cgp::vec3>& velocity, cgp::grid_3D<cgp::vec3> const& grid, sphere_tool_structure const& sphere_tool, constant_velocity_structure const& constant_velocity);
void update_velocity_field(cgp::grid_3D<cgp::vec3>& velocity, cgp::grid_3D<cgp::vec3> const& grid, sphere_tool_structure const& sphere_tool, constant_velocity_structure const& constant_velocity);
void update_velocity_visual(cgp::segments_drawable& velocity_visual, cgp::buffer<cgp::vec3>& velocity_grid_data, cgp::grid_3D<cgp::vec3> const& velocity, cgp::grid_3D<cgp::vec3>& grid, float scale);

//move these to utilities.hpp
cgp::vec3 get_cell(const cgp::vec3& p, int N);
cgp::vec3 trilinear_interpolation(cgp::vec3 const& p, cgp::vec3 const& cell, cgp::grid_3D<cgp::vec3> const& grid, cgp::grid_3D<cgp::vec3> const& v, int N);
cgp::mesh laplacian_smoothing(cgp::mesh& shape, cgp::buffer<cgp::buffer<int>> one_ring);

float bernstein(int n, int k, float x);
int C(int n, int k);
bool are_equal(const cgp::vec3& v1, const cgp::vec3& v2);//might want to change the place of this


// TO REMOVE
//enum deformer_type_enum {
//	deform_translate,
//	deform_twist,
//	deform_scale
//};
//
//enum deformer_direction_enum {
//	direction_view_space,
//	direction_surface_normal
//};
//
//struct deformer_parameters_structure {
//	deformer_type_enum type           = deform_translate;      // Type of deformation type
//	deformer_direction_enum direction = direction_view_space;  // Type of deformation direction
//	float falloff                     = 1 / 5.0f;              // Falloff/influence distance (can be adjusted from the GUI or with mouse scroll)
//};

//void apply_deformation(cgp::mesh& shape, cgp::buffer<cgp::vec3> const& position_before_deformation, cgp::vec2 const& translate_screen, cgp::vec3 const& picked_position, cgp::vec3 const& picked_normal, cgp::rotation_transform const& camera_orientation, deformer_parameters_structure const& deformer_parameters);


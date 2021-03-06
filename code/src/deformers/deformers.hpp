#pragma once
#include "cgp/cgp.hpp"

enum constant_velocity_direction { //TO DO: CHANGE THE NAMES OF THESE
	direction_view,
	direction_normal,
	direction_inverse_normal,
	direction_mouse_movement,
	deformation_painting_normal,
	deformation_painting_normal_bis,
	deformation_painting_inverse
};

struct constant_velocity_structure { 
	constant_velocity_direction type = direction_normal;   
	cgp::vec3 dir = cgp::vec3(0, 0, 0);
	float magnitude = 1;
};

struct sphere_tool_structure{
	cgp::vec3 c; // Position
	cgp::vec3 previous_c;
	cgp::vec3 c0; //colors
	cgp::vec3 ci;
	float r0; //radiuses 
	float ri;
};

void integrate(cgp::mesh& shape, cgp::buffer<cgp::vec3> const& position_before_deformation, cgp::grid_3D<cgp::vec3>& velocity, cgp::grid_3D<cgp::vec3> const& grid, sphere_tool_structure const& sphere_tool, constant_velocity_structure const& constant_velocity, bool const& bool_trilinear_interpolation);
void update_velocity_field(cgp::grid_3D<cgp::vec3>& velocity, cgp::grid_3D<cgp::vec3> const& grid, sphere_tool_structure const& sphere_tool, constant_velocity_structure const& constant_velocity);
void update_velocity_visual(cgp::segments_drawable& velocity_visual, cgp::buffer<cgp::vec3>& velocity_grid_data, cgp::grid_3D<cgp::vec3> const& velocity, cgp::grid_3D<cgp::vec3>& grid, float scale);

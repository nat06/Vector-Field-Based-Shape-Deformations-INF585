#pragma once


#include "cgp/cgp.hpp"
using namespace cgp;

//#######################project
#include "../helpers/initialization.hpp"  // good practice ??
//#######################



enum deformer_type_enum {
	deform_translate,
	deform_twist,
	deform_scale
};

enum deformer_direction_enum {
	direction_view_space,
	direction_surface_normal
};

struct deformer_parameters_structure {
	deformer_type_enum type           = deform_translate;      // Type of deformation type
	deformer_direction_enum direction = direction_view_space;  // Type of deformation direction
	float falloff                     = 1 / 5.0f;              // Falloff/influence distance (can be adjusted from the GUI or with mouse scroll)
};




void apply_deformation(cgp::mesh& shape, cgp::buffer<cgp::vec3> const& position_before_deformation, cgp::vec2 const& translate_screen, cgp::vec3 const& picked_position, cgp::vec3 const& picked_normal, cgp::rotation_transform const& camera_orientation, deformer_parameters_structure const& deformer_parameters);


//###########################
//PROJECT


// void integrate(cgp::mesh& shape, cgp::buffer<cgp::vec3> const& position_before_deformation, cgp::vec3 const& picked_position, deformer_parameters_structure const& deformer_parameters, cgp::grid_3D<cgp::vec3>& velocity, cgp::grid_3D<cgp::vec3> const& grid, sphere_tool_structure const& sphere_tool);
void integrate(mesh &shape, buffer<vec3> const &position_before_deformation, vec3 const &picked_position, deformer_parameters_structure const &deformer_parameters, grid_3D<vec3> &velocity, grid_3D<vec3> const &grid, sphere_tool_structure const &sphere_tool, buffer<buffer<int> > one_ring);
	// cgp::vec3 get_cell(cgp::vec3 point);
cgp::vec3 get_cell(const cgp::vec3 &p, int N);
//cgp::vec3 get_interpolated_velocity(cgp::vec3 point, cgp::grid_3D<cgp::vec3>& velocity);
cgp::vec3 get_interpolated_velocity(cgp::vec3 const& p, cgp::grid_3D<cgp::vec3> const& v, int N);
cgp::vec3 trilinear_interpolation(cgp::vec3 const &p, cgp::grid_3D<cgp::vec3> const &grid, cgp::grid_3D<cgp::vec3> const &v, int N);
float distance_3D(const cgp::vec3& p1, const cgp::vec3& p2);
cgp::mesh laplacian_smoothing(cgp::mesh &shape, buffer<buffer<int>> one_ring);
//###########################

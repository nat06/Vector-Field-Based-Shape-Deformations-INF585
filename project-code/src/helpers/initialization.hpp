#pragma once

#include "cgp/cgp.hpp"

// Special initialization of shapes
cgp::mesh initialize_plane();
cgp::mesh initialize_cylinder();
cgp::mesh initialize_sphere();
cgp::mesh initialize_cube();

//  TO DO : use a filename as argument of one function only
cgp::mesh initialize_mesh();
cgp::mesh initialize_mesh_2();
cgp::mesh initialize_mesh_3();

//############################################################
//PROJECT

cgp::grid_3D<cgp::vec3> initialize_grid(int N);
void update_grid_segments(cgp::buffer<cgp::vec3>& segments_grid, cgp::grid_3D<cgp::vec3> const& grid);


//TO DO: MOVE ALL THIS IN A DIFFERENT FILE -> DEFORMERS OR WE MIGHT HAVE PROBLEMS

//this might not be the best place for this
struct sphere_tool_structure
{
    cgp::vec3 c; // Position
    cgp::vec3 c0; //colors
    cgp::vec3 ci;
    float r0; //radiuses 
    float ri;

};

//TO DO: put this in another file (not initialization.cpp)
void update_velocity_visual(cgp::segments_drawable& velocity_visual, cgp::buffer<cgp::vec3>& velocity_grid_data, cgp::grid_3D<cgp::vec3> const& velocity, cgp::grid_3D<cgp::vec3>& grid, float scale);
//void update_velocity_field(cgp::grid_3D<cgp::vec3>& velocity, cgp::grid_3D<cgp::vec3> const& grid, sphere_tool_structure const& sphere_tool, cgp::vec3 const& previous_tool_pos, cgp::vec3 const& tr, cgp::vec3& constant_velocity_direction);




//############################################################

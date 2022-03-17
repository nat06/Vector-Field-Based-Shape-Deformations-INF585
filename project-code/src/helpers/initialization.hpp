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
cgp::mesh initialize_mesh_4();

cgp::grid_3D<cgp::vec3> initialize_grid(int N);
void initialize_grid_segments(cgp::buffer<cgp::vec3>& segments_grid, cgp::grid_3D<cgp::vec3> const& grid);
void initialize_grid_box(cgp::buffer<cgp::vec3>& segments_grid, cgp::grid_3D<cgp::vec3> const& grid);


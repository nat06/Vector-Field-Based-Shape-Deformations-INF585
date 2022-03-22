#pragma once

#include "cgp/cgp.hpp"

// Special initialization of shapes
cgp::mesh initialize_plane();
cgp::mesh initialize_cylinder();
cgp::mesh initialize_sphere();
cgp::mesh initialize_cube();
cgp::mesh initialize_mesh();
cgp::mesh initialize_mesh_2();
cgp::mesh initialize_mesh_3();
cgp::mesh initialize_mesh_4();
cgp::mesh initialize_mesh_5();
cgp::mesh initialize_mesh_6();
cgp::mesh initialize_mesh_7();

cgp::grid_3D<cgp::vec3> initialize_grid(int N);
void initialize_grid_segments(cgp::buffer<cgp::vec3>& segments_grid, cgp::grid_3D<cgp::vec3> const& grid);
void initialize_grid_box(cgp::buffer<cgp::vec3>& segments_grid, cgp::grid_3D<cgp::vec3> const& grid);

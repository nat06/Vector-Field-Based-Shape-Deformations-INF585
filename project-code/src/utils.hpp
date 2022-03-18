#pragma once
#include "cgp/cgp.hpp"
#include "deformers/deformers.hpp"

cgp::vec3 get_cell(const cgp::vec3& p, int N);
cgp::vec3 trilinear_interpolation(cgp::vec3 const& p, cgp::vec3 const& cell, cgp::grid_3D<cgp::vec3> const& grid, cgp::grid_3D<cgp::vec3> const& v, int N);
cgp::mesh laplacian_smoothing(cgp::mesh& shape, cgp::buffer<cgp::buffer<int>> one_ring);

void set_tool_in_grid(cgp::vec3& new_pos, sphere_tool_structure& sphere_tool);
float bernstein(int n, int k, float x);
int C(int n, int k);
bool are_equal(const cgp::vec3& v1, const cgp::vec3& v2);//might want to change the place of this
float mesh_volume(cgp::mesh& shape);
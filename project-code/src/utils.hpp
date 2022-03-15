
#pragma once
#include "cgp/cgp.hpp"


cgp::vec3 get_cell(const cgp::vec3& p, int N);
cgp::vec3 trilinear_interpolation(cgp::vec3 const& p, cgp::vec3 const& cell, cgp::grid_3D<cgp::vec3> const& grid, cgp::grid_3D<cgp::vec3> const& v, int N);
cgp::mesh laplacian_smoothing(cgp::mesh& shape, cgp::buffer<cgp::buffer<int>> one_ring);

float bernstein(int n, int k, float x);
int C(int n, int k);
bool are_equal(const cgp::vec3& v1, const cgp::vec3& v2);//might want to change the place of this
#pragma once

#include "cgp/display/opengl/opengl.hpp"
#include "cgp/shape/spatial_domain/spatial_domain_grid_3D/spatial_domain_grid_3D.hpp"
#include "../segments_drawable/segments_drawable.hpp"

namespace cgp
{
	segments_drawable spatial_domain_grid_drawable(spatial_domain_grid_3D const& domain, GLuint shader = segments_drawable::default_shader);
	segments_drawable spatial_domain_grid_drawable_voxels(spatial_domain_grid_3D const& domain, GLuint shader = segments_drawable::default_shader);
}

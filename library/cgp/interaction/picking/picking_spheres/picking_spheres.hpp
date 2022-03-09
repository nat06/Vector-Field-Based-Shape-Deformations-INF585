#pragma once

#include "../picking_structure/picking_structure.hpp"
#include "cgp/interaction/camera/camera.hpp"
#include "cgp/math/math.hpp"

namespace cgp
{
	/** Picking through a set of spheres with specified centers and given radius */
	picking_structure picking_spheres(vec2 const& screen_click, buffer<vec3> const& spheres_centers, float spheres_radius, camera_base const& camera, camera_projection const& projection);

	/** Compute picking of a mesh vertex assuming that each vertex is a sphere of specified radius */
	picking_structure picking_mesh_vertex_as_sphere(vec2 const& screen_click, buffer<vec3> const& vertex_position, buffer<vec3> const& vertex_normal, float picking_distance, camera_base const& camera, camera_projection const& projection);
}
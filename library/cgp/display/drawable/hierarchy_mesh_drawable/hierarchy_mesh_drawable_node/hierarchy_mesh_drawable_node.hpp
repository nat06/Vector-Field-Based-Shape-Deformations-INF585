#pragma once

#include "cgp/display/drawable/mesh_drawable/mesh_drawable.hpp"
#include "cgp/math/affine/affine_rts/affine_rts.hpp"

namespace cgp
{
struct hierarchy_mesh_drawable_node
{
	hierarchy_mesh_drawable_node();

	hierarchy_mesh_drawable_node(const mesh_drawable& element,
									const std::string& name_parent="global_frame",
									const affine_rts& transform = affine_rts());



	mesh_drawable drawable;   // the visual element associated to this node
	std::string name_parent;  // name of the parent node

	// Hierarchical affine transform express in relative coordinates with respect to the parent node frame
	affine_rts transform;
	// Hierarchical Affine transform expressed in global coordinates (supposed to be computed automatically)
	affine_rts global_transform; 
};

template <typename ENVIRONMENT>
void draw(hierarchy_mesh_drawable_node const& node, ENVIRONMENT const& environment)
{
	// copy of the mesh drawable (lightweight element) - to preserve its uniform parameters
	mesh_drawable visual_element = node.drawable;

	// Update local uniform values (and combine them with uniform already stored in the mesh)
	visual_element.transform = node.global_transform * visual_element.transform;

	if(visual_element.shader!=0)
		draw(visual_element, environment);
}

template <typename ENVIRONMENT>
void draw_wireframe(hierarchy_mesh_drawable_node const& node, ENVIRONMENT const& environment, vec3 const& color={0,0,1})
{
	// copy of the mesh drawable (lightweight element) - to preserve its uniform parameters
	mesh_drawable visual_element = node.drawable;

	// Update local uniform values (and combine them with uniform already stored in the mesh)
	visual_element.transform = node.global_transform * visual_element.transform;

	if(visual_element.shader!=0)
		draw_wireframe(visual_element, environment, color);
}

}

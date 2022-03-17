#include "picking_visual.hpp"

using namespace cgp;

void picking_visual_parameters::initialize()
{
	sphere.initialize(mesh_primitive_sphere(0.02f), "Sphere picking");
	circle.initialize(curve_primitive_circle(1.0f, { 0,0,0 }, { 0,0,1 }, 40), "Circle picking");
}

void picking_visual_parameters::draw(cgp::scene_environment_basic const& environment, vec3 const& position, vec3 const& normal, float radius)
{
	// Display a sphere representing the picked vertex
	/*sphere.transform.translation = position;
	cgp::draw(sphere, environment);*/

	// Display a circle representing the radius of influence and the orientation of the surface and 
	/*circle.transform.scaling = radius;
	circle.transform.translation = position;
	circle.transform.rotation = rotation_transform::between_vector({ 0,0,1 }, normal);
	cgp::draw(circle, environment);*/
}
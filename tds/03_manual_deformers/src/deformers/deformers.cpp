#include "deformers.hpp"



using namespace cgp;

void apply_deformation(mesh& shape, buffer<vec3> const& position_before_deformation, vec2 const& translate_screen, vec3 const& picked_position, vec3 const& picked_normal, rotation_transform const& camera_orientation, deformer_parameters_structure const& deformer_parameters)
{

	/** 
		shape:  The position of shape are the one to be deformed
		position_before_deformation:  Reference position of the mesh before applying this deformation
		translate_screen:   Input translation of the user in the 2D-screen coordinates - tr must be converted into a 3D transformation applied to the positions of shape
		picked_position:    3D Position of the picked vertex
		picked_normal:      Normal of the surface at the picked vertex position
		camera_orientation: Current camera orientation - allows to convert the 2D-screen coordinates into 3D coordinates
	*/


	float const r = deformer_parameters.falloff; // radius of influence of the deformation
	size_t const N = shape.position.size();
	for (size_t k = 0; k < N; ++k)
	{
		vec3& p_shape = shape.position[k];                             // position to deform
		vec3 const& p_shape_original = position_before_deformation[k]; // reference position before deformation
		vec3 const& p_clicked = picked_position;                       // 3D position of picked position
		vec3 const& n_clicked = picked_normal;                         // normal of the surface (before deformation) at the picked position

		float const dist = norm(p_clicked - p_shape_original);         // distance between the picked position and the vertex before deformation
		float const w = exp(-pow(dist, 2) / pow(r, 2));
		vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);

		// TO DO: Implement the deformation models
		// **************************************************************** //
		// ...
		if (deformer_parameters.type == deform_translate) // Case of translation
		{
			// Hint: You can convert the 2D translation in screen space into a 3D translation in the view plane in multiplying 
			//       camera_orientation * vec3(translate_screen, 0)
			if (deformer_parameters.direction == direction_view_space){
				p_shape = p_shape_original + w * translation; // p' = p + w * t3D
			}
			else{
				p_shape = p_shape_original + w * translation.z * n_clicked; // p' = p + w * t3D.z
			}

			// Fake deformation (linear translation in the screen space) 
			//   the following lines should be modified to get the expected smooth deformation
			// if (dist < r)
			// 	p_shape = p_shape_original + (1 - dist / r) * translation;

		}
		if (deformer_parameters.type == deform_twist)
		{
			// Deformation to implement
			/// performing a rotation to twist
			float theta = translation.x * w;
			rotation_transform R;
			vec3 axis_u;
			mat3 I = {1, 0, 0,
					  0, 1, 0,
					  0, 0, 1};
			if (deformer_parameters.direction == direction_view_space){
				axis_u = camera_orientation * vec3(0,0,1); // forward vector, v * {0,0,1}
			}
			else{
				axis_u = picked_normal;
			}
			// Rodrigues' rotation formula, rotation of a vector x around the axis u by an angle θ
			// mat3 U = {0, -axis_u.z, axis_u.y,
			// 		  axis_u.z, 0, -axis_u.x,
			// 		  -axis_u.y, axis_u.x, 0};
			// mat3 R = I + sin(theta) * U + (1 - cos(theta)) * U * U;
			// p_shape = R * (p_shape_original - p_clicked) + p_clicked;

			// OR:
			float angle = w * theta;
			R = rotation_transform::from_axis_angle(axis_u, angle);
			p_shape = R * (p_shape_original - p_clicked) + p_clicked;
		}
		if (deformer_parameters.type == deform_scale)
		{
			// Deformation to implement"
			float s = 1 + w * translation.x;
			p_shape = s * (p_shape_original - p_clicked) + p_clicked; 
		}
	}
}
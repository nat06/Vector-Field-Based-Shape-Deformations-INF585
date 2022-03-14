#include "gui.hpp"


bool gui_parameters::display()
{
	ImGui::Checkbox("Display frame", &display_frame);
	ImGui::Checkbox("Wireframe", &display_wireframe);

	//#PROJECT
	ImGui::Checkbox("Grid", &display_grid_edge);
	ImGui::Checkbox("Velocity", &display_velocity);
	ImGui::Checkbox("Velocity", &display_constant_velocity);
	ImGui::SliderFloat("ri", &gui_ri, 0.07f, 0.2f);
	ImGui::SliderFloat("r0", &gui_r0, 0.01f, 0.5f);

	ImGui::Text("Laplacian smoothing:"); // Select surface to be deformed
	ImGui::Checkbox("activate", &laplacian_smoothing);
	ImGui::SliderInt("steps", &smoothing_steps, 10, 100);


	ImGui::Text("direction of constant velocity (inner sphere):"); // Select the type of deformation to apply
	int* ptr_velocity_type = (int*)& velocity_direction_type; 
	ImGui::RadioButton("view_space", ptr_velocity_type, direction_view); ImGui::SameLine();
	ImGui::RadioButton("normal", ptr_velocity_type, direction_normal); ImGui::SameLine();
	ImGui::RadioButton("mouse movement", ptr_velocity_type, direction_mouse_movement);
	//#

	

	// Create interface
	ImGui::Text("Surface type:"); // Select surface to be deformed
	int* ptr_surface_type  = reinterpret_cast<int*>(&surface_type); // trick - use pointer to select enum
	bool new_surface = false;
    new_surface |= ImGui::RadioButton("Plane",ptr_surface_type, surface_plane); ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Cylinder",ptr_surface_type, surface_cylinder); ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Sphere",ptr_surface_type, surface_sphere); ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Cube",ptr_surface_type, surface_cube);  ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Mesh",ptr_surface_type, surface_mesh);

	new_surface |= ImGui::RadioButton("Camel", ptr_surface_type, surface_mesh_2);
	new_surface |= ImGui::RadioButton("Spoon", ptr_surface_type, surface_mesh_3);

	ImGui::Text("Deformer type:"); // Select the type of deformation to apply
	int* ptr_deformer_type = (int*)&deformer_parameters.type;
	ImGui::RadioButton("Translate",ptr_deformer_type, deform_translate); ImGui::SameLine();
    ImGui::RadioButton("Twist",ptr_deformer_type, deform_twist); ImGui::SameLine();
    ImGui::RadioButton("Scale",ptr_deformer_type, deform_scale);

	ImGui::Text("Deformer direction:"); // Select the type of deformation to apply
	int* ptr_deformer_direction = (int*)&deformer_parameters.direction;
    ImGui::RadioButton("View space",ptr_deformer_direction, direction_view_space); ImGui::SameLine();
    ImGui::RadioButton("Surface normal",ptr_deformer_direction, direction_surface_normal);

	// Select falloff distance using slider
    ImGui::SliderFloat("Falloff distance", &deformer_parameters.falloff, 0.01f, 0.8f);

	

	return new_surface;
}
#include "gui.hpp"


bool gui_parameters::display()
{	
	
	ImGui::Text("Visibility:");
	ImGui::Checkbox("Frame", &display_frame);
	ImGui::Checkbox("Mesh wireframe", &display_wireframe);
	ImGui::Checkbox("Grid", &display_grid_edge);
	ImGui::Checkbox("Velocity field", &display_constant_velocity);
	
	ImGui::Text("\n Tool parameters:"); 
	ImGui::SliderFloat("ri", &gui_ri, 0.03f, 0.2f);
	ImGui::SliderFloat("r0", &gui_r0, 0.005f, 0.5f);
	
	ImGui::Text("Direction of constant velocity (inner sphere):"); 
	int* ptr_velocity_type = (int*)&constant_velocity_parameters.type;
	ImGui::RadioButton("view_space", ptr_velocity_type, direction_view); ImGui::SameLine();
	ImGui::RadioButton("normal", ptr_velocity_type, direction_normal); ImGui::SameLine();
	ImGui::RadioButton("mouse movement", ptr_velocity_type, direction_mouse_movement);
	
	ImGui::Text("\n Surface type:"); // Select surface to be deformed
	int* ptr_surface_type  = reinterpret_cast<int*>(&surface_type); // trick - use pointer to select enum
	bool new_surface = false;
    new_surface |= ImGui::RadioButton("Plane",ptr_surface_type, surface_plane); ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Cylinder",ptr_surface_type, surface_cylinder); ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Sphere",ptr_surface_type, surface_sphere); ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Cube",ptr_surface_type, surface_cube);  ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Mesh",ptr_surface_type, surface_mesh);
	new_surface |= ImGui::RadioButton("Camel", ptr_surface_type, surface_mesh_2);
	new_surface |= ImGui::RadioButton("Spoon", ptr_surface_type, surface_mesh_3);
	
	ImGui::Text("\n Laplacian smoothing:"); // Select surface to be deformed
	ImGui::Checkbox("activate", &laplacian_smoothing);
	ImGui::SliderInt("steps", &smoothing_steps, 10, 100);

	return new_surface;
}
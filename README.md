# Vector-Field-Based-Shape-Deformations-INF585-
Reimplementation of the paper Vector Field Based Shape Deformations for Ecole Polytechnique INF585 - Computer Animation's final project

TO DO LIST:

- when moving without clicking: move the tool along the shape
- when moving + shift + click: move the tool with a projection along the screen space
- more intuitive approach with the tool
- remove argument previous_tool_pos from the functions ?
- remove require_normal_update (not sure)
- display bounding box of the grid
- more choices of meshes on the gui
- bending ?
- check it is necessary to change velocity at each integration step
- keep deformer direction (view space, normal) + add mouse movement (add it on the gui too)
- add a visual bounding box for the grid (add it on the gui too)
- change the name of the timer => timer_update_velocity
- add arrow for the velocity (en cours)
- be able to make a bigger grid and generalize code for this

//-----------------------------------
DONE:
//-----------------------------------

- **enforce the fact that the mesh cannot go out of the grid**
- **trilinear interpolation**
- **use normal as vector field**
- **put an arrow vor the velocity direction**
- **remove tiny circle of the tool**
- replace Nx, Ny, Nz, by only N
- display velocity at each point (not at the center of each cell), and adapt computations accordingly
- laplacian smoothing

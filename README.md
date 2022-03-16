# Vector-Field-Based-Shape-Deformations-INF585-
Reimplementation of the paper Vector Field Based Shape Deformations for Ecole Polytechnique INF585 - Computer Animation's final project

TO DO LIST:

- boutton gui pour trilinear interpolation
- compilation instructions in a readme
- (revoir instructions sur moodle)
- pas de build ni d'executable
- implement sculpting metaphor

- make a function for the tool to stay in the grid -> scene.cpp l.33
- scale and rotate the the meshes when initialized
- if time: mettre outils pour scale les mesh (dans gui)
- if time: cange color ? fancy shaders ?

- FIX problem with arrow when using view_space
- plus de choix de vitesse constante au milieu de la inner ball 
- -> rotations avec les fleches ?
- bending ?

</br>

_CODE CLEANUP_
- remove useless arguments of fonctions
- remove useless variables
- rename unclear variables
- bending ?
</br>

_..._
- more intuitive approach with the tool ?
- when moving without clicking: move the tool along the shape
- more intuitive approach with the tool
- be able to make a bigger grid and generalize code for this

</br>
**DONE:**

- **enforce the fact that the mesh cannot go out of the grid**
- **trilinear interpolation**
- **use normal as vector field**
- **put an arrow vor the velocity direction**
- **remove tiny circle of the tool**
- **replace Nx, Ny, Nz, by only N**
- **display velocity at each point (not at the center of each cell), and adapt computations accordingly**
- **laplacian smoothing**
- **gui for laplacian smoothing + slider for number of steps**
- **when moving without clicking: move the tool along the shape**
- **when moving + shift + click: move the tool with a projection along the screen space**
- **regler probleme avec _enum_**
- easy: more range choice for ri, ro
- fix problem with the normal
- inverse normal
- bounding box for the grid
- camera zoom with molette de souris ?

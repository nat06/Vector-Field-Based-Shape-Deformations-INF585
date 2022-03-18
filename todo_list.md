TO DO LIST:

-if the interactive
motion changes the position of the tool by ∆r, then the integration
inside the inner region moves the points by ∆r as well
  -> FIX THIS

- **~~fix the 'double' deformation problem~~**
- fix problem with deformation painting
- compute time of each deformation (ex: 10 steps) -> make a table 
- add deformation GIF to README
- **~~scale and rotate the meshes when initialized N~~**
- if time: change color ? fancy shaders ? N
- plus de choix de vitesse constante au milieu de la inner ball  A
- -> rotations avec les fleches ?
- **~~update velocity visual along with changes in gui tool~~**
- **~~compute the volume to make a table N~~**
- **~~FIX problem with arrow when using view_space A~~**
- **~~implement sculpting metaphor A?~~**
- **~~make a function for the tool to stay in the grid -> scene.cpp l.33 A~~**
- **~~boutton gui pour trilinear interpolation A~~**
- **~~boutton gui pour tool display~~**
- **~~compilation instructions in a readme N~~**
- **~~(revoir instructions sur moodle) N~~**
- **~~pas de build ni d'executable~~**

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

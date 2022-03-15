# Vector-Field-Based-Shape-Deformations-INF585-
Reimplementation of the paper Vector Field Based Shape Deformations for Ecole Polytechnique INF585 - Computer Animation's final project

TO DO LIST:

_PRIORITY_
- plus de choix de mesh (+ peut etre la possibilite de les scale, + custom mesh ?)
- display la bounding grid (voir un td qui le fait)
- plus de choix de vitesse constante au milieu de la inner ball 
- bending ?
- compilation instructions
- nice github readme presentation
- fancy shaders ?
</br>


_CODE CLEANUP_
- remove useless arguments of fonctions
- remove useless variables
- rename unclear variables
- bending ?
</br>

_..._
- more intuitive approach with the tool ?
- check it is necessary to change velocity at each integration step
- add arrow for the velocity (en cours)
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

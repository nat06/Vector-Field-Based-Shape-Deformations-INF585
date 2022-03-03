#pragma once

/*! \mainpage CGP Code Documentation
 *
 * # Introduction
 *
 * cgp - Visual Computing Library - is a simple lightweight library on top of OpenGL provided to ease learning of 3D programming while avoiding re-coding everything from scratch (matrices, mesh structures, etc.). The library provides several helper structure and functions to set up 3D scene with interactive and/or animated elements.
 *
 * The objective of the library is to be simple as simple as possible to read and use. The majority of cgp structures and functions are aimed to be minimalistic without hidden states. The code remains fully compatible with direct raw OpenGL calls and user-defined shaders. The animation loop itself is entirely defined by the user.
 *
 * 
 *
 */


// To skip all cgp internal debug, declare cgp_NO_DEBUG as in the following line
// #define cgp_NO_DEBUG


#include "base/base.hpp"
#include "containers/containers.hpp"
#include "math/math.hpp"
#include "files/files.hpp"
#include "shape/shape.hpp"
#include "display/display.hpp"
#include "interaction/interaction.hpp"
#include "helper_common_scene/helper_common_scene.hpp"
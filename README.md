# Vector Field Based Shape Deformations 

An Ecole Polytechnique INF585 - Computer Animation's final project.

Implementation of the paper [Vector Field Based Shape Deformations](http://graphics.stanford.edu/courses/cs468-07-winter/Papers/fts-vfbsd-06.pdf) by Wolfram von Funck, Holger Theisel and Hans-Peter Seidel from MPI Informatik, with experiments.

## Credits
Credits to the original authors of the paper for their approach as well as to Ecole Polytechnique Professor [Damien Rohmer](https://imagecomputing.net/damien.rohmer/) for C++ environment code provided in the context of the INF585 course.

## Running the project
#### Compatible system

Below are the main steps to run this project and repeat all experiments.
To start, a C++14 (or newer) compatible compiler (CGG/CLang or recent Visual Studio) and an OpenGL 3.3 (or newer) compatible system are required to use the `CGP` (*Computer Graphics Programming*) library which this project utilizes. Additional information related to the `CGP` library can be found [here](https://imagecomputing.net/cgp/content/01_general/index.html). Detailed instructions for compiling CGP can be found [here](https://imagecomputing.net/cgp/compilation/content/01_compilation/index.html).

#### System setup
The setup varies slightly depending on the OS. 
* `MacOS`

For `MacOS`, first check that g++/clang++ is installed by running `g++ -v` in a Terminal. Follow the installation instruction if the OS proposes to install it.

The following development tools should be installed: `CMake`, `GLFW` and `pkg-config`. 

```bash
brew install cmake
brew install glfw
brew install pkg-config
```

* For `Linux`

For `Linux` systems, the setup is similar to `MacOS`.

```bash
sudo apt-get update
sudo apt-get install build-essential pkg-config
sudo apt-get install cmake 
sudo apt-get install libglfw3-dev
```
* For `Windows`

For `Windows` systems, the use of Microsoft Visual Studio is recommended as it comes with a default C++ compiler. Detailed instructions  are provided [here](https://imagecomputing.net/cgp/compilation/content/02_windows_setup/index.html).

#### Compile & Run

* `MacOS` and `Linux`

This method relies on the provided `CMakeLists.txt` file.

For the very first compilation, the procedure is as follows:
``` bash
cd project-code
mkdir build && cd build

# Run CMake, generate a Makefile
cmake ..

# Compile
make
cd ..

# Run the executable from the root directory
./build/project-code
```

For any subsequent compilation, one can simply compile (`make`) and run the executable from the root directory (from `project-code`).

* `Windows`

The project can be directly run from Microsoft Visual Studio ([details](https://imagecomputing.net/cgp/compilation/content/02_windows_setup/index.html)).

# APG

## Getting Started

On Linux follow the start of [the raylib wiki](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux) to install the correct libs.

`git clone --recurse-submodules -j8 https://github.com/ArchieAtkinson/APG.git`  
`cd APG`    
`mkdir build`   
`cmake -S . -B build`   
`cmake --build build`   

If not using CMake a header file must be made in the inc folder with the root path. Check configuration\root_dir.h.in for the template. 

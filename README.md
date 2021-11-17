# Album Poster Generator - APG

This is a project to generate album cover poster programmatically. Just provide the album artwork, artists name, album name, release date, album length and track list. 

![](example_output/folklore-Taylor_Swift.png)

## Usage

The program take two arguments, album artwork (as .png) and album information (as .txt).

APG [path/to/album/artwork.png] [path/to/album/info.txt]

For insight on how to structure the info.txt, please look in the example_data folder. If no arguments are given it will use the Folklore example data will be used.

Once you are started the program it will show you a render of the poster. If the text doesn't look great don't worry, it will look fine in the final image. To generate the poster just press escape and wait for a couple of seconds. The program will generate the poster and close. The output will be in the project directory.  

## Building

On Linux follow the start of [the raylib wiki](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux) to install the correct libs.

`git clone --recurse-submodules -j8 https://github.com/ArchieAtkinson/APG.git`  
`cd APG`    
`mkdir build`   
`cmake -S . -B build`   
`cmake --build build --target ALL_BUILD --config Release`   

If not using CMake a header file must be made in the inc folder with the root path. Check configuration\root_dir.h.in for the template. 

## Other

This has been tested only Window and Linux using g++ 17 and Ninja. If you have any problems or questions please create a ticket. Pull requests welcome. 
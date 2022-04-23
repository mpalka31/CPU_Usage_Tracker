# CPU_Usage_Tracker
This is application for CPU usage tracking. It constantly scans CPU usage and displays average usage every 1 second.
## COMPILE PROJECT
This project uses CMake build system. It is recommended to store all compilation related files inside build folder. To do so, follow this guide.\n
In your terminall window go into root directory of this project.
```
$ cd CPU_Usage_Tracker
```
Now you have to create build directory,
```
$ mkdir build
```
cd into build, run CMake, and build poject.
```
$ cd build
$ cmake ../
$ cmake --build .
```
## RUN APPLICATION
To run compiled application simply type this inside build directory.
```
$ ./cut
```

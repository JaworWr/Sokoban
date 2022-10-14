# Sokoban
This is an implementation of the [Sokoban](https://en.wikipedia.org/wiki/Sokoban) I prepared for a C programming course back in 2016. Unfortunately, I didn't follow the best code practices back 
then. Nonetheless, the program is functional. The biggest change from the original code is adding a CMakeList file.

Note that the whole interface is in Polish.
## Compilation
First, install GTK - the app uses version 3. Then:
```commandline
mkdir build
cd build
cmake ..
make
```
Afterwards, you can run the app:
```commandline
cd ..
build/Sokoban
```
Make sure to run it in the root directory of the repository, otherwise it won't be able to locate the image files.

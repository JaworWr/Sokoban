# Sokoban
This is an implementation of the [Sokoban](https://en.wikipedia.org/wiki/Sokoban) I prepared for a C programming course back in 2016. I decided to keep most of the original code intact, despite 
the fact that I didn't follow good programming practices at the time. The 
biggest change is adding a CMakeList file for easier compilation.

Note that the entire interface is in Polish.
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

## Level file format
The program uses a rather standard text-based file format. Some example levels are contained in the file `Przykładowe poziomy.txt`. Note that the level names must be enclosed in single quotes.
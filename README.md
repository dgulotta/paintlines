paintlines
==========

a program for drawing tileable images consisting of lines and dots

Installation instructions
=========================
Paintlines requires the following software:
* A C++ compiler that supports C++11
* Qt 5
* pkg-config

The following software is optional:
* FFTW 3 (for Stripes, Squiggles, and Quasiperiodic Stripes designs)
* GraphicsMagick or ImageMagick (for saving multipage TIFF files)
* Lua (for adding custom rules to Lines)

The following commands will compile and run the program.
```
qmake lines.pro
make
./lines
```

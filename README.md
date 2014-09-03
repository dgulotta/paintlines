paintlines
==========

a program for drawing tileable images consisting of lines and dots

Installation instructions
=========================
Paintlines requires the following software:
* A C++ compiler that supports C++11
* Qt 5
* FFTW 3 (required for paintstripes, paintsquiggles, and quasiperiodic_paintstripes programs only)
* GraphicsMagick or ImageMagick (only required if you want to save multipage TIFF files)
* Lua (only required if you want to create custom rules for paintlines)

The following commands will compile and run the 'paintclouds' program.  Other programs can be compiled similarly.
```
cd paintclouds
qmake paintclouds.pro
make
./paintclouds
```

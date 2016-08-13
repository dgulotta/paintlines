paintlines [![Build Status](https://travis-ci.org/dgulotta/paintlines.svg?branch=master)](https://travis-ci.org/dgulotta/paintlines)
==========

a program for drawing tileable images consisting of lines and dots

Installation instructions
=========================
Paintlines requires the following software:
* A C++ compiler that supports C++14 (gcc >=4.9 and clang >=3.4 should work)
* Qt 5.4 or newer

The following software is optional:
* pkg-config
* FFTW 3 (for Stripes, Squiggles, and Quasiperiodic Stripes designs)
* GraphicsMagick or ImageMagick (for saving multipage TIFF files)
* Lua (for adding custom rules to Lines)

The following commands will compile and run the program.
```
qmake lines.pro
make
./lines
```

paintlines
==========

a program for drawing tileable images consisting of lines and dots

Installation instructions
=========================
Paintlines requires the following software:
* A C++ compiler that supports C++11
* Qt 4
* FFTW 3 (required for paintstripes, paintsquiggles, and quasiperiodic_painstripes programs only)

The following commands will compile and run the 'paintclouds' program.  Other programs can be compiled similarly.
```
cd paintclouds
qmake paintclouds.pro
make
./paintclouds
```

TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release link_pkgconfig

QT += widgets

PKGCONFIG += fftw3

HEADERS	+= quasiperiodic_paintstripes.h \
	quasistripesform.h \
	../randgen.h \
	../canvas.h \
	../color.h \
	../basicform.h

SOURCES	+= quasiperiodic_paintstripes.cpp \
	main.cpp \
	quasistripesform.cpp \
	../randgen.cpp \
	../basicform.cpp

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


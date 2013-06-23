TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release link_pkgconfig

PKGCONFIG += fftw3

HEADERS	+= quasiperiodic_paintstripes.h \
	quasiperiodic_paintstripeswidget.h \
	../basic_painter.h \
	../basic_painterwidget.h \
	quasistripesform.h

SOURCES	+= quasiperiodic_paintstripes.cpp \
	quasiperiodic_paintstripeswidget.cpp \
	main.cpp \
	quasistripesform.cpp

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on debug link_pkgconfig

PKGCONFIG += fftw3

QT += widgets

HEADERS	+= paintstripes.h \
	../basicform.h \
	stripesform.h \
	../randgen.h \
	../stripes_common.h \
	../randomizewidget.h \
	../randomize.h \
	../canvas.h \
	../symmetric_canvas.h \
	../color.h

SOURCES	+= paintstripes.cpp \
	../basicform.cpp \
	main.cpp \
	stripesform.cpp \
	../randgen.cpp \
	../stripes_common.cpp \
	../randomizewidget.cpp \
	../randomize.cpp \
	../symmetric_canvas.cpp

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



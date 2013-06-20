TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release link_pkgconfig

PKGCONFIG += fftw3

HEADERS	+= paintstripes.h \
	paintstripeswidget.h \
	../painter.h \
	../painterwidget.h \
	../basic_painter.h \
	../basic_painterwidget.h \
	../basicform.h \
	stripesform.h \
	../randgen.h

SOURCES	+= paintstripes.cpp \
	paintstripeswidget.cpp \
	../painter.cpp \
	../painterwidget.cpp \
	../basicform.cpp \
	main.cpp \
	stripesform.cpp \
	../randgen.cpp

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



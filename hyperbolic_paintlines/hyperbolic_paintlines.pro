TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS	+= ../basic_painter.h \
	../basic_painterwidget.h \
	../hyperbolic_painter.h \
	hyperbolic_paintlines.h \
	hyperbolic_paintlineswidget.h \
	hyperboliclinesform.h \
	../randgen.h

SOURCES	+= ../hyperbolic_painter.cpp \
	hyperbolic_paintlines.cpp \
	hyperbolic_paintlineswidget.cpp \
	main.cpp \
	hyperboliclinesform.cpp \
	../randgen.cpp

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


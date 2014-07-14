TEMPLATE        = app
LANGUAGE        = C++

CONFIG  += qt warn_on release

HEADERS += converterform.h \
	../basicform.h \
	../randgen.h \
	../randomizewidget.h \
	../randomize.h \
	../canvas.h \
	../symmetric_canvas.h \
	../color.h \
	../hyperbolic_painter.h \
	hyperbolic_converter.h \
	../hyperbolic_polygons.h \
	../interpolate.h

SOURCES += converterform.cpp \
	main.cpp \
	../basicform.cpp \
	../randgen.cpp \
	../randomizewidget.cpp \
	../randomize.cpp \
	../symmetric_canvas.cpp \
	../hyperbolic_painter.cpp \
	hyperbolic_converter.cpp \
	../interpolate.cpp

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


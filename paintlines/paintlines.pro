TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS	+= paintlines.h \
	paintlineswidget.h \
	../painter.h \
	../painterwidget.h \
	../basic_painter.h \
	../basic_painterwidget.h \
	linesform.h \
	../basicform.h \
	../randgen.h \
	../layer_painter.h \
	../magick.h

SOURCES	+= paintlines.cpp \
	main.cpp \
	paintlineswidget.cpp \
	../painter.cpp \
	../painterwidget.cpp \
	linesform.cpp \
	../basicform.cpp \
	../randgen.cpp \
	../layer_painter.cpp \
	../magick.cpp

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

QMAKE_CXXFLAGS += $$system(GraphicsMagick++-config --cppflags)
LIBS += $$system(GraphicsMagick++-config --libs)

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

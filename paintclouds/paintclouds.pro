TEMPLATE        = app
LANGUAGE        = C++

CONFIG  += qt warn_on release

HEADERS += paintclouds.h \
	cloudsform.h \
	../basicform.h \
	../randgen.h \
	../randomizewidget.h \
	../randomize.h \
	../canvas.h \
	../symmetric_canvas.h \
	../color.h

SOURCES += paintclouds.cpp \
	main.cpp \
	cloudsform.cpp \
	../basicform.cpp \
	../randgen.cpp \
	../randomizewidget.cpp \
	../randomize.cpp \
	../symmetric_canvas.cpp

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


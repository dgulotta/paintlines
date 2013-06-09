TEMPLATE        = app
LANGUAGE        = C++

CONFIG  += qt warn_on release

HEADERS += paintclouds.h \
	paintcloudswidget.h \
	../painter.h \
	../painterwidget.h \
	../basic_painter.h \
	../basic_painterwidget.h \
	cloudsform.h \
	../basicform.h

SOURCES += paintclouds.cpp \
	paintcloudswidget.cpp \
	../painter.cpp \
	../painterwidget.cpp \
	main.cpp \
	cloudsform.cpp \
	../basicform.cpp

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


TEMPLATE        = app
LANGUAGE        = C++

CONFIG  += qt warn_on release

HEADERS += ca.h \
	caform.h \
	../canvas.h \
	../color.h \
	../randgen.h \
	../basicform.h

SOURCES += ca.cpp \
	caform.cpp \
	main.cpp \
	../randgen.cpp \
	../basicform.cpp

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release link_pkgconfig

PKGCONFIG += fftw3

HEADERS	+= paintsquiggles.h \
	paintsquiggleswidget.h \
	../painter.h \
	../painterwidget.h \
	../basic_painter.h \
	../basic_painterwidget.h \
	../basicform.h \
	squigglesform.h \
	../randgen.h

SOURCES	+= paintsquiggles.cpp \
	paintsquiggleswidget.cpp \
	../painter.cpp \
	../painterwidget.cpp \
	../basicform.cpp \
	main.cpp \
	squigglesform.cpp \
	../randgen.cpp

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


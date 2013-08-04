TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release link_pkgconfig

PKGCONFIG += fftw3

packagesExist(GraphicsMagick++) {
	PKGCONFIG += GraphicsMagick++
	DEFINES += MULTIPAGE
} else:packagesExist(Magick++) {
	PKGCONFIG += Magick++
	DEFINES += MULTIPAGE
}

HEADERS	+= paintsquiggles.h \
	paintsquiggleswidget.h \
	../painter.h \
	../painterwidget.h \
	../basic_painter.h \
	../basic_painterwidget.h \
	../basicform.h \
	squigglesform.h \
	../randgen.h \
	../stripes_common.h \
	../layer_painter.h \
	../randomcolorwidget.h

SOURCES	+= paintsquiggles.cpp \
	paintsquiggleswidget.cpp \
	../painter.cpp \
	../painterwidget.cpp \
	../basicform.cpp \
	main.cpp \
	squigglesform.cpp \
	../randgen.cpp \
	../stripes_common.cpp \
	../layer_painter.cpp \
	../randomcolorwidget.cpp

contains(DEFINES,MULTIPAGE) {
	HEADERS += ../magick.h
	SOURCES += ../magick.cpp
}

*-g++* {
    QMAKE_CXXFLAGS += --std=c++11
}

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


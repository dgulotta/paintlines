TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release link_pkgconfig

PKGCONFIG += fftw3

QT += widgets

packagesExist(GraphicsMagick++) {
	PKGCONFIG += GraphicsMagick++
	DEFINES += MULTIPAGE
} else:packagesExist(Magick++) {
	PKGCONFIG += Magick++
	DEFINES += MULTIPAGE
}

HEADERS	+= paintsquiggles.h \
	../basicform.h \
	squigglesform.h \
	../randgen.h \
	../stripes_common.h \
	../layer.h \
	../randomcolorwidget.h \
	../randomizewidget.h \
	../randomize.h \
	../canvas.h \
	../symmetric_canvas.h \
	../color.h

SOURCES	+= paintsquiggles.cpp \
	../basicform.cpp \
	main.cpp \
	squigglesform.cpp \
	../randgen.cpp \
	../stripes_common.cpp \
	../layer.cpp \
	../randomcolorwidget.cpp \
	../randomizewidget.cpp \
	../randomize.cpp \
	../symmetric_canvas.cpp

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


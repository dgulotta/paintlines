TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release link_pkgconfig

packagesExist(GraphicsMagick++) {
	PKGCONFIG += GraphicsMagick++
	DEFINES += MULTIPAGE
} else:packagesExist(Magick++) {
	PKGCONFIG += Magick++
	DEFINES += MULTIPAGE
}

HEADERS	+= ../hyperbolic_painter.h \
	hyperbolic_paintlines.h \
	hyperboliclinesform.h \
	../randgen.h \
	../basicform.h \
	../canvas.h \
	../color.h \
	../layer.h \
	../hyperbolic_polygons.h

SOURCES	+= ../hyperbolic_painter.cpp \
	hyperbolic_paintlines.cpp \
	main.cpp \
	hyperboliclinesform.cpp \
	../randgen.cpp \
	../basicform.cpp \
	../layer.cpp

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


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

packagesExist(lua) {
	PKGCONFIG += lua
	DEFINES += LUARULES
	HEADERS += luafuncs.h
	SOURCES += luafuncs.cpp
}

HEADERS	+= paintlines.h \
	linesform.h \
	../basicform.h \
	../randgen.h \
	../layer.h \
	../randomcolorwidget.h \
	../randomizewidget.h \
	../randomize.h \
	../canvas.h \
	../symmetric_canvas.h \
	../color.h

SOURCES	+= paintlines.cpp \
	main.cpp \
	linesform.cpp \
	../basicform.cpp \
	../randgen.cpp \
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

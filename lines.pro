TEMPLATE	= app
LANGUAGE	= C++

CONFIG += qt warn_on release link_pkgconfig c++14

QT += widgets

HEADERS += canvas.h \
	color.h \
	converterwidget.h \
	fftw_cxx.h \
	hyperbolic.h \
	hyperbolic_group.h \
	hyperbolic_polygons.h \
	hyperbolic_symmetry_chooser.h \
	imagedata.h \
	imagegeneratorwidget.h \
	interpolate.h \
	layer.h \
	mainform.h \
	randgen.h \
	randomcolorwidget.h \
	randomize.h \
	randomizewidget.h \
	symmetry.h \
	symmetric_canvas.h \
	ca/ca.h \
	ca/cawidget.h \
	converter/converters.h \
	converter/loaderwidget.h \
	hyperbolic_paintlines/hyperboliclineswidget.h \
	hyperbolic_paintlines/hyperbolic_paintlines.h \
	paintclouds/cloudswidget.h \
	paintclouds/hyperboliccloudswidget.h \
	paintclouds/paintclouds.h \
	paintlines/lineswidget.h \
	paintlines/paintlines.h \
	trap/trap.h \
	trap/trapwidget.h \
	trap/quasitrapwidget.h

SOURCES += converterwidget.cpp \
	hyperbolic.cpp \
	hyperbolic_group.cpp \
	hyperbolic_polygons.cpp \
	hyperbolic_symmetry_chooser.cpp \
	imagedata.cpp \
	imagegeneratorwidget.cpp \
	interpolate.cpp \
	layer.cpp \
	main.cpp \
	mainform.cpp \
	randgen.cpp \
	randomcolorwidget.cpp \
	randomize.cpp \
	randomizewidget.cpp \
	ca/ca.cpp \
	ca/cawidget.cpp \
	converter/converters.cpp \
	converter/loaderwidget.cpp \
	hyperbolic_paintlines/hyperbolic_paintlines.cpp \
	hyperbolic_paintlines/hyperboliclineswidget.cpp \
	paintclouds/cloudswidget.cpp \
	paintclouds/hyperboliccloudswidget.cpp \
	paintclouds/paintclouds.cpp \
	paintlines/paintlines.cpp \
	paintlines/lineswidget.cpp \
	trap/trap.cpp \
	trap/trapwidget.cpp \
	trap/quasitrapwidget.cpp

packagesExist(GraphicsMagick++) {
	PKGCONFIG += GraphicsMagick++
	DEFINES += MULTIPAGE
} else:packagesExist(Magick++) {
	PKGCONFIG += Magick++
	DEFINES += MULTIPAGE
}

contains(DEFINES,MULTIPAGE) {
	HEADERS += magick.h
	SOURCES += magick.cpp
}

packagesExist(luajit) {
	PKGCONFIG += luajit
	DEFINES += LUARULES
} else: packagesExist(lua) {
	PKGCONFIG += lua
	DEFINES += LUARULES
}

contains(DEFINES,LUARULES) {
	HEADERS += paintlines/luafuncs.h
	SOURCES += paintlines/luafuncs.cpp
}

packagesExist(fftw3) {
	PKGCONFIG += fftw3
	DEFINES += FFTWPROGS
	HEADERS += stripes_common.h \
		paintsquiggles/paintsquiggles.h \
		paintsquiggles/squiggleswidget.h \
		paintstripes/paintstripes.h \
		paintstripes/stripeswidget.h \
		quasiperiodic_paintstripes/quasiperiodic_paintstripes.h \
		quasiperiodic_paintstripes/quasistripeswidget.h
	SOURCES += stripes_common.cpp \
		paintsquiggles/paintsquiggles.cpp \
		paintsquiggles/squiggleswidget.cpp \
		paintstripes/paintstripes.cpp \
		paintstripes/stripeswidget.cpp \
		quasiperiodic_paintstripes/quasiperiodic_paintstripes.cpp \
		quasiperiodic_paintstripes/quasistripeswidget.cpp
}

QMAKE_CXXFLAGS += -ffast-math

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

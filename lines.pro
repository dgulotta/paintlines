TEMPLATE	= app
LANGUAGE	= C++

CONFIG += qt warn_on release link_pkgconfig c++14

QT += widgets

HEADERS += $$files(*.hpp, true)
SOURCES += $$files(*.cpp, true)

INCLUDEPATH += $$PWD/src

packagesExist(GraphicsMagick++) {
	PKGCONFIG += GraphicsMagick++
	DEFINES += MULTIPAGE
} else:packagesExist(Magick++) {
	PKGCONFIG += Magick++
	DEFINES += MULTIPAGE
}

packagesExist(luajit) {
	PKGCONFIG += luajit
	DEFINES += LUARULES
} else: packagesExist(lua) {
	PKGCONFIG += lua
	DEFINES += LUARULES
}

packagesExist(fftw3) {
	PKGCONFIG += fftw3
	DEFINES += FFTWPROGS
}

gcc|clang {
	QMAKE_CXXFLAGS += -ffast-math -fopenmp
	QMAKE_LFLAGS += -fopenmp
}

linux-clang {
	system(clang++ --version | grep 'version.3.[0-8]') {
		DEFINES += _OPENMP=201107
		QMAKE_CXXFLAGS += -Wno-macro-redefined
	}
}

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

msvc {
	DEFINES += _USE_MATH_DEFINES
	QMAKE_CXXFLAGS += /fp:fast /openmp
}

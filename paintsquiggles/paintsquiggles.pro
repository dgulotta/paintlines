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
	squigglesform.h

SOURCES	+= paintsquiggles.cpp \
	paintsquiggleswidget.cpp \
	../painter.cpp \
	../painterwidget.cpp \
	main.cpp \
	squigglesform.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


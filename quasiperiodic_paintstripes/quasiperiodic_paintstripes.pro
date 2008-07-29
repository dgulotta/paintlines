TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -lfftw3

HEADERS	+= quasiperiodic_paintstripes.h \
	quasiperiodic_paintstripeswidget.h \
	../painter.h \
	../painterwidget.h

SOURCES	+= quasiperiodic_paintstripes.cpp \
	quasiperiodic_paintstripeswidget.cpp \
	../painter.cpp \
	../painterwidget.cpp \
	main.cpp

FORMS	= quasistripesform.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}




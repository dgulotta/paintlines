TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -lfftw3

HEADERS	+= paintstripes.h \
	paintstripeswidget.h \
	../painter.h \
	../painterwidget.h

SOURCES	+= paintstripes.cpp \
	paintstripeswidget.cpp \
	../painter.cpp \
	../painterwidget.cpp \
	main.cpp

FORMS	= stripesform.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}




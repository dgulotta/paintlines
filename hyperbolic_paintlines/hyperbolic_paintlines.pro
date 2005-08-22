TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS	+= ../hyperbolic_painter.h \
	hyperbolic_paintlines.h \
	hyperbolic_paintlineswidget.h

SOURCES	+= ../hyperbolic_painter.cpp \
	hyperbolic_paintlines.cpp \
	hyperbolic_paintlineswidget.cpp \
	main.cpp

FORMS	= hyperboliclinesform.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

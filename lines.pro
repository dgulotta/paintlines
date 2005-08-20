TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS	+= paintlines.h \
	hyperbolic_painter.h \
	hyperbolic_paintlines.h \
	hyperbolic_paintlineswidget.h \
	paintlineswidget.h \
	painter.h \

SOURCES	+= paintlines.cpp \
	hyperbolic_painter.cpp \
	hyperbolic_paintlines.cpp \
	hyperbolic_paintlineswidget.cpp \
	main.cpp \
	paintlineswidget.cpp \
	painter.cpp \

FORMS	= paintform.ui \
	hyperboliclinesform.ui

IMAGES	= images/filenew \
	images/fileopen \
	images/filesave \
	images/print \
	images/undo \
	images/redo \
	images/editcut \
	images/editcopy \
	images/editpaste \
	images/searchfind
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

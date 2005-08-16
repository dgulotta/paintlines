TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS	+= paintlines.h \
	paintlines_hyperbolic.h \
	paintlineswidget.h \
	painter.h \
	paintclouds.h

SOURCES	+= paintlines.cpp \
	paintlines_hyperbolic.cpp \
	main.cpp \
	paintlineswidget.cpp \
	painter.cpp \
	paintclouds.cpp

FORMS	= paintform.ui

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


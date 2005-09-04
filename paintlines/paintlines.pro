TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS	+= paintlines.h \
	paintlineswidget.h \
	../painter.h \
	../painterwidget.h

SOURCES	+= paintlines.cpp \
	main.cpp \
	paintlineswidget.cpp \
	../painter.cpp \
	../painterwidget.cpp

FORMS	= paintform.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

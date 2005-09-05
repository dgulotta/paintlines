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

IMAGES	= images/editcopy \
	images/editcut \
	images/editpaste \
	images/filenew \
	images/fileopen \
	images/filesave \
	images/print \
	images/redo \
	images/searchfind \
	images/undo

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}




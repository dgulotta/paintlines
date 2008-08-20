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

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3	= paintform.ui

QT += QT3_SUPPORT

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3


TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release link_pkgconfig

PKGCONFIG += fftw3

HEADERS	+= paintstripes.h \
	paintstripeswidget.h \
	../painter.h \
	../painterwidget.h \
	../basic_painter.h \
	../basic_painterwidget.h

SOURCES	+= paintstripes.cpp \
	paintstripeswidget.cpp \
	../painter.cpp \
	../painterwidget.cpp \
	main.cpp

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3	= stripesform.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3


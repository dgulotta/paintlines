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

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3	= quasistripesform.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3


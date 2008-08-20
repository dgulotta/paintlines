TEMPLATE        = app
LANGUAGE        = C++

CONFIG  += qt warn_on release

HEADERS += paintclouds.h \
        paintcloudswidget.h \
        ../painter.h \
        ../painterwidget.h \
	../basic_painter.h \
	../basic_painterwidget.h

SOURCES += paintclouds.cpp \
        paintcloudswidget.cpp \
        ../painter.cpp \
        ../painterwidget.cpp \
        main.cpp

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3   = cloudsform.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3


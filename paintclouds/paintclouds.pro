TEMPLATE        = app
LANGUAGE        = C++

CONFIG  += qt warn_on release

HEADERS += paintclouds.h \
        paintcloudswidget.h \
        ../painter.h \
        ../painterwidget.h \
	../basic_painter.h \
	../basic_painterwidget.h \
	cloudsform.h

SOURCES += paintclouds.cpp \
        paintcloudswidget.cpp \
        ../painter.cpp \
        ../painterwidget.cpp \
        main.cpp \
	cloudsform.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


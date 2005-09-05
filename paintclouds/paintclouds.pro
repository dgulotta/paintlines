TEMPLATE        = app
LANGUAGE        = C++

CONFIG  += qt warn_on release

HEADERS += paintclouds.h \
        paintcloudswidget.h \
        ../painter.h \
        ../painterwidget.h

SOURCES += paintclouds.cpp \
        paintcloudswidget.cpp \
        ../painter.cpp \
        ../painterwidget.cpp \
        main.cpp

FORMS   = cloudsform.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

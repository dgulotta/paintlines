#include <QApplication>
#include "cloudsform.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    CloudsForm w;
    w.show();
    return a.exec();
}

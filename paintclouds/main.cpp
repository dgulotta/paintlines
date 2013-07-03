#include <QApplication>
#include "cloudsform.h"
#include "../randgen.h"

int main( int argc, char ** argv )
{
	seed();
    QApplication a( argc, argv );
    CloudsForm w;
    w.show();
    return a.exec();
}

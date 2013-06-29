#include <QApplication>
#include "stripesform.h"
#include "../randgen.h"

int main( int argc, char ** argv )
{
	seed();
    QApplication a( argc, argv );
    StripesForm w;
    w.show();
    return a.exec();
}

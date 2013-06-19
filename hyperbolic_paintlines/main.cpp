#include <QApplication>
#include <cstdlib>
#include <ctime>
#include "hyperboliclinesform.h"
#include "../randgen.h"

int main( int argc, char ** argv )
{
	seed();
    QApplication a( argc, argv );
    HyperbolicLinesForm w;
    w.show();
    return a.exec();
}

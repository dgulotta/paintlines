#include <QApplication>
#include "linesform.h"
#include "../randgen.h"

int main( int argc, char ** argv )
{
	seed();
    QApplication a( argc, argv );
    LinesForm w;
    w.show();
    return a.exec();
}

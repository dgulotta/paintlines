#include <QApplication>
#include "squigglesform.h"
#include "../randgen.h"

int main( int argc, char ** argv )
{
	seed();
    QApplication a( argc, argv );
    SquigglesForm w;
    w.show();
    return a.exec();
}

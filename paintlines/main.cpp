#include <QApplication>
#include "linesform.h"
#include "../randgen.h"
#include <Magick++.h>

int main( int argc, char ** argv )
{
	seed();
	Magick::InitializeMagick(nullptr);
    QApplication a( argc, argv );
    LinesForm w;
    w.show();
    return a.exec();
}

#include <QApplication>
#include "squigglesform.h"
#include "../randgen.h"
#include <Magick++.h>

int main( int argc, char ** argv )
{
	seed();
	Magick::InitializeMagick(nullptr);
    QApplication a( argc, argv );
    SquigglesForm w;
    w.show();
    return a.exec();
}

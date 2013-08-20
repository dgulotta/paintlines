#include <QApplication>
#include "linesform.h"
#include "../randgen.h"
#ifdef MULTIPAGE
#include <Magick++.h>
#endif

int main( int argc, char ** argv )
{
	seed();
#ifdef MULTIPAGE
	Magick::InitializeMagick(nullptr);
#endif
    QApplication a( argc, argv );
    LinesForm w;
    w.show();
    return a.exec();
}

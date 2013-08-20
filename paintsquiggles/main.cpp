#include <QApplication>
#include "squigglesform.h"
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
    SquigglesForm w;
    w.show();
    return a.exec();
}

#include <qapplication.h>
#include <stdlib.h>
#include <time.h>
#include "hyperboliclinesform.h"

int main( int argc, char ** argv )
{
    srand(time(0));
    QApplication a( argc, argv );
    HyperbolicLinesForm w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}

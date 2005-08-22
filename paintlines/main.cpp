#include <qapplication.h>
#include <stdlib.h>
#include <time.h>
#include "paintform.h"

int main( int argc, char ** argv )
{
    srand(time(0));
    QApplication a( argc, argv );
    PaintForm w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}

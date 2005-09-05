#include <qapplication.h>
#include "stripesform.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    StripesForm w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}

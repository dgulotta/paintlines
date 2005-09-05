#include <qapplication.h>
#include "cloudsform.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    CloudsForm w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}

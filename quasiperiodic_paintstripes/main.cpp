#include <qapplication.h>
#include "quasistripesform.h"
#include <cstdlib>
#include <ctime>

int main( int argc, char ** argv )
{
    std::srand(std::time(0));
    QApplication a( argc, argv );
    QuasiStripesForm w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}

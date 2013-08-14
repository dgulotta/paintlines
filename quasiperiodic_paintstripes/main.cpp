#include <QApplication>
#include "quasistripesform.h"
#include "../randgen.h"

int main( int argc, char ** argv )
{
    seed();
    QApplication a( argc, argv );
    QuasiStripesForm w;
    w.show();
    return a.exec();
}

#include <QApplication>
#include <cstdlib>
#include <ctime>
#include "hyperboliclinesform.h"

int main( int argc, char ** argv )
{
    std::srand(std::time(0));
    QApplication a( argc, argv );
    HyperbolicLinesForm w;
    w.show();
    return a.exec();
}

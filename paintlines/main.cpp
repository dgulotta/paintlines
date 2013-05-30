#include <QApplication>
#include "linesform.h"
#include <cstdlib>
#include <ctime>

int main( int argc, char ** argv )
{
    std::srand(std::time(0));
    QApplication a( argc, argv );
    LinesForm w;
    w.show();
    return a.exec();
}

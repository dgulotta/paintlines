#include <QApplication>
#include "stripesform.h"
#include <cstdlib>
#include <ctime>

int main( int argc, char ** argv )
{
    std::srand(std::time(0));
    QApplication a( argc, argv );
    StripesForm w;
    w.show();
    return a.exec();
}
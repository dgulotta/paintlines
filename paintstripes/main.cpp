#include <QApplication>
#include "stripesform.h"
#include "../randgen.h"
#include <cstdlib>
#include <ctime>

int main( int argc, char ** argv )
{
    std::srand(std::time(0));
	seed();
    QApplication a( argc, argv );
    StripesForm w;
    w.show();
    return a.exec();
}

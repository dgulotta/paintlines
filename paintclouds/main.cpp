#include <QApplication>
#include "cloudsform.h"
#include <cstdlib>
#include <ctime>

int main( int argc, char ** argv )
{
    std::srand(std::time(0));
    QApplication a( argc, argv );
    CloudsForm w;
    w.show();
    return a.exec();
}
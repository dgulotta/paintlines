#include <QApplication>
#include "converterform.h"
#include "../randgen.h"

int main( int argc, char ** argv )
{
	seed();
    QApplication a( argc, argv );
    ConverterForm w;
    w.show();
    return a.exec();
}

#include <QApplication>
#include "mainform.h"
#ifdef MULTIPAGE
#include <Magick++.h>
#endif

int main( int argc, char ** argv )
{
#ifdef MULTIPAGE
	Magick::InitializeMagick(nullptr);
#endif
    QApplication a( argc, argv );
    MainForm w;
    w.show();
    return a.exec();
}

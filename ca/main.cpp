#include <QtGui>
#include "caform.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	CAForm w;
	w.show();
	return a.exec();
}

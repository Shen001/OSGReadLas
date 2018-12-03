#include "osgreadlas.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OSGReadLas w;
	w.show();
	return a.exec();
}

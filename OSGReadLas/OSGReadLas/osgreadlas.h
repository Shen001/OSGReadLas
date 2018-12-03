#ifndef OSGREADLAS_H
#define OSGREADLAS_H

#include <QtWidgets/QMainWindow>
#include "ui_osgreadlas.h"

class OSGReadLas : public QMainWindow
{
	Q_OBJECT

public:
	OSGReadLas(QWidget *parent = 0);
	~OSGReadLas();

private:
	Ui::OSGReadLasClass ui;
};

#endif // OSGREADLAS_H

#ifndef OSGREADLAS_H
#define OSGREADLAS_H

#include <QtWidgets/QMainWindow>
#include "ui_osgreadlas.h"

#include "mainForm.h" 


class OSGReadLas : public QMainWindow
{
	Q_OBJECT

public:
	OSGReadLas(QWidget *parent = 0);
	~OSGReadLas();

	main_form* osg_widget;
	QAction* open_action;
	QAction* quit_action;
	QMenu* file_menu;
	void create_tool_bars();
	QToolBar* file_tool_bar;


	public slots:
	void open_files();
private:
	Ui::OSGReadLasClass ui;
};

#endif // OSGREADLAS_H

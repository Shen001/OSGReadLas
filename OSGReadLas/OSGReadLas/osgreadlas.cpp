#include "osgreadlas.h"
#include <QFileDialog>

OSGReadLas::OSGReadLas(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	osg_widget = new main_form;

	open_action = new QAction(QString::fromLocal8Bit("打开"), this);
	open_action->setIcon(QIcon("Resources/open.png"));
	open_action->setStatusTip(QString::fromLocal8Bit("打开三维模型"));
	connect(open_action, SIGNAL(triggered()), this, SLOT(open_files()));
	quit_action = new QAction(QString::fromLocal8Bit("退出"), this);
	quit_action->setIcon(QIcon("Resources/mActionFileExit.png"));
	connect(quit_action, SIGNAL(triggered()), this, SLOT(close()));


	file_menu = menuBar()->addMenu(QString::fromLocal8Bit("文件"));
	file_menu->addAction(open_action);
	file_menu->addAction(quit_action);
	create_tool_bars();
	resize(800, 600);
	setCentralWidget(osg_widget);
}

OSGReadLas::~OSGReadLas()
{

}

void OSGReadLas::open_files()
{
	QString node_name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择文件"), QDir::currentPath(), QString::fromLocal8Bit("点云(*.las);;OpenSceneGraph(*.osg);;IVE(*.ive);;所有文件(*.*)"));
	if (!node_name.isEmpty())
	{
		osg_widget->add_node(node_name);
	}
	osg_widget->getView(0)->getCameraManipulator()->home(0);
}

void OSGReadLas::create_tool_bars()
{
	file_tool_bar = addToolBar(QString::fromLocal8Bit("&T工具栏"));
	file_tool_bar->addAction(open_action);
	file_tool_bar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	addToolBar(file_tool_bar);
}


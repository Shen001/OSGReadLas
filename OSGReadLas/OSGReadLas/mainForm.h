#pragma once 
#include "GraphicsWindowQt.h" 
#include <osgViewer/CompositeViewer> 
#include <osgViewer/Viewer> 
#include <QtCore/QTimer> 
#include <QGridLayout> 
#include <QtGui\QtGui> 
#include <qmessagebox.h>

#include <osgGA/StateSetManipulator> 
#include <osgGA/KeySwitchMatrixManipulator> 
#include <osgGA/TrackballManipulator> 
#include <osgGA/TrackballManipulator> 
#include <osgViewer/ViewerEventHandlers> 
#include <osgDB/ReadFile> 
#include <osg/MatrixTransform> 

#include "lasPaser.h" 

class main_form : public QWidget, public osgViewer::CompositeViewer
{
public:
	main_form();
	~main_form();

	FILE* file_in;
	int p_count;
	lasreader *lasread;
	osg::ref_ptr< osg::Vec3Array> point_array;
	osg::ref_ptr<osg::Geometry> geometry;
	osg::ref_ptr<osg::Geode> geode;
	osg::ref_ptr<osgViewer::Viewer> get_viewer();
	osg::ref_ptr<osg::Vec4Array> color;
	osg::ref_ptr<osg::MatrixTransform> mt;
	float boundingBoxMin[3];
	float boundingBoxMax[3];
	float boundingBoxHeight;
	float boundingBoxScale;
	float boundingBoxTranslateX;
	float boundingBoxTranslateY;
	float boundingBoxTranslateZ;

	QWidget* add_view_widget(osg::Camera* camera);

	osg::Camera* create_camera(int x, int y, int w, int h, const std::string& name = "", bool window_decoration = false);

	//理新控件中的内容 
	void paintEvent(QPaintEvent* event);
	osg::Vec4f init_color(unsigned  r, unsigned  g, unsigned b, unsigned a);
	osg::ref_ptr<osgViewer::Viewer> view;
	void add_node(const QString node_name);
protected:
	QTimer _timer;

};
#include "mainForm.h"   

#include "globalValue.h"

extern osg::ref_ptr<osg::Switch> root_all;
extern osg::ref_ptr<osg::Switch> root_one;
static osg::Vec4f Colors[10] = { osg::Vec4f(0.0, 0.0, 255.0 / 255.0, 1.0), osg::Vec4f(0.0, 0.0, 127 / 255, 1.0), osg::Vec4f(0, 127 / 255, 127 / 255, 1.0), osg::Vec4f(127 / 255, 0, 127 / 255, 1.0), osg::Vec4f(127 / 255, 127 / 255, 0, 1.0),
osg::Vec4f(127 / 255, 0, 0, 1.0), osg::Vec4f(255 / 255, 0, 0, 1.0), osg::Vec4f(255 / 255, 255 / 255, 0, 1.0), osg::Vec4f(255 / 255, 0, 255 / 255, 1.0), osg::Vec4f(255 / 255, 255 / 255, 255 / 255, 1.0)
};

main_form::main_form() : QWidget()
{
	setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
	root_all->addChild(root_one);
	QWidget* widget_one = add_view_widget(create_camera(0, 0, 800, 800));
	QGridLayout* grid = new QGridLayout;
	grid->addWidget(widget_one);
	setLayout(grid);
	connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
	_timer.start(10);
	p_count = 0;
	point_array = new osg::Vec3Array;
	geometry = new osg::Geometry;
	file_in = 0;
	lasread = 0;
	geode = new osg::Geode;
	color = new osg::Vec4Array;
	mt = new osg::MatrixTransform;

	if (lasread == 0)
		lasread = new lasreader();

	for (int i = 0; i < 3; i++) boundingBoxMin[i] = 0.0f;

	for (int i = 0; i < 3; i++) boundingBoxMax[i] = 0.0f;

	boundingBoxHeight = 0.0f;
	boundingBoxTranslateX = 0.0f;
	boundingBoxTranslateY = 0.0f;
	boundingBoxTranslateZ = 0.0f;
	boundingBoxScale = 0.0f;
}

QWidget* main_form::add_view_widget(osg::Camera* camera)
{
	osgViewer::Viewer* view = new osgViewer::Viewer;
	view->addEventHandler(new osgGA::StateSetManipulator(camera->getOrCreateStateSet()));
	view->setCamera(camera);
	root_all->getOrCreateStateSet()->setMode(GL_LIGHT_MODEL_TWO_SIDE, osg::StateAttribute::OFF);
	root_all->setDataVariance(osg::Object::DYNAMIC);
	addView(view);
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> key_switch_manipulator = new osgGA::KeySwitchMatrixManipulator();
	key_switch_manipulator->addMatrixManipulator('1', "Trackball", new osgGA::TrackballManipulator());
	key_switch_manipulator->addMatrixManipulator('2', "Terrainball", new osgGA::TrackballManipulator());
	view->setSceneData(root_all);
	view->addEventHandler(new osgViewer::StatsHandler);
	view->setCameraManipulator(key_switch_manipulator);
	GraphicsWindowQt* gw = dynamic_cast<GraphicsWindowQt*>(camera->getGraphicsContext());
	return gw ? gw->getGraphWidget() : NULL;
}

osg::Camera* main_form::create_camera(int x, int y, int w, int h, const std::string& name, bool window_decoration)
{
	osg::DisplaySettings* ds = osg::DisplaySettings::instance();
	ds->setNumMultiSamples(10);
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->windowName = name;
	traits->windowDecoration = window_decoration;
	traits->x = x;
	traits->y = y;
	traits->width = w;
	traits->height = h;
	traits->doubleBuffer = true;
	traits->alpha = ds->getMinimumNumAlphaBits();
	traits->stencil = ds->getMinimumNumStencilBits();
	traits->sampleBuffers = ds->getMultiSamples();
	traits->samples = ds->getNumMultiSamples();
	osgViewer::Viewer* view = new osgViewer::Viewer;
	osg::ref_ptr<osg::Camera> camera = view->getCamera();
	camera->setGraphicsContext(new GraphicsWindowQt(traits));
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setClearColor(init_color(45, 45, 90, 225));
	camera->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);
	return camera.release();
}

osg::Vec4f main_form::init_color(unsigned r, unsigned g, unsigned b, unsigned a)
{
	float div = 1.f / 256.f;
	return osg::Vec4f(div * (float)r, div * (float)g, div * (float)b, div * (float)a);
}

void main_form::paintEvent(QPaintEvent* event)
{
	frame();
}

void main_form::add_node(QString node_name)
{
	std::string str = node_name.toStdString();
	const char* lasSource = str.c_str();
	file_in = fopen(lasSource, "rb");

	if ((lasread->open(file_in) == false) || file_in == 0)
	{
		QMessageBox message(QMessageBox::Warning, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("读取数据失败！"));
		message.setIconPixmap(QPixmap(":resource/2.bmp"));
		message.exec();
		return;
	}

	boundingBoxMin[0] = lasread->header.min_x;
	boundingBoxMin[1] = lasread->header.min_y;
	boundingBoxMin[2] = lasread->header.min_z;
	boundingBoxMax[0] = lasread->header.max_x;
	boundingBoxMax[1] = lasread->header.max_y;
	boundingBoxMax[2] = lasread->header.max_z;
	boundingBoxHeight = boundingBoxMax[2] - boundingBoxMin[2];
	if ((boundingBoxMax[1] - boundingBoxMin[1]) > (boundingBoxMax[0] - boundingBoxMin[0]))
	{
		if ((boundingBoxMax[1] - boundingBoxMin[1]) > (boundingBoxMax[2] - boundingBoxMin[2]))
		{
			boundingBoxScale = 1.0f / (boundingBoxMax[1] - boundingBoxMin[1]);
		}
		else
		{
			boundingBoxScale = 1.0f / (boundingBoxMax[2] - boundingBoxMin[2]);
		}
	}
	else
	{
		if ((boundingBoxMax[0] - boundingBoxMin[0]) > (boundingBoxMax[2] - boundingBoxMin[2]))
		{
			boundingBoxScale = 1.0f / (boundingBoxMax[0] - boundingBoxMin[0]);
		}
		else
		{
			boundingBoxScale = 1.0f / (boundingBoxMax[2] - boundingBoxMin[2]);
		}
	}

	boundingBoxTranslateX = -boundingBoxScale * (boundingBoxMin[0] + 0.5f * (boundingBoxMax[0] - boundingBoxMin[0]));
	boundingBoxTranslateY = -boundingBoxScale * (boundingBoxMin[1] + 0.5f * (boundingBoxMax[1] - boundingBoxMin[1]));
	boundingBoxTranslateZ = -boundingBoxScale * (boundingBoxMin[2] + 0.5f * (boundingBoxMax[2] - boundingBoxMin[2]));

	p_count = lasread->npoints;
	LASpt pt;
	double point[3];
	lasread->open_skipheader();

	while (lasread->p_count < p_count)
	{
		if (lasread->read_point(&pt))
		{
			point[0] = pt.x;
			point[1] = pt.y;
			point[2] = pt.z;
			point_array->push_back(osg::Vec3(point[0], point[1], point[2]));
			int t = int((point[2] - boundingBoxMin[2]) * 10 / boundingBoxHeight);
			color->push_back(osg::Vec4(Colors[t]));
			geometry->setColorArray(color);
			geometry->setVertexArray(point_array);
		}
	}

	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, point_array->size()));
	geode->addDrawable(geometry);
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, false);
	mt->addChild(geode);
	mt->setMatrix(osg::Matrixd::translate(osg::Vec3(boundingBoxTranslateX, boundingBoxTranslateY, boundingBoxTranslateZ)));
	root_one->addChild(mt);
}

main_form::~main_form()
{
}
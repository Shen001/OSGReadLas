#ifndef OSGVIEWER_GRAPHICSWINDOWQT 
#define OSGVIEWER_GRAPHICSWINDOWQT 

#include <QGLWidget> 
#include <QtGui/QInputEvent> 
#include <QtOpenGL/QGLWidget> 
#include <osgViewer/GraphicsWindow> 

class GraphWidget : public QGLWidget
{
public:
	GraphWidget(const QGLFormat& format, QWidget* parent = 0, const QGLWidget* shareWidget = 0, Qt::WindowFlags f = 0);

	inline void setGraphicsWindow(osgViewer::GraphicsWindow* gw) { _gw = gw; }

	void setKeyboardModifiers(QInputEvent* event);

	virtual void resizeEvent(QResizeEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

protected:
	osgViewer::GraphicsWindow* _gw;
};

class GraphicsWindowQt : public osgViewer::GraphicsWindow
{
public:
	GraphicsWindowQt(osg::GraphicsContext::Traits* traits);
	virtual ~GraphicsWindowQt();

	inline GraphWidget* getGraphWidget() { return _widget; }
	inline const GraphWidget* getGraphWidget() const { return _widget; }

	struct WindowData : public osg::Referenced
	{
		WindowData(GraphWidget* widget) : _widget(widget) {}
		GraphWidget* _widget;
	};

	bool init();

	virtual bool setWindowRectangleImplementation(int x, int y, int width, int height);
	virtual void getWindowRectangle(int& x, int& y, int& width, int& height);
	virtual bool setWindowDecorationImplementation(bool windowDecoration);
	virtual bool getWindowDecoration() const;
	virtual void grabFocus();
	virtual void grabFocusIfPointerInWindow();
	virtual void raiseWindow();
	virtual void setWindowName(const std::string& name);
	virtual std::string getWindowName();
	virtual void useCursor(bool cursorOn);
	virtual void setCursor(MouseCursor cursor);

	virtual bool valid() const;
	virtual bool realizeImplementation();
	virtual bool isRealizedImplementation() const;
	virtual void closeImplementation();
	virtual bool makeCurrentImplementation();
	virtual bool releaseContextImplementation();
	virtual void swapBuffersImplementation();

	virtual void requestWarpPointer(float x, float y);

protected:
	GraphWidget* _widget;
	QCursor _currentCursor;
	bool _initialized;
	bool _realized;
};

#endif 
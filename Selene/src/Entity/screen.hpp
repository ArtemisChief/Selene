#pragma once
#include <QVector2D>;
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RESOLUTION_X 640
#define RESOLUTION_Y 480

class Screen {

public:

	Screen(QOpenGLContext *context);
	~Screen();

	void DrawGrids();

private:

	const int grid_counts_ = RESOLUTION_X * RESOLUTION_Y;

	// OpenGL函数
	QOpenGLExtraFunctions *gl_functions_;

	// 顶点缓存对象
	QOpenGLVertexArrayObject vao_;
	QOpenGLBuffer ebo_;
	QOpenGLBuffer color_vbo_;

	cv::VideoCapture *capture_;
	cv::Mat current_mat_;

};

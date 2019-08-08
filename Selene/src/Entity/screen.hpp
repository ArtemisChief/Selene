#pragma once
#include <QVector2D>;
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RESOLUTION_X 640
#define RESOLUTION_Y 480

class Screen {

public:

	Screen(QOpenGLContext *context, QOpenGLShaderProgram *shader_program);
	~Screen();

	void SetIsPaused(const bool pause);
	bool GetIsPaused() const;
	void DrawGrids();
	void ShowGridLine(const bool show_grid_line);

private:

	const int grid_counts_ = RESOLUTION_X * RESOLUTION_Y;

	// OpenGL函数
	QOpenGLExtraFunctions *gl_functions_;

	// 着色器程序
	QOpenGLShaderProgram *shader_program_;

	// 顶点缓存对象
	QOpenGLVertexArrayObject grid_vao_;
	QOpenGLBuffer grid_color_vbo_;

	// 视频
	cv::VideoCapture *capture_;
	cv::Mat current_mat_;

	// 是否暂停
	bool is_paused_;

	// 着色器Uniform位置
	int uniform_is_border_color_location_;

	// 网格线
	bool show_grid_line_;
};

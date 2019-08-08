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

	// OpenGL����
	QOpenGLExtraFunctions *gl_functions_;

	// ��ɫ������
	QOpenGLShaderProgram *shader_program_;

	// ���㻺�����
	QOpenGLVertexArrayObject grid_vao_;
	QOpenGLBuffer grid_color_vbo_;

	// ��Ƶ
	cv::VideoCapture *capture_;
	cv::Mat current_mat_;

	// �Ƿ���ͣ
	bool is_paused_;

	// ��ɫ��Uniformλ��
	int uniform_is_border_color_location_;

	// ������
	bool show_grid_line_;
};

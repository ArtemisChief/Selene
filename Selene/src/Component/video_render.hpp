#pragma once
#include <QObject>
#include <QVector2D>;
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RESOLUTION_X 1440
#define RESOLUTION_Y 1080

class VideoRender : public QObject {

	Q_OBJECT

public:

	static VideoRender* CreateInstance(QOpenGLContext *context, QOpenGLShaderProgram *shader_program);
	static VideoRender* GetInstance();

	void DrawGrids();

	void ShowGridLine(const bool show_grid_line);
	void SetIsPaused();

private:

	//���캯��
	VideoRender(QOpenGLContext *context, QOpenGLShaderProgram *shader_program);

	// ����
	static VideoRender* video_render_;

	// Grid����
	static const int GRID_COUNTS = RESOLUTION_X * RESOLUTION_Y;

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

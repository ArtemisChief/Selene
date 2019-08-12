#pragma once
#include <QThread>
#include <QKeyEvent>
#include <QOpenGLWindow>
#include "src/Shader/glsl.hpp"
#include "video_render.hpp"

#define FPS 60

class GLWindow : public QOpenGLWindow {

	Q_OBJECT

public:

	GLWindow();
	~GLWindow();

	void ProcessInput();

protected:

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void keyPressEvent(QKeyEvent *key_event) override;
	void keyReleaseEvent(QKeyEvent*key_event) override;

private:

	// 着色器程序
	QOpenGLShaderProgram *shader_program_;

	// 着色器Uniform位置
	int uniform_camera_location_;
	int uniform_projection_location_;

	// 摄像机矩阵
	QMatrix4x4 m_camera_;
	// 透视矩阵
	QMatrix4x4 m_projection_;

	// 缩放因子
	int zoom_;

	// 摄像机移动位置
	int camera_x_;
	int camera_y_;

	// Uniform是否已写入值
	bool is_uniforms_dirty_;

	// 按键
	bool is_key_w_pressed_;
	bool is_key_a_pressed_;
	bool is_key_s_pressed_;
	bool is_key_d_pressed_;
	bool is_key_q_pressed_;
	bool is_key_e_pressed_;

};

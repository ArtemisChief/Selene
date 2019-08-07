#pragma once
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include "src/Entity/screen.hpp"

class GLWindow : public QOpenGLWindow {

	Q_OBJECT

public:

	GLWindow();
	~GLWindow();

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

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

	// Uniform是否已写入值
	bool is_uniforms_dirty_;

	// 屏幕
	Screen *screen_;

};

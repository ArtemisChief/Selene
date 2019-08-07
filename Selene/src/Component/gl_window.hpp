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

	// ��ɫ������
	QOpenGLShaderProgram *shader_program_;

	// ��ɫ��Uniformλ��
	int uniform_camera_location_;
	int uniform_projection_location_;

	// ���������
	QMatrix4x4 m_camera_;
	// ͸�Ӿ���
	QMatrix4x4 m_projection_;

	// Uniform�Ƿ���д��ֵ
	bool is_uniforms_dirty_;

	// ��Ļ
	Screen *screen_;

};

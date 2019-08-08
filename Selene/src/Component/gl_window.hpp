#pragma once
#include <iostream>
#include <QOpenGLWindow>
#include <QKeyEvent>
#include <QtConcurrent/QtConcurrent>

#include "src/Shader/glsl.hpp"
#include "src/Entity/screen.hpp"

#define FPS 60

class GLWindow : public QOpenGLWindow {

	Q_OBJECT

public:

	GLWindow();
	~GLWindow();

	void ProcessInput();

	void CalibrateFPS();

protected:

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void keyPressEvent(QKeyEvent *key_event) override;
	void keyReleaseEvent(QKeyEvent*key_event) override;

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

	// ��������
	int zoom_;

	// ������ƶ�λ��
	int camera_x_;
	int camera_y_;

	// Uniform�Ƿ���д��ֵ
	bool is_uniforms_dirty_;

	// ��Ļ
	Screen *screen_;

	// ����
	bool is_key_w_pressed_;
	bool is_key_a_pressed_;
	bool is_key_s_pressed_;
	bool is_key_d_pressed_;
	bool is_key_q_pressed_;
	bool is_key_e_pressed_;

	// ֹͣFPSУ׼�߳�
	bool should_close_;

signals:
	void fps_time_out();

};

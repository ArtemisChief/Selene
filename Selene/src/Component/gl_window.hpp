#pragma once
#include <QOpenGLWindow>
#include <QMatrix4x4>

QT_BEGIN_NAMESPACE
class QOpenGLTexture;
class QOpenGLShaderProgram;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;
QT_END_NAMESPACE

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

	// ���㻺�����
	QOpenGLVertexArrayObject *vao_;
	QOpenGLBuffer *vbo_;
	QOpenGLBuffer *ebo_;
	QOpenGLBuffer *instance_vbo_;

	// ��ɫ��Uniformλ��
	int uniform_camera_location_;
	int uniform_projection_location_;
	int uniform_color_location_;

	// ���������
	QMatrix4x4 m_camera_;
	// ͸�Ӿ���
	QMatrix4x4 m_projection_;

	// Uniform�Ƿ���д��ֵ
	bool is_uniforms_dirty_;

private slots:


};
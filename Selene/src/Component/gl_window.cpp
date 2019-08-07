#include "gl_window.hpp"
#include "src/Shader/glsl.hpp"

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include <iostream>
#include "src/Entity/screen.hpp"

GLWindow::GLWindow() : shader_program_(nullptr), uniform_camera_location_(0), uniform_projection_location_(0),
                       is_uniforms_dirty_(true), screen_(nullptr)
{
	m_camera_.setToIdentity();
	m_projection_.setToIdentity();
}

GLWindow::~GLWindow() {
	makeCurrent();
	delete shader_program_;
}

void GLWindow::initializeGL() {

	screen_ = new Screen(QOpenGLContext::currentContext());

	// 使用QOpenGL的ExtraFunctions而不是QOpenGLFunctions，以启用OpenGL3.0以上版本的功能
	auto gl_functions = QOpenGLContext::currentContext()->extraFunctions();

	// 着色器程序
	shader_program_ = new QOpenGLShaderProgram;

	// 链接着色器
	shader_program_->addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader_source);
	shader_program_->addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader_source);
	shader_program_->link();

	// 获得Uniform位置
	uniform_camera_location_ = shader_program_->uniformLocation("cameraMatrix");
	uniform_projection_location_ = shader_program_->uniformLocation("projectionMatrix");

	// 开启多重采样抗锯齿
	gl_functions->glEnable(GL_MULTISAMPLE);

}

void GLWindow::resizeGL(const int w, const int h) {
	m_projection_.setColumn(0, QVector4D(11.06f / w, 0, 0, 0));
	m_projection_.setColumn(1, QVector4D(0, 11.06f / h, 0, 0));
	m_projection_.setColumn(2, QVector4D(0, 0, 1, 0));
	m_projection_.setColumn(3, QVector4D(0, 0, 0, 1));
	is_uniforms_dirty_ = true;
}

void GLWindow::paintGL() {

	auto gl_extra_functions = QOpenGLContext::currentContext()->extraFunctions();

	gl_extra_functions->glClearColor(0, 0, 0, 1);
	gl_extra_functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	shader_program_->bind();

	if (is_uniforms_dirty_) {
		is_uniforms_dirty_ = false;
		shader_program_->setUniformValue(uniform_projection_location_, m_projection_);
		shader_program_->setUniformValue(uniform_camera_location_, m_camera_);
	}

	screen_->DrawGrids();

	update();
}
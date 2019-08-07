#include "gl_window.hpp"
#include "src/Shader/glsl.hpp"

#include <QTimer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <iostream>

GLWindow::GLWindow() : shader_program_(nullptr), vao_(nullptr), vbo_(nullptr), ebo_(nullptr), instance_vbo_(nullptr),
                       uniform_camera_location_(0), uniform_projection_location_(0), uniform_color_location_(0),
                       is_uniforms_dirty_(true) {
	m_camera_.setToIdentity();
	m_projection_.setToIdentity();
}

GLWindow::~GLWindow() {
	makeCurrent();
	delete shader_program_;
	delete vbo_;
	delete vao_;
}

void GLWindow::initializeGL() {

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
	//uniform_color_location_ = shader_program_->uniformLocation("color");

	// 开启多重采样抗锯齿
	gl_functions->glEnable(GL_MULTISAMPLE);

	float vertices[] = {
		 1.0f,  1.0f,  // top right
		 1.0f, -1.0f,  // bottom right
		-1.0f, -1.0f,  // bottom left
		-1.0f,  1.0f,  // top left  
	};

	unsigned int indices[] = {
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	QVector2D offsets[10800];
	auto index = 0;
	const auto offset = 1.0f;
	for (auto y = 945; y > -945; y -= 21) {
		for (auto x = -1260; x < 1260; x += 21) {
			QVector2D translation;
			translation.setX(static_cast<float>(x) / 10.0f + offset);
			translation.setY(static_cast<float>(y) / 10.0f - offset);
			offsets[index++] = translation;
		}
	}

	instance_vbo_= new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	instance_vbo_->create();
	instance_vbo_->bind();
	instance_vbo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
	instance_vbo_->allocate(offsets, sizeof(QVector2D) * 10800);
	instance_vbo_->release();

	vao_ = new QOpenGLVertexArrayObject;
	vao_->create();
	vao_->bind();

	vbo_ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vbo_->create();
	vbo_->bind();
	vbo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_->allocate(vertices, sizeof vertices);

	ebo_ = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	ebo_->create();
	ebo_->bind();
	ebo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
	ebo_->allocate(indices, sizeof indices);

	gl_functions->glEnableVertexAttribArray(0);
	gl_functions->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));
	
	gl_functions->glEnableVertexAttribArray(1);
	instance_vbo_->bind();
	gl_functions->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));
	instance_vbo_->release();
	gl_functions->glVertexAttribDivisor(1, 1);

	vbo_->release();
	vao_->release();
}

void GLWindow::resizeGL(const int w, const int h) {
	m_projection_.setColumn(0, QVector4D(10.8f / w, 0, 0, 0));
	m_projection_.setColumn(1, QVector4D(0, 10.8f / h, 0, 0));
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
		//shader_program_->setUniformValue(uniform_color_location_, QVector3D(1.0, 1.0, 1.0));
		shader_program_->setUniformValue(uniform_projection_location_, m_projection_);
		shader_program_->setUniformValue(uniform_camera_location_, m_camera_);
		std::cout << "uniforms is dirty" << std::endl;
	}

	vao_->bind();
	gl_extra_functions->glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, 10800);
	vao_->release();

	update();
}
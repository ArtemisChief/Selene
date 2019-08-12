#include "gl_window.hpp"
#include "src/Util/microsecond_timer.hpp"

GLWindow::GLWindow() : shader_program_(nullptr), uniform_camera_location_(0), uniform_projection_location_(0), zoom_(0),
                       camera_x_(0), camera_y_(0), is_uniforms_dirty_(true), is_key_w_pressed_(false), 
					   is_key_a_pressed_(false), is_key_s_pressed_(false), is_key_d_pressed_(false), is_key_q_pressed_(false),
                       is_key_e_pressed_(false) {

	m_camera_.setToIdentity();
	m_projection_.setToIdentity();
}

GLWindow::~GLWindow() {
	makeCurrent();
	delete shader_program_;
}

void GLWindow::ProcessInput() {

	if (is_key_w_pressed_) {
		if (zoom_ > 60)
			camera_y_ -= 1;
		else
			camera_y_ -= 5;
		m_camera_.setToIdentity();
		m_camera_.translate(camera_x_, camera_y_);
		is_uniforms_dirty_ = true;
	}
	if (is_key_a_pressed_) {
		if (zoom_ > 60)
			camera_x_ += 1;
		else
			camera_x_ += 5;
		m_camera_.setToIdentity();
		m_camera_.translate(camera_x_, camera_y_);
		is_uniforms_dirty_ = true;
	}
	if (is_key_s_pressed_) {
		if (zoom_ > 60)
			camera_y_ += 1;
		else
			camera_y_ += 5;
		m_camera_.setToIdentity();
		m_camera_.translate(camera_x_, camera_y_);
		is_uniforms_dirty_ = true;
	}
	if (is_key_d_pressed_) {
		if (zoom_ > 60)
			camera_x_ -= 1;
		else
			camera_x_ -= 5;
		m_camera_.setToIdentity();
		m_camera_.translate(camera_x_, camera_y_);
		is_uniforms_dirty_ = true;
	}

	if (is_key_e_pressed_) {
		if (zoom_ < 20)
			zoom_ += 2;
		else if (zoom_ < 40)
			zoom_ += 5;
		else if (zoom_ < 480)
			zoom_ += 10;
		resizeGL(1280, 960);
	}
	else if (is_key_q_pressed_) {
		if (zoom_ > 40)
			zoom_ -= 10;
		else if (zoom_ > 20)
			zoom_ -= 5;
		else if (zoom_ > 0)
			zoom_ -= 2;
		resizeGL(1280, 960);
	}
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

	// 开启多重采样抗锯齿
	gl_functions->glEnable(GL_MULTISAMPLE);

	// 初始化视频渲染器，获取OpenGL上下文
	VideoRender::CreateInstance(QOpenGLContext::currentContext(), shader_program_);

	// 设置计时器以校准帧率
	auto timer = new MicrosecondTimer;
	connect(timer, SIGNAL(TimeOut()), this, SLOT(update()));
	timer->Start(1000000 / FPS);
}

void GLWindow::resizeGL(const int w, const int h) {
	if (zoom_ > 20)
		VideoRender::GetInstance()->ShowGridLine(true);
	else
		VideoRender::GetInstance()->ShowGridLine(false);

	m_projection_.setColumn(0, QVector4D((1.778f + zoom_ / 10.0) / w, 0, 0, 0));
	m_projection_.setColumn(1, QVector4D(0, (1.778f + zoom_ / 10.0) / h, 0, 0));
	m_projection_.setColumn(2, QVector4D(0, 0, 1, 0));
	m_projection_.setColumn(3, QVector4D(0, 0, 0, 1));
	is_uniforms_dirty_ = true;
}

void GLWindow::paintGL() {
	ProcessInput();

	shader_program_->bind();

	if (is_uniforms_dirty_) {
		is_uniforms_dirty_ = false;
		shader_program_->setUniformValue(uniform_projection_location_, m_projection_);
		shader_program_->setUniformValue(uniform_camera_location_, m_camera_);
	}

	VideoRender::GetInstance()->DrawGrids();
}

void GLWindow::keyPressEvent(QKeyEvent *key_event) {
	const auto key = key_event->key();
	switch (key) {
	case Qt::Key_E:
		is_key_e_pressed_ = true;
		break;
	case Qt::Key_Q:
		is_key_q_pressed_ = true;
		break;
	case Qt::Key_W:
		is_key_w_pressed_ = true;
		break;
	case Qt::Key_A:
		is_key_a_pressed_ = true;
		break;
	case Qt::Key_S:
		is_key_s_pressed_ = true;
		break;
	case Qt::Key_D:
		is_key_d_pressed_ = true;
		break;
	case Qt::Key_Space:
		VideoRender::GetInstance()->SetIsPaused();
		break;
	default:
		break;
	}
}

void GLWindow::keyReleaseEvent(QKeyEvent* key_event) {
	const auto key = key_event->key();
	switch (key) {
	case Qt::Key_E:
		is_key_e_pressed_ = false;
		break;
	case Qt::Key_Q:
		is_key_q_pressed_ = false;
		break;
	case Qt::Key_W:
		is_key_w_pressed_ = false;
		break;
	case Qt::Key_A:
		is_key_a_pressed_ = false;
		break;
	case Qt::Key_S:
		is_key_s_pressed_ = false;
		break;
	case Qt::Key_D:
		is_key_d_pressed_ = false;
		break;
	default:
		break;
	}
}
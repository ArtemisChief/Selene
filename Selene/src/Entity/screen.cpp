#include "screen.hpp"

Screen::Screen(QOpenGLContext *context) {

	// 创建120*90的Grid矩阵
	for (auto y = 0; y < 90; y++) {
		for (auto x = 0; x < 120; x++) {
			grids_.append(Grid(x, y));
		}
	}

	// 获取OpenGL上下文
	context_ = context;

	// 设置Grid顶点坐标
	grid_vertices_ = new float[8] {
		1.0f,  1.0f,  // top right
		1.0f, -1.0f,  // bottom right
	   -1.0f, -1.0f,  // bottom left
	   -1.0f,  1.0f,  // top left  
	};

	// 设置Grid Indices
	grid_indices_ = new unsigned int[6] {
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	// 设置Grid实例化渲染中每一个Grid的偏移量
	grid_offsets_ = new QVector2D[10800];
	auto index = 0;
	const auto offset = 1.0f;
	for (auto y = 945; y > -945; y -= 21) {
		for (auto x = -1260; x < 1260; x += 21) {
			QVector2D translation;
			translation.setX(static_cast<float>(x) / 10.0f + offset);
			translation.setY(static_cast<float>(y) / 10.0f - offset);
			grid_offsets_[index++] = translation;
		}
	}

	// 设置偏移量Buffer
	offset_vbo_ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	offset_vbo_->create();
	offset_vbo_->bind();
	offset_vbo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
	offset_vbo_->allocate(grid_offsets_, sizeof(QVector2D) * 10800);
	offset_vbo_->release();

	// 设置Grid实例化渲染中每一个Grid的颜色
	grid_colors_ = new QVector3D[10800];
	index = 0;
	for (auto grid : grids_) {
		grid_colors_[index++] = grid.GetNormalizedRgb();
	}

	// 设置颜色Buffer
	color_vbo_ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	color_vbo_->create();
	color_vbo_->bind();
	color_vbo_->setUsagePattern(QOpenGLBuffer::StreamDraw);
	color_vbo_->allocate(grid_colors_, sizeof(QVector3D) * 10800);
	color_vbo_->release();

	// 设置顶点坐标数组对象与Buffer
	vao_ = new QOpenGLVertexArrayObject;
	vao_->create();
	vao_->bind();

	vbo_ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vbo_->create();
	vbo_->bind();
	vbo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_->allocate(grid_vertices_, sizeof(float) * 8);

	ebo_ = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	ebo_->create();
	ebo_->bind();
	ebo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
	ebo_->allocate(grid_indices_, sizeof(unsigned int) * 6);

	auto gl_functions = context_->extraFunctions();

	gl_functions->glEnableVertexAttribArray(0);
	gl_functions->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));

	gl_functions->glEnableVertexAttribArray(1);
	offset_vbo_->bind();
	gl_functions->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));
	offset_vbo_->release();
	gl_functions->glVertexAttribDivisor(1, 1);

	gl_functions->glEnableVertexAttribArray(2);
	color_vbo_->bind();
	gl_functions->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
	color_vbo_->release();
	gl_functions->glVertexAttribDivisor(2, 1);

	vbo_->release();
	vao_->release();
}

Screen::~Screen() {
	delete color_vbo_;
	delete offset_vbo_;
	delete ebo_;
	delete vbo_;
	delete vao_;
}

QOpenGLVertexArrayObject* Screen::GetVao() const {
	return vao_;
}

void Screen::DrawGrids() {

}

#include "screen.hpp"

Screen::Screen(QOpenGLContext *context) {

	// 创建120*90的Grid矩阵
	for (auto y = 0; y < 90; y++) {
		for (auto x = 0; x < 120; x++) {
			grids_.append(Grid(x, y));
		}
	}

	// 获取OpenGL函数
	gl_functions_ = context->extraFunctions();

	// 设置Grid顶点坐标（顺时针顺序）
	float grid_vertices[8] = {
		1.0f,  1.0f,  // 右上
		1.0f, -1.0f,  // 右下
	   -1.0f, -1.0f,  // 左下
	   -1.0f,  1.0f,  // 左上  
	};

	// 设置Grid Indices
	unsigned int grid_indices[6] = {
		0, 1, 3,  // 第一个三角形
		1, 2, 3   // 第二个三角形
	};

	// 设置Grid实例化渲染中每一个Grid的偏移量
	QVector2D grid_offsets[10800];
	auto index = 0;
	const auto offset = 1.025f;
	for (auto y = 9225; y > -9225; y -= 205) {
		for (auto x = -12300; x < 12300; x += 205) {
			QVector2D translation;
			translation.setX(static_cast<float>(x) / 100.0f + offset);
			translation.setY(static_cast<float>(y) / 100.0f - offset);
			grid_offsets[index++] = translation;
		}
	}

	// 设置偏移量Buffer
	offset_vbo_ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	offset_vbo_->create();
	offset_vbo_->bind();
	offset_vbo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
	offset_vbo_->allocate(grid_offsets, sizeof grid_offsets);
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
	delete[] grid_colors_;

	// 设置顶点坐标数组对象与Buffer
	vao_ = new QOpenGLVertexArrayObject;
	vao_->create();
	vao_->bind();

	vbo_ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vbo_->create();
	vbo_->bind();
	vbo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_->allocate(grid_vertices, sizeof grid_vertices);

	ebo_ = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	ebo_->create();
	ebo_->bind();
	ebo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
	ebo_->allocate(grid_indices, sizeof grid_indices);

	// 设置顶点属性-坐标
	gl_functions_->glEnableVertexAttribArray(0);
	gl_functions_->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));

	// 设置顶点属性-偏移量
	gl_functions_->glEnableVertexAttribArray(1);
	offset_vbo_->bind();
	gl_functions_->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));
	offset_vbo_->release();
	gl_functions_->glVertexAttribDivisor(1, 1);

	// 设置顶点属性-颜色
	gl_functions_->glEnableVertexAttribArray(2);
	color_vbo_->bind();
	gl_functions_->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
	color_vbo_->release();
	gl_functions_->glVertexAttribDivisor(2, 1);

	ebo_->release();
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

int i = 0;
void Screen::DrawGrids() {
	grids_[i++].SetColorRgb(255, 255, 255);

	// 获取需更新的Grid
	for (auto grid : grids_)
		if (grid.IsDirty())
			dirty_grids_list_.append(grid);

	// 仅更新需要更新的Grid
	color_vbo_->bind();
	grid_colors_ = static_cast<QVector3D*>(gl_functions_->glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(QVector3D) * 10800, GL_MAP_WRITE_BIT));
	for (auto grid : dirty_grids_list_) {
		const auto index = grid.GetY() * 120 + grid.GetX();
		grid_colors_[index] = grid.GetNormalizedRgb();
		grid.SetClear();
	}
	dirty_grids_list_.clear();
	gl_functions_->glUnmapBuffer(GL_ARRAY_BUFFER);
	color_vbo_->release();

	// 绘制Grid
	vao_->bind();
	ebo_->bind();
	gl_functions_->glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, 10800);
	ebo_->release();
	vao_->release();
}

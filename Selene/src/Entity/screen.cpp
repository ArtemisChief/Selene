#include "screen.hpp"

Screen::Screen(QOpenGLContext *context) {

	// ����120*90��Grid����
	for (auto y = 0; y < 90; y++) {
		for (auto x = 0; x < 120; x++) {
			grids_.append(Grid(x, y));
		}
	}

	// ��ȡOpenGL����
	gl_functions_ = context->extraFunctions();

	// ����Grid�������꣨˳ʱ��˳��
	float grid_vertices[8] = {
		1.0f,  1.0f,  // ����
		1.0f, -1.0f,  // ����
	   -1.0f, -1.0f,  // ����
	   -1.0f,  1.0f,  // ����  
	};

	// ����Grid Indices
	unsigned int grid_indices[6] = {
		0, 1, 3,  // ��һ��������
		1, 2, 3   // �ڶ���������
	};

	// ����Gridʵ������Ⱦ��ÿһ��Grid��ƫ����
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

	// ����ƫ����Buffer
	offset_vbo_ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	offset_vbo_->create();
	offset_vbo_->bind();
	offset_vbo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
	offset_vbo_->allocate(grid_offsets, sizeof grid_offsets);
	offset_vbo_->release();

	// ����Gridʵ������Ⱦ��ÿһ��Grid����ɫ
	grid_colors_ = new QVector3D[10800];
	index = 0;
	for (auto grid : grids_) {
		grid_colors_[index++] = grid.GetNormalizedRgb();
	}

	// ������ɫBuffer
	color_vbo_ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	color_vbo_->create();
	color_vbo_->bind();
	color_vbo_->setUsagePattern(QOpenGLBuffer::StreamDraw);
	color_vbo_->allocate(grid_colors_, sizeof(QVector3D) * 10800);
	color_vbo_->release();
	delete[] grid_colors_;

	// ���ö����������������Buffer
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

	// ���ö�������-����
	gl_functions_->glEnableVertexAttribArray(0);
	gl_functions_->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));

	// ���ö�������-ƫ����
	gl_functions_->glEnableVertexAttribArray(1);
	offset_vbo_->bind();
	gl_functions_->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));
	offset_vbo_->release();
	gl_functions_->glVertexAttribDivisor(1, 1);

	// ���ö�������-��ɫ
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

	// ��ȡ����µ�Grid
	for (auto grid : grids_)
		if (grid.IsDirty())
			dirty_grids_list_.append(grid);

	// ��������Ҫ���µ�Grid
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

	// ����Grid
	vao_->bind();
	ebo_->bind();
	gl_functions_->glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, 10800);
	ebo_->release();
	vao_->release();
}

#include <iostream>

#include "screen.hpp"
#include "grid.hpp"

#include <QMediaPlayer>

Screen::Screen(QOpenGLContext *context, QOpenGLShaderProgram *shader_program)
{
	// ��ȡOpenGL����
	gl_functions_ = context->extraFunctions();

	// ��ȡ��ɫ��
	shader_program_ = shader_program;

	// ��ȡ��ɫ��Uniformλ��
	uniform_is_border_color_location_ = shader_program_->uniformLocation("isBorderColor");

	// ��ȡ��Ƶ
	capture_ = new cv::VideoCapture("./BadApple.mp4");

	// ���ó�ʼ����ͣ
	is_paused_ = false;

	// ������
	show_grid_line_ = false;
	gl_functions_->glLineWidth(0.1f);

	// ����Gridʵ������Ⱦ��ÿһ��Grid��ƫ����
	const auto grid_offsets = new QVector2D[grid_counts_];
	auto index = 0;
	const auto distance = 2;
	const auto range_x = RESOLUTION_X * distance / 2;
	const auto range_y = RESOLUTION_Y * distance / 2;
	const auto offset = static_cast<float>(distance) / 2.0f;
	for (auto y = range_y; y > -range_y; y -= distance)
	{
		for (auto x = -range_x; x < range_x; x += distance)
		{
			QVector2D translation;
			translation.setX(static_cast<float>(x) + offset);
			translation.setY(static_cast<float>(y) - offset);
			grid_offsets[index++] = translation;
		}
	}

	// ����ƫ����Buffer
	QOpenGLBuffer offset_vbo(QOpenGLBuffer::VertexBuffer);
	offset_vbo.create();
	offset_vbo.bind();
	offset_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	offset_vbo.allocate(grid_offsets, sizeof(QVector2D) * grid_counts_);
	offset_vbo.release();
	delete[] grid_offsets;

	// ����Gridʵ������Ⱦ��ÿһ��Grid����ɫ��ʼΪ��ɫ
	const auto grid_colors = new QVector3D[grid_counts_];
	for (auto i = 0; i < grid_counts_; i++)
		grid_colors[i++] = QVector3D(0.0f, 0.0f, 0.0f);

	// ������ɫBuffer
	grid_color_vbo_ = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	grid_color_vbo_.create();
	grid_color_vbo_.bind();
	grid_color_vbo_.setUsagePattern(QOpenGLBuffer::StreamDraw);
	grid_color_vbo_.allocate(grid_colors, sizeof(QVector3D) * grid_counts_);
	grid_color_vbo_.release();
	delete[] grid_colors;

	// ���ö����������������Buffer
	grid_vao_.create();
	grid_vao_.bind();

	QOpenGLBuffer vbo(QOpenGLBuffer::VertexBuffer);
	vbo.create();
	vbo.bind();
	vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo.allocate(Grid::GRID_VERTICES, sizeof(float) * 8);

	// ���ö�������-����
	gl_functions_->glEnableVertexAttribArray(0);
	gl_functions_->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));

	// ���ö�������-ƫ����
	gl_functions_->glEnableVertexAttribArray(1);
	offset_vbo.bind();
	gl_functions_->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));
	offset_vbo.release();
	gl_functions_->glVertexAttribDivisor(1, 1);

	// ���ö�������-��ɫ
	gl_functions_->glEnableVertexAttribArray(2);
	grid_color_vbo_.bind();
	gl_functions_->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
	grid_color_vbo_.release();
	gl_functions_->glVertexAttribDivisor(2, 1);

	vbo.release();
	grid_vao_.release();

}

Screen::~Screen() {
}

void Screen::SetIsPaused(const bool is_paused) {
	is_paused_ = is_paused;
}

bool Screen::GetIsPaused() const {
	return is_paused_;
}

void Screen::DrawGrids() {

	if (!is_paused_) {
		// ��������Grid��ɫ
		grid_color_vbo_.bind();

		const auto grid_colors = static_cast<QVector3D*>(gl_functions_->glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(QVector3D) * grid_counts_, GL_MAP_WRITE_BIT));

		capture_->read(current_mat_);

		const auto mat_rows = current_mat_.rows;
		const auto mat_cols = current_mat_.cols;

		for (auto y = 0; y < mat_rows; y+=2) {
			const uchar *pixel = current_mat_.data + y * current_mat_.step;
			for (auto x = 0; x < mat_cols; x += 2) {
				grid_colors[y / 2 * RESOLUTION_X + x / 2] = QVector3D(*pixel++, *pixel++, *pixel++) / 255;
				pixel += 3;
			}
		}

		gl_functions_->glUnmapBuffer(GL_ARRAY_BUFFER);

		grid_color_vbo_.release();
	}

	// ����Grid
	grid_vao_.bind();

	gl_functions_->glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, Grid::GRID_INDICES, grid_counts_);

	if (show_grid_line_) {
		shader_program_->setUniformValue(uniform_is_border_color_location_, true);
		gl_functions_->glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, grid_counts_);
		shader_program_->setUniformValue(uniform_is_border_color_location_, false);
	}

	grid_vao_.release();

}

void Screen::ShowGridLine(const bool show_grid_line) {
	show_grid_line_ = show_grid_line;
}

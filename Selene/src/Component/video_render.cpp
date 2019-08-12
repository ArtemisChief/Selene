#include <iostream>

#include "video_render.hpp"
#include "src/Entity/grid.hpp"

VideoRender* VideoRender::video_render_ = nullptr;

VideoRender::VideoRender(QOpenGLContext *context, QOpenGLShaderProgram *shader_program) {
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
	const auto grid_offsets = new QVector2D[GRID_COUNTS];
	auto index = 0;
	const auto distance = 2;
	const auto range_x = RESOLUTION_X * distance / 2;
	const auto range_y = RESOLUTION_Y * distance / 2;
	const auto offset = static_cast<float>(distance) / 2.0f;
	for (auto y = range_y; y > -range_y; y -= distance) {
		for (auto x = -range_x; x < range_x; x += distance) {
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
	offset_vbo.allocate(grid_offsets, sizeof(QVector2D) * GRID_COUNTS);
	offset_vbo.release();
	delete[] grid_offsets;

	// ����Gridʵ������Ⱦ��ÿһ��Grid����ɫ��ʼΪ��ɫ
	const auto grid_colors = new QVector3D[GRID_COUNTS];
	for (auto i = 0; i < GRID_COUNTS; i++)
		grid_colors[i++] = QVector3D(0.0f, 0.0f, 0.0f);

	// ������ɫBuffer
	grid_color_vbo_ = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	grid_color_vbo_.create();
	grid_color_vbo_.bind();
	grid_color_vbo_.setUsagePattern(QOpenGLBuffer::StreamDraw);
	grid_color_vbo_.allocate(grid_colors, sizeof(QVector3D) * GRID_COUNTS);
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

VideoRender* VideoRender::CreateInstance(QOpenGLContext *context, QOpenGLShaderProgram *shader_program) {
	if (video_render_ == nullptr)
		video_render_ = new VideoRender(context, shader_program);
	return video_render_;
}

VideoRender* VideoRender::GetInstance() {
	return video_render_;
}

void VideoRender::DrawGrids() {

	if (!is_paused_) {
		// ��������Grid��ɫ
		grid_color_vbo_.bind();

		const auto pixel_channel_sum = GRID_COUNTS * 3;
		auto grid_colors = static_cast<float*>(gl_functions_->glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(float) * pixel_channel_sum, GL_MAP_WRITE_BIT));

		capture_->read(current_mat_);

		const uchar *pixel = current_mat_.data;

		for (auto j = 0; j < pixel_channel_sum; j++) 
			*grid_colors++ = *pixel++;

		gl_functions_->glUnmapBuffer(GL_ARRAY_BUFFER);

		grid_color_vbo_.release();
	}

	// ����Grid

	gl_functions_->glClear(GL_COLOR_BUFFER_BIT);

	grid_vao_.bind();

	gl_functions_->glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, Grid::GRID_INDICES, GRID_COUNTS);

	if (show_grid_line_) {
		shader_program_->setUniformValue(uniform_is_border_color_location_, true);
		gl_functions_->glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, GRID_COUNTS);
		shader_program_->setUniformValue(uniform_is_border_color_location_, false);
	}

	grid_vao_.release();

}

void VideoRender::ShowGridLine(const bool show_grid_line) {
	show_grid_line_ = show_grid_line;
}

void VideoRender::SetIsPaused() {
	is_paused_ = !is_paused_;
}

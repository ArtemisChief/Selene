#include <iostream>

#include "video_render.hpp"
#include "src/Entity/grid.hpp"

VideoRender* VideoRender::video_render_ = nullptr;

VideoRender::VideoRender(QOpenGLContext *context, QOpenGLShaderProgram *shader_program) {
	// 获取OpenGL函数
	gl_functions_ = context->extraFunctions();

	// 获取着色器
	shader_program_ = shader_program;

	// 获取着色器Uniform位置
	uniform_is_border_color_location_ = shader_program_->uniformLocation("isBorderColor");

	// 获取视频
	capture_ = new cv::VideoCapture("./BadApple.mp4");

	// 设置初始不暂停
	is_paused_ = false;

	// 网格线
	show_grid_line_ = false;
	gl_functions_->glLineWidth(0.1f);

	// 设置Grid实例化渲染中每一个Grid的偏移量
	const auto grid_offsets = new QVector2D[grids_quantity_];
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

	// 设置偏移量Buffer
	QOpenGLBuffer offset_vbo(QOpenGLBuffer::VertexBuffer);
	offset_vbo.create();
	offset_vbo.bind();
	offset_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	offset_vbo.allocate(grid_offsets, sizeof(QVector2D) * grids_quantity_);
	offset_vbo.release();
	delete[] grid_offsets;

	// 设置颜色Buffer
	grids_color_vbo_ = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	grids_color_vbo_.create();
	grids_color_vbo_.bind();
	grids_color_vbo_.setUsagePattern(QOpenGLBuffer::DynamicCopy);
	grids_color_vbo_.release();

	// 设置顶点坐标数组对象与Buffer
	grid_vao_.create();
	grid_vao_.bind();

	QOpenGLBuffer vbo(QOpenGLBuffer::VertexBuffer);
	vbo.create();
	vbo.bind();
	vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo.allocate(Grid::GRID_VERTICES, sizeof(float) * 8);

	// 设置顶点属性-坐标
	gl_functions_->glEnableVertexAttribArray(0);
	gl_functions_->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));

	// 设置顶点属性-偏移量
	gl_functions_->glEnableVertexAttribArray(1);
	offset_vbo.bind();
	gl_functions_->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));
	offset_vbo.release();
	gl_functions_->glVertexAttribDivisor(1, 1);

	// 设置顶点属性-颜色
	gl_functions_->glEnableVertexAttribArray(2);
	grids_color_vbo_.bind();
	gl_functions_->glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(uchar), static_cast<void*>(nullptr));
	grids_color_vbo_.release();
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

	gl_functions_->glClear(GL_COLOR_BUFFER_BIT);

	if (!is_paused_) {
		// 更新所有Grid颜色
		capture_->read(current_frame_);
		grids_color_vbo_.bind();
		grids_color_vbo_.allocate(current_frame_.data, sizeof(uchar) * grids_quantity_ * 3);
		grids_color_vbo_.release();
	}

	// 绘制Grid
	grid_vao_.bind();

	gl_functions_->glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, Grid::GRID_INDICES, grids_quantity_);

	if (show_grid_line_) {
		shader_program_->setUniformValue(uniform_is_border_color_location_, true);
		gl_functions_->glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, grids_quantity_);
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

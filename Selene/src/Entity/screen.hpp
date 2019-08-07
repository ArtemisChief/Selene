#pragma once
#include <QVector2D>;
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>

#include "grid.hpp"

class Screen {

public:

	Screen(QOpenGLContext *context);
	~Screen();

	void DrawGrids();

private:

	QList<Grid> grids_;
	QList<Grid> dirty_grids_list_;

	// OpenGL函数
	QOpenGLExtraFunctions *gl_functions_;

	// 顶点缓存对象
	QOpenGLVertexArrayObject *vao_;
	QOpenGLBuffer *vbo_;
	QOpenGLBuffer *ebo_;
	QOpenGLBuffer *offset_vbo_;
	QOpenGLBuffer *color_vbo_;

	// Grid颜色数组
	QVector3D* grid_colors_;

};

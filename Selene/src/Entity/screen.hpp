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

	// OpenGL����
	QOpenGLExtraFunctions *gl_functions_;

	// ���㻺�����
	QOpenGLVertexArrayObject *vao_;
	QOpenGLBuffer *vbo_;
	QOpenGLBuffer *ebo_;
	QOpenGLBuffer *offset_vbo_;
	QOpenGLBuffer *color_vbo_;

	// Grid��ɫ����
	QVector3D* grid_colors_;

};

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

	QOpenGLVertexArrayObject* GetVao() const;

	void DrawGrids();

private:

	QList<Grid> grids_;
	QList<Grid> dirty_grids_list_;

	// OpenGL上下文
	QOpenGLContext *context_;

	// 顶点缓存对象
	QOpenGLVertexArrayObject *vao_;
	QOpenGLBuffer *vbo_;
	QOpenGLBuffer *ebo_;
	QOpenGLBuffer *offset_vbo_;
	QOpenGLBuffer *color_vbo_;

	// Grid顶点坐标与偏移量
	float* grid_vertices_;
	unsigned int* grid_indices_;
	QVector2D* grid_offsets_;
	QVector3D* grid_colors_;

};

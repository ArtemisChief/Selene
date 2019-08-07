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

	// OpenGL������
	QOpenGLContext *context_;

	// ���㻺�����
	QOpenGLVertexArrayObject *vao_;
	QOpenGLBuffer *vbo_;
	QOpenGLBuffer *ebo_;
	QOpenGLBuffer *offset_vbo_;
	QOpenGLBuffer *color_vbo_;

	// Grid����������ƫ����
	float* grid_vertices_;
	unsigned int* grid_indices_;
	QVector2D* grid_offsets_;
	QVector3D* grid_colors_;

};

#include "grid.hpp"

float *Grid::GRID_VERTICES =new float[8] {
		1.0f,  1.0f, // 右上
		1.0f, -1.0f, // 右下
	   -1.0f, -1.0f, // 左下
	   -1.0f,  1.0f, // 左上  
};

unsigned int *Grid::GRID_INDICES = new unsigned int[6] {
		0, 1, 3, // 第一个三角形
		1, 2, 3 // 第二个三角形
};
#include "grid.hpp"

float *Grid::GRID_VERTICES =new float[8] {
		1.0f,  1.0f, // ����
		1.0f, -1.0f, // ����
	   -1.0f, -1.0f, // ����
	   -1.0f,  1.0f, // ����  
};

unsigned int *Grid::GRID_INDICES = new unsigned int[6] {
		0, 1, 3, // ��һ��������
		1, 2, 3 // �ڶ���������
};
#include "grid.hpp"


Grid::Grid() : color_r_(0), color_g_(0), color_b_(0), is_dirty_(false) {
}

Grid::~Grid() {
}

void Grid::SetColorR(const short value) {
	color_r_ = value;
}

void Grid::SetColorG(const short value) {
	color_g_ = value;
}

void Grid::SetColorB(const short value) {
	color_b_ = value;
}

void Grid::SetColorRgb(const short r, const short g, const short b) {
	color_r_ = r;
	color_g_ = g;
	color_b_ = b;
}

void Grid::SetColorRgb(const QVector3D rgb) {
	color_r_ = rgb.x();
	color_g_ = rgb.y();
	color_b_ = rgb.z();
}

QVector3D Grid::GetNormalizedRgb() const {
	return QVector3D(color_r_, color_g_, color_b_) /= 255;
}

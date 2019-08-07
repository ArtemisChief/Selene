#include "grid.hpp"


Grid::Grid(const short x, const short y) : x_(x), y_(y), color_r_(0), color_g_(0), color_b_(0), is_dirty_(false) {
}

Grid::~Grid() {
}

void Grid::SetColorRgb(const short r, const short g, const short b) {
	color_r_ = r;
	color_g_ = g;
	color_b_ = b;
	is_dirty_ = true;
}

void Grid::SetColorRgb(const QVector3D rgb) {
	color_r_ = rgb.x();
	color_g_ = rgb.y();
	color_b_ = rgb.z();
	is_dirty_ = true;
}

QVector3D Grid::GetNormalizedRgb() const {
	return QVector3D(color_r_, color_g_, color_b_) /= 255;
}

short Grid::GetX() const {
	return x_;
}

short Grid::GetY() const {
	return y_;
}

void Grid::SetClear() {
	is_dirty_ = false;
}

bool Grid::IsDirty() const {
	return is_dirty_;
}

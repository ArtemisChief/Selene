#pragma once
#include <QVector3D>

class Grid {

public:
	Grid();
	~Grid();

	void SetColorR(const short value);
	void SetColorG(const short value);
	void SetColorB(const short value);
	void SetColorRgb(const short r, const short g, const short b);
	void SetColorRgb(const QVector3D rgb);

	QVector3D GetNormalizedRgb() const;

private:
	short color_r_;
	short color_g_;
	short color_b_;

};
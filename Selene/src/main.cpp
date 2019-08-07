#include <QGuiApplication>
#include "Component/gl_window.hpp"

int main(int argc, char *argv[]) {
	
	QGuiApplication app(argc, argv);

	QSurfaceFormat surface_format;

	// 使用OpenGL 3.3 Core 模式
	surface_format.setVersion(3, 3);
	surface_format.setProfile(QSurfaceFormat::CoreProfile);

	// 设置多重采样抗锯齿倍率
	surface_format.setSamples(8);

	QSurfaceFormat::setDefaultFormat(surface_format);

	GLWindow gl_window;
	gl_window.resize(1280, 960);
	gl_window.show();

	return app.exec();
}

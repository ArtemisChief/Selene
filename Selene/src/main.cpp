#include <QGuiApplication>
#include "Component/gl_window.hpp"

int main(int argc, char *argv[]) {
	
	QGuiApplication app(argc, argv);

	QSurfaceFormat surface_format;

	// ʹ��OpenGL 3.3 Core ģʽ
	surface_format.setVersion(3, 3);
	surface_format.setProfile(QSurfaceFormat::CoreProfile);

	// ���ö��ز�������ݱ���
	surface_format.setSamples(8);

	QSurfaceFormat::setDefaultFormat(surface_format);

	GLWindow gl_window;
	gl_window.resize(1280, 960);
	gl_window.show();

	return app.exec();
}

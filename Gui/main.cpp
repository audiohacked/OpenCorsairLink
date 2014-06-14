#include <QApplication>

#include "window.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	app.setOrganizationName("Sean Nelson");
	app.setApplicationName("Open Corsair Link");
	MainWindow mainWin;
	mainWin.show();
	return app.exec();
}


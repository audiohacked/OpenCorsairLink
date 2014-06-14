#include <QtWidgets>
#include "window.h"

MainWindow::MainWindow() {
}

void MainWindow::closeEvent(QCloseEvent *event) {
	event->accept();
}

void MainWindow::about() {
	QMessageBox::about(this, tr("About OpenCorsairLink"),
		tr("The <b>Application</b> example demonstrates how to "
		"write modern GUI applications using Qt, with a menu bar, "
		"toolbars, and a status bar."));
}

void MainWindow::createActions() {
}

void MainWindow::createMenus() {
}

void MainWindow::createToolBars() {
}

void MainWindow::createStatusBar() {
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings() {
}

void MainWindow::writeSettings() {
}


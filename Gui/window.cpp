#include <QtGui>
#include "window.h"

CorsairLink *cl;

MainWindow::MainWindow() {
	/*
	 * widget
	 */
	cl = new CorsairLink();

	if(!cl->Initialize()) {
		fprintf(stdout, "Cannot initialize link.\n");

		QMessageBox msgBox;
		msgBox.setText("No CorsairLink Present!");
		msgBox.exec();

		delete cl;
	} else {
		mainWidget = new MainWidget(this);
		setCentralWidget(mainWidget);

		createActions();
		createMenus();
		//createToolBars();
		createStatusBar();
		readSettings();
		setUnifiedTitleAndToolBarOnMac(true);
	}
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
	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus() {
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

//void MainWindow::createToolBars() {
//}

void MainWindow::createStatusBar() {
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings() {
	QSettings settings("HyperKineticNerd", "OpenCorsairLink");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
}

void MainWindow::writeSettings() {
	QSettings settings("HyperKineticNerd", "OpenCorsairLink");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

MainWidget::MainWidget(QWidget *parent)
	: QTabWidget(parent)
{
	f = new CorsairFan();
	l = new CorsairLed();
	t = new CorsairTemp();

	if (cl != NULL) {
		status = new StatusWidget(this);
		//fanWidget = new FanWidget(this);
		//ledWidget = new LedWidget(this);
		//tempWidget = new TemperatureWidget(this);
	
		addTab(status, "Status");
		//addTab(fanWidget, "Fan");
		//addTab(ledWidget, "LEDs");
		//addTab(tempWidget, "Temperature");
	}
}

MainFrame::MainFrame(QWidget *parent)
	: QWidget(parent)
{
	
}
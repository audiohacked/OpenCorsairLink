#ifndef _WINDOW_H
#define _WINDOW_H

#include <QMainWindow>

#include <QAction>
#include <QMenu>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QString>
#include <QSettings>
#include <QStatusBar>
#include <QTabWidget>

#include "StatusWidget.h"
#include "FanWidget.h"
#include "LedWidget.h"
#include "TemperatureWidget.h"

#include "Proto.h"
#include "Link.h"
#include "Fan.h"
#include "Led.h"
#include "Temperature.h"

class MainFrame : public QWidget
{
	Q_OBJECT
public:
	MainFrame(QWidget *parent = 0);
};

class MainWidget : public QTabWidget
{
	Q_OBJECT
public:
	MainWidget(QWidget *parent = 0);
private:
	CorsairLink *cl;
	CorsairFan *f;
	CorsairLed *l;
	StatusWidget *status;
	//FanWidget *fanWidget;
	//LedWidget *ledWidget;
	//TemperatureWidget *tempWidget;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void about();

private:
	void createActions();
	void createMenus();
	//void createToolBars();
	void createStatusBar();
	void readSettings();
	void writeSettings();

	MainWidget *mainWidget;

	QMenu *fileMenu;
	QMenu *helpMenu;
	QAction *exitAct;
	QAction *aboutAct;
	QAction *aboutQtAct;
};

#endif /* _WINDOW_H */


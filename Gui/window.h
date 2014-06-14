#ifndef _WINDOW_H
#define _WINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;

class MainWidget : public QTabWidget
{
	Q_OBJECT
public:
	MainWidget();
signals:
	void selectionChanged(const QItemSelection &selected);
private:
	StatusWidget *status;
	FanWidget *fanWidget;
	LedWidget *ledWidget;
	TemperatureWidget *tempWidget;
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


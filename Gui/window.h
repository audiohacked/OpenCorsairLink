#ifndef _WINDOW_H
#define _WINDOW_H

#include <QMainWindow>

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
	void createMenu();
}

#endif


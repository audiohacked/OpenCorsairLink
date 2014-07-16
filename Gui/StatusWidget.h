#ifndef _STATUS_WIDGET_H
#define _STATUS_WIDGET_H

#include <QWidget>
#include <QLCDNumber>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QVBoxLayout>
#include <QPair>

#include "Fan.h"

class MainWidget;

class StatusWidget : public QWidget
{
	Q_OBJECT

public:
	explicit StatusWidget(MainWidget *parent = 0);
	~StatusWidget();
	void timerEvent(QTimerEvent *event);
private:
	void updateLedColor();
	void updateTemp();
	void updateFanSpeed();

	int timerid;
	MainWidget *parentWidget;
	QList<QPushButton*> LedArray;
	QList<QLCDNumber*> TempArray;
	QList< QPair<QLCDNumber*, CorsairFanInfo*> > FanArray;
};

#endif /* _STATUS_WIDGET_H */


#include "StatusWidget.h"
#include "window.h"

#include <QDebug>

#include "Fan.h"
#include "Led.h"
#include "Temperature.h"

StatusWidget::StatusWidget(MainWidget *parent) 
	: QWidget(parent)
{
	int i = 0;
	parentWidget = parent;
	/* setup layout before we initialize all our monitors */
	QVBoxLayout *layout = new QVBoxLayout;

	/* Set up LEDs, dynamically */
	QLabel *LEDLabel = new QLabel(QString("LEDs:"), this);
	qDebug() << "LED Count:" << parent->led()->GetLedCount();
	for( i=0; i < parent->led()->GetLedCount(); i++) {
		QPushButton *led = new QPushButton(QString::number(i+1), this);
		LedArray.append(led);
	}

	/* Set up Temperatures, dynamically */
	QLabel *TemperatureLabel = new QLabel(QString("Temperatures:"), this);
	qDebug() << "Temperature Count:" << parent->temp()->GetTempSensors();
	for( i=0; i < parent->temp()->GetTempSensors(); i++) {
		QLCDNumber *TemperatureLCD = new QLCDNumber(8, this);
		TempArray.append(TemperatureLCD);
	}

	/* Set up Fan Monitoring, dynamically */
	QLabel *FanLabel = new QLabel(QString("Fans:"), this);
	qDebug() << "Fan Count:" << parent->fan()->ConnectedFans();
	for( i=0; i < parent->fan()->ConnectedFans()+1; i++) {
		FanArray.append( qMakePair(new QLCDNumber(8, this), new CorsairFanInfo()) );
		parentWidget->fan()->ReadFanInfo(i, FanArray[i].second);
	}

	/* setup layout of panel */
	layout->addWidget(LEDLabel);
	for( i=0; i < LedArray.count(); i++) {
		layout->addWidget(LedArray[i]);
	}

	layout->addWidget(TemperatureLabel);
	for( i=0; i < TempArray.count(); i++) {
		layout->addWidget(TempArray[i]);
	}

	layout->addWidget(FanLabel);
	for( i=0; i < FanArray.count(); i++) {
		QLabel *fanNameLabel = new QLabel(QString(FanArray[i].second->Name), this);
		layout->addWidget(fanNameLabel);
		layout->addWidget(FanArray[i].first);
	}

	setLayout(layout);
	
	/* Set up Timer Update-Loop */
	timerid = this->startTimer(2000);
}

StatusWidget::~StatusWidget() {
	this->killTimer(timerid);
}

void StatusWidget::timerEvent(QTimerEvent *event) {
	if (event) {
		updateLedColor();
		updateTemp();
		updateFanSpeed();
	}
}

void StatusWidget::updateLedColor() {
	for(int i=0; i < LedArray.count(); i++) {
		QPalette pal = LedArray[i]->palette();
		parentWidget->led()->GetColor(&parentWidget->led()->color[0]);
		pal.setColor(
				QPalette::Button,
			 	QColor(
						parentWidget->led()->color[0].red,
						parentWidget->led()->color[0].green,
						parentWidget->led()->color[0].blue
				)
		);
		LedArray[i]->setPalette( pal );
		LedArray[i]->setAutoFillBackground( true );
	}		
}

void StatusWidget::updateTemp() {
	for(int i=0; i < TempArray.count(); i++) {
		parentWidget->temp()->SelectSensor(i);
		QString q_sensor_temp = QString::number( (float)parentWidget->temp()->GetTemp()/256, 'f', 2 );
		TempArray[i]->display( q_sensor_temp.append(" C") );
	}
}

void StatusWidget::updateFanSpeed() {
	for(int i=0; i < FanArray.count(); i++) {
		parentWidget->fan()->ReadFanInfo(i, FanArray[i].second);
		//qDebug() << FanArray[i].second.Name;
		//qDebug() << "Fan Mode:" << FanArray[i].second.Mode;
		//qDebug() << "Fan Speed:" << FanArray[i].second.RPM;
		QString q_fan_speed = QString::number( FanArray[i].second->RPM );
		FanArray[i].first->display( q_fan_speed );
	}
}

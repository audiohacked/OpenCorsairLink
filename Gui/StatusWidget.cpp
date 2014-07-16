#include "StatusWidget.h"
#include "window.h"

#include <QLCDNumber>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QVBoxLayout>

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
	for( i=0; i < parent->led()->GetLedCount(); i++) {
		QPushButton *led = new QPushButton(QString(i), this);
		LedArray.append(led);
	}

	/* Set up Temperatures, dynamically */
	QLabel *TemperatureLabel = new QLabel(QString("Temperatures:"), this);
	for( i=0; i < parent->temp()->GetTempSensors(); i++) {
		QLCDNumber *TemperatureLCD = new QLCDNumber(7, this);
		TempArray.append(TemperatureLCD);
	}

	/* Set up Fan Monitoring, dynamically */
	QLabel *FanLabel = new QLabel(QString("Fans:"), this);
	for( i=0; i < parent->fan()->GetFanCount(); i++) {
		QLCDNumber *FanLCD = new QLCDNumber(8, this);
		FanArray.append(FanLCD);
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
		layout->addWidget(FanArray[i]);
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
		int sensor_temp = parentWidget->temp()->GetTemp();
		TempArray[i]->display(
				QString( sensor_temp/256 ).append(" C")
		);
	}
}

void StatusWidget::updateFanSpeed() {
	for(int i=0; i < FanArray.count(); i++) {
		parentWidget->fan()->SelectFan(i);
		if (parentWidget->fan()->GetFanMode() == FixedPWM){
			int pwm = parentWidget->fan()->GetFanPWM();
			FanArray[i]->display(
				QString( pwm ).append(" PWM")
			);
		} else {
				int rpm = parentWidget->fan()->GetFanRPM();
				FanArray[i]->display(
					QString( rpm ).append(" RPM")
				);
		}
	}
}

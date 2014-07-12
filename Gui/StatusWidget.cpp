#include "StatusWidget.h"

#include <QLCDNumber>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QVBoxLayout>

StatusWidget::StatusWidget(QWidget *parent) 
	: QWidget(parent)
{
	QLabel *LEDLabel = new QLabel(QString("LEDs:"), this);
	QPushButton *LED1 = new QPushButton(QString("1"), this);
	QPalette pal = LED1->palette( );
	pal.setColor( QPalette::Button, QColor(0,255,255) );
	LED1->setPalette( pal );
	LED1->setAutoFillBackground( true );

	QLabel *TemperatureLabel = new QLabel(QString("Temperatures:"), this);
	QLCDNumber *TemperatureLCD1 = new QLCDNumber(7, this);
	TemperatureLCD1->display(QString("30.23 ").append("C"));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(LEDLabel);
	layout->addWidget(LED1);
	layout->addWidget(TemperatureLabel);
	layout->addWidget(TemperatureLCD1);

	setLayout(layout);
}
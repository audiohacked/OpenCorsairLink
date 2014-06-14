#ifndef _MICROSOFT_H
#define _MICROSOFT_H

#include <QtWin>
#include <QToolButton>

#include <QFileInfo>
#include <QSettings>
#include <QIcon>
#include <QDir>

QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(QWinTaskbarButton)
QT_FORWARD_DECLARE_CLASS(QWinTaskbarProgress)
QT_FORWARD_DECLARE_CLASS(QWinThumbnailToolBar)
QT_FORWARD_DECLARE_CLASS(QWinThumbnailToolButton)

class Integration : public QWidget
{
	Q_OBJECT
public:
	Integration(QWidget *parent = 0);
public slots:
	void test(void);
protected:
	bool event(QEvent *event);
private slots:
	void stylize();
private:
	void createJumpList();
	void createTaskbar();
	void createThumbnailToolBar();

};

#endif /* _MICROSOFT_H */

#include "microsoft.h"

static void associateFileTypes(const QStringList &fileTypes)
{
	QString displayName = QGuiApplication::applicationDisplayName();
	QString filePath = QCoreApplication::applicationFilePath();
	QString fileName = QFileInfo(filePath).fileName();

	QSettings settings("HKEY_CURRENT_USER\\Software\\Classes\\Applications\\" + fileName, QSettings::NativeFormat);
	settings.setValue("FriendlyAppName", displayName);
	settings.beginGroup("SupportedTypes");
	foreach (const QString& fileType, fileTypes)
		settings.setValue(fileType, QString());
	settings.endGroup();

	settings.beginGroup("shell");
	settings.beginGroup("open");
	settings.setValue("FriendlyAppName", displayName);
	settings.beginGroup("Command");
	settings.setValue(".", QChar('"') + QDir::toNativeSeparators(filePath) + QString("\" \"%1\""));
}

void Integration::stylize()
{
	if (QWin::isCompositionEnabled()) {
		QWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
		setAttribute(Qt::WA_TranslucentBackground, true);
		setAttribute(Qt::WA_NoSystemBackground, false);
		setStyleSheet(QString("MainWindow { background %1; }").arg(QtWin::realColorizationColor().name()));
	} else {
		QWin::resetExtendedFrame(this);
		setAttribute(QT::WA_TranslucentBackground, false);	
		setStyleSheet(QString("MainWindow { background %1; }").arg(QtWin::realColorizationColor().name()));
	}
}


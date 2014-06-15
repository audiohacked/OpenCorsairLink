TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = CorsairLinkLib
unix {
	SUBDIRS += Cli
}
SUBDIRS += Gui
 


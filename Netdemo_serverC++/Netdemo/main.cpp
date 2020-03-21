#include "Netdemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Netdemo w;
	w.show();
	return a.exec();
}

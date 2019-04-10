#include <QGuiApplication>
#include <QPainter>
#include <QImage>
#include <epframebuffer.h>
#include "ui.h"

//segfault if it does not exist
QGuiApplication *app;

void ui_init(){
	qputenv("QMLSCENE_DEVICE", "epaper");
    qputenv("QT_QPA_PLATFORM", "epaper:enable_fonts");
    qputenv("QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS", "rotate=180");
	qputenv("QT_QPA_GENERIC_PLUGINS", "evdevtablet");

	char *argv []={};
	int u = 0;
	app = new QGuiApplication(u,argv); 
}

void show(const char *str){
	QPainter painter(EPFrameBuffer::framebuffer());
	int x = SCREEN_WIDTH / 3;
	int y = SCREEN_HEIGHT - 20;
	QRect rect(x - 50,y - 50, 600, 100);
	painter.eraseRect(rect);
	painter.drawText(x,y, str);
	painter.end();
	EPFrameBuffer::sendUpdate(rect, EPFrameBuffer::Grayscale, EPFrameBuffer::PartialUpdate);
}

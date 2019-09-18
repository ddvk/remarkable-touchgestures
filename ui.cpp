#include <QGuiApplication>
#include <QPainter>
#include <QImage>
#include "ui.h"
#include "config.h"
#ifdef REMARKABLE
#include <epframebuffer.h>
//segfault if it does not exist
static QGuiApplication *app=nullptr;
#endif

extern "C" {
    void ui_init(){
    #ifdef REMARKABLE
        qputenv("QMLSCENE_DEVICE", "epaper");
        qputenv("QT_QPA_PLATFORM", "epaper:enable_fonts");
        qputenv("QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS", "rotate=180");
        qputenv("QT_QPA_GENERIC_PLUGINS", "evdevtablet");

        char *argv [0];
        int argc = 0;
        app = new QGuiApplication(argc,argv);
    #endif
    }

    void show(const char *str){

    #ifdef REMARKABLE
        QPainter painter(EPFrameBuffer::framebuffer());
        int x = SCREEN_WIDTH / 3;
        int y = SCREEN_HEIGHT - 20;
        QRect rect(x - 50,y - 50, 600, 100);
        painter.eraseRect(rect);
        painter.drawText(x,y, str);
        painter.end();
        EPFrameBuffer::sendUpdate(rect, EPFrameBuffer::Grayscale, EPFrameBuffer::PartialUpdate);
    #else
        printf("show: %s\n", str);

    #endif
    }
}

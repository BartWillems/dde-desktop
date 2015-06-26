#include "views/translucentframe.h"

TranslucentFrame::TranslucentFrame(QWidget *parent) : QFrame(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowState(Qt::WindowMaximized);
}

TranslucentFrame::~TranslucentFrame()
{
    qDebug() << "delete";
}

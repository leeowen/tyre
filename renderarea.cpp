#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent) : QWidget(parent),mBackgroundColor(0,0,255),mShapeColor(255,255,255)
{

}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100,100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400,300);
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(mBackgroundColor);

    switch(mShape){
    case Origin:
        setShapeColor(Qt::red);
        break;
    case Stretch:
        setShapeColor(Qt::green);
        break;
    }

    painter.setPen(mShapeColor);

    //draw the background
    painter.drawRect(this->rect());
    painter.drawLine(this->rect().topLeft(),this->rect().bottomRight());
}

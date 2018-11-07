#include "renderarea.h"
#include<math.h>

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

QPointF RenderArea::compute_Standard_Ellipse(float t)
{
    float scale=40;
    float x=cos(t)*scale;
    float y=sin(t)*scale;
    return QPointF(x,y);
}
void RenderArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

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
    QPoint center=this->rect().center();
    int stepCount=64;
    float intervalLength=2*M_PI;
    float step=intervalLength/stepCount;
    for(float t=0;t<intervalLength;t+=step)
    {
        QPointF point=compute_Standard_Ellipse(t);
        QPoint pixel;
        pixel.setX(point.x()+center.x());
        pixel.setY(point.y()+center.y());

        painter.drawPoint(pixel);
    }
}

#include "renderarea.h"
#include<math.h>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent),mShapeColor(255,255,255)
{

}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(600,500);
}

QSize RenderArea::sizeHint() const
{
    return QSize(800,600);
}

void RenderArea::on_shape_changed()
{
    switch(mShape){
    case Origin:
        setShapeColor(Qt::red);
        break;
    case Stretch:
        setShapeColor(Qt::green);
        break;
    }
}

QPointF RenderArea::compute_Standard_Ellipse(float t)
{
    float radius=150;
    float x=cos(t)*radius;
    float y=sin(t)*radius;
    return QPointF(x,y);
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(mBackgroundColor);

    painter.setPen(mShapeColor);

    //draw the background
    painter.drawRect(this->rect());

    QPoint center=this->rect().center();
    float intervalLength=2*M_PI;
    mStepCount=getStep();
    float step=intervalLength/mStepCount;

    switch (mShape) {
    case Origin:{
        QPointF prevPoint=compute_Standard_Ellipse(0);
        QPoint prevPixel;
        prevPixel.setX(prevPoint.x()+center.x());
        prevPixel.setY(prevPoint.y()+center.y());
        for(float t=step;t<intervalLength+step;t+=step)
        {
            QPointF point=compute_Standard_Ellipse(t);
            QPoint pixel;
            pixel.setX(point.x()+center.x());
            pixel.setY(point.y()+center.y());

            painter.drawLine(pixel,prevPixel);
            prevPixel=pixel;
        }
        break;
    }
    case Stretch:{
        break;
    }
    default:{
        break;
    }
    }

}

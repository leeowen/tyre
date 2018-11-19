#include "renderarea.h"
#include<math.h>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent),mBackgroundColor(255,255,255),mStepCount(100),mRadius(150),ks(1.0),kb(100.0)
{
    mPen.setWidth(2);
    mPen.setColor(mShapeColor);
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
        cleanup();
        break;
    case Stretch:
        setShapeColor(Qt::green);
        break;
    }
}

Point RenderArea::compute_Standard_Ellipse(float t)
{
    float x=cos(t)*mRadius;
    float y=sin(t)*mRadius;
    return Point(x,y);
}

void RenderArea::stretch(QPainter &painter)
{
    float step=2*M_PI/mStepCount;

    Eigen::MatrixXf b(mStepCount,2);


    for(int i=0;i<mStepCount;i++)
    {
        b(i,0)=-mRadius*cos(i*step);
        b(i,1)=-mRadius*sin(i*step);
    }

    int N=mStepCount/2;
    float delta0=-mRadius*2*mStretchPercentage*0.01;
    float deltaN=mRadius*2*mStretchPercentage*0.01;
    Eigen::Vector2f x=ODEsolver(delta0,deltaN);

    for(int i=0;i<mStepCount;i++)
    {
        b(i,0)=x(i);
    }

    b(0,0)=delta0;
    b(N,0)=deltaN;

    // traverse the vertices
    int n=0;
    for (Vertex_iterator vi = mTyre.vertices_begin(); vi != mTyre.vertices_end(); ++vi)
    {
        CGAL::Vector_2<R> d(b(n,0)-vi->x(),b(n,1)-vi->y());//d for displacement
        CGAL::Aff_transformation_2<R> tsl(CGAL::TRANSLATION, d);
        vi->transform(tsl) ;
        n++;
    }
}


Eigen::Vector2f RenderArea::ODEsolver(float delta0,float deltaN)
{
    int N=mStepCount/2;
    float h=1.0/mStepCount;
    float a1=6.0*kb-2.0*ks*h*h;
    float a2=4.0*kb+ks*h*h;
    float h4=h*h*h*h;
    Eigen::MatrixXf A(mStepCount,mStepCount);//solve the equation Ax=b
    Eigen::MatrixXf x(mStepCount,1),b(mStepCount,1);//x is the displacement vector

    int row=-1;
// node 0 - Eq. (1)
    row=row+1;
    A(row,0)=-h4;
    A(row,1)=-a2;
    A(row,2)=kb;
    A(row,mStepCount-2)=kb;
    A(row,mStepCount-1)=-a2;
    b(row)=-a1*delta0;
// node 1 - Eq. (2)
    row=row+1;
    A(row,1)=a1;
    A(row,2)=-a2;
    A(row,3)=kb;
    A(row,mStepCount-1)=kb;
    b(row)=a2*delta0;
// node 2 - Eq. (3)
    row=row+1;
    A(row,1)=-a2;
    A(row,2)=a1;
    A(row,3)=-a2;
    A(row,4)=kb;
    b(row)=-kb*delta0;
// for the nodes between node 3 and node N-3 - Eq. (4)
    for(row=3;row<=N-3;row++)
    {
        A(row,row-2)=kb;
        A(row,row-1)=-a2;
        A(row,row)=a1;
        A(row,row+1)=-a2;
        A(row,row+2)=kb;
    }

// node N-2 - Eq. (5)
    row=N-2;
    A(row,N-4)=kb;
    A(row,N-3)=-a2;
    A(row,N-2)=a1;
    A(row,N-1)=-a2;
    b(row)=-kb*deltaN;
// node N-1 - Eq. (6)
    row=row+1;
    A(row,N-3)=kb;
    A(row,N-2)=-a2;
    A(row,N-1)=a1;
    A(row,N+1)=kb;
    b(row)=a2*deltaN;
// node N - Eq. (7)
    row=row+1;
    A(row,N-2)=kb;
    A(row,N-1)=-a2;
    A(row,N)=-h4;
    A(row,N+1)=-a2;
    A(row,N+2)=kb;
    b(row)=-a1*deltaN;
// node N+1 - Eq. (8)
    row=row+1;
    A(row,N-1)=kb;
    A(row,N+1)=a1;
    A(row,N+2)=-a2;
    A(row,N+3)=kb;
    b(row)=a2*deltaN;
// node N+2 - Eq. (9)
    row=row+1;
    A(row,N+1)=-a2;
    A(row,N+2)=a1;
    A(row,N+3)=-a2;
    A(row,N+4)=kb;
    b(row)=-kb*deltaN;
// for the nodes between node N+3 and node 2N-3 - Eq. (10)
    for(row=N+3;row<=mStepCount-3;row++)
    {
        A(row,row-2)=kb;
        A(row,row-1)=-a2;
        A(row,row)=a1;
        A(row,row+1)=-a2;
        A(row,row+2)=kb;
    }
// node 2N-2 - Eq. (11)
    row=mStepCount-2;
    A(row,2*N-4)=kb;
    A(row,2*N-3)=-a2;
    A(row,2*N-2)=a1;
    A(row,2*N-1)=-a2;
    b(row)=-kb*delta0;
// node 2N-1 - Eq. (12)
    row=row+1;
    A(row,1)=kb;
    A(row,2*N-3)=kb;
    A(row,2*N-2)=-a2;
    A(row,2*N-1)=a1;
    b(row)=a2*delta0;

    //ciggj(aa,200,bb);
    x = A.colPivHouseholderQr().solve(b);

    return x;
}

void RenderArea::cleanup()
{
    mTyre.clear();
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(mBackgroundColor);
    //draw the background
    painter.drawRect(this->rect());

    painter.setPen(mPen);
    QPoint center=this->rect().center();
    float intervalLength=2*M_PI;
    mStepCount=getStep();
    float step=intervalLength/mStepCount;

    switch (mShape) {
    case Origin:{
        if(mTyre.is_empty())
        {
            for(float t=0;t<=intervalLength+step;t+=step)
            {
                Point point=compute_Standard_Ellipse(t);
                mTyre.push_back(point);
            }
        }
        break;
    }
    case Stretch:{
        try {
            if(mTyre.is_empty()) {
               throw "Nothing to be stretched, press Origin first!";
            }
            stretch(painter);
        } catch (const char* msg) {
            QMessageBox msgBox;
            msgBox.setText(msg);
            msgBox.exec();
        }

        break;
    }
    default: {
        break;
    }
    }

    //actual drawing
    if (mShape==Origin or mShape==Stretch) {
        Point prevPoint=mTyre.vertex(0);
        QPoint prevPixel;
        prevPixel.setX(prevPoint.x()+center.x());
        prevPixel.setY(prevPoint.y()+center.y());
        for(int i=1;i<mTyre.size();i++)
        {
            Point point=mTyre.vertex(i);
            QPoint pixel;
            pixel.setX(point.x()+center.x());
            pixel.setY(point.y()+center.y());

            painter.drawLine(pixel,prevPixel);
            prevPixel=pixel;
        }
    }
}

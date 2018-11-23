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
        cleanup();
        break;
    }
}

Point RenderArea::compute_Standard_Ellipse(float t)
{
    float x=cos(t)*mRadius;
    float y=sin(t)*mRadius;
    return Point(x,y);
}

float RenderArea::perimeter(Polygon &tmp)
{
    float Lap=0;
    int i=0;
    Vertex_iterator vn;
    assert(!tmp.is_empty() && "input polygon is empty!");// This works because char* can be converted to bool and that it never will be false (since the address is not 0).
    for(Vertex_iterator vi=tmp.vertices_begin();vi!=tmp.vertices_end();++vi){
        if(vi!=tmp.vertices_begin())
        {
            float dL=sqrt(pow((vn->x()-vi->x()),2)+pow((vn->y()-vi->y()),2));//  Cord length between two adjacent node
            Lap+=dL;
        }
        if(vi!=tmp.vertices_end())
            vn=vi;
        i++;
    }
    Vertex_iterator vi=tmp.vertices_begin();
    float dL=sqrt(pow((vn->x()-vi->x()),2)+pow((vn->y()-vi->y()),2));//  Cord length between two adjacent node
    Lap+=dL;
    return Lap;
}

Polygon RenderArea::updateShape(Polygon& origin,bool isXcoord,Eigen::VectorXf &x)
{
    assert(!origin.is_empty() && "Input polygon is empty!");
    int n=0;
    Polygon tmp;
    if (isXcoord==true){
        for (Vertex_iterator vi = origin.vertices_begin(); vi != origin.vertices_end(); ++vi)
        {
            tmp.push_back(Point(x(n)+vi->x(),vi->y()));
            n++;
        }
    }
    else{
        for (Vertex_iterator vi = origin.vertices_begin(); vi != origin.vertices_end(); ++vi)
        {
            tmp.push_back(Point(vi->x(),vi->y()+x(n)));
            n++;
        }
    }
    return tmp;
}

void RenderArea::stretch(QPainter &painter)
{
    float step=2*M_PI/mStepCount;
    int N=mStepCount/2;
    mTyre.clear();
    Polygon tmp;
    for(int i=0;i<mStepCount;i++)
    {
        tmp.push_back(Point(-mRadius*cos(i*step),-mRadius*sin(i*step)));
    }

    float area0=tmp.area();
    Eigen::VectorXf x=Eigen::VectorXf::Zero(mStepCount,1);
    Eigen::VectorXf y=Eigen::VectorXf::Zero(mStepCount,1);
    float delta0,deltaN;
    bool isXcoord;
    delta0=-mRadius*2*mStretchPercentage*0.01;
    deltaN=mRadius*2*mStretchPercentage*0.01;
    x=ODEsolver(delta0,deltaN);

    x(0)=delta0;
    x(N)=deltaN;

    isXcoord=true;
    tmp=updateShape(tmp,isXcoord,x);

    float area1=tmp.area();

    // Same area: area0=area1, area1 appro equal to a*b*M_PI
    float a,b,precision;
    precision=0.1;
    a=fabs(tmp.vertex(0).x());
    b=area0/(a*M_PI);//(M_PI*mRadius-2*fabs(a))/(M_PI-2);

    int n=0;
    while ((area1-area0)>precision && n<200)
    {
        std::cout<<"area0 and area1:"<<area0<<", "<<area1<<std::endl;
        float y1=fabs(tmp.vertex(N/2).y())-b;
        std::cout<<"b:"<<b<<std::endl;
        delta0=y1;
        deltaN=-y1;

        Eigen::VectorXf yy=ODEsolver(delta0,deltaN);
        yy(0)=delta0;
        yy(N)=deltaN;
        for(int i=0;i<N/2;i++)
        {
            y(i)=yy(i+3*N/2);
        }
        for(int i=N/2;i<2*N;i++)
        {
            y(i)=yy(i-N/2);
        }
        isXcoord=false;
        tmp=updateShape(tmp,isXcoord,y);
        area1=tmp.area();
        std::cout<<"area0 and area1:"<<area0<<", "<<area1<<std::endl;
        b+=precision;
        n++;
        std::cout<<"iteration:"<<n<<std::endl;
    }

    n=0;
    for (Vertex_iterator vi = tmp.vertices_begin(); vi != tmp.vertices_end(); ++vi)
    {
        mTyre.push_back(Point(vi->x(),vi->y()));
        n++;
    }
}


Eigen::VectorXf RenderArea::ODEsolver(float delta0,float deltaN)
{
    int N=mStepCount/2;
    float h=1.0/mStepCount;
    float a1=6.0*kb-2.0*ks*h*h;
    float a2=4.0*kb+ks*h*h;
    float h4=h*h*h*h;
    Eigen::MatrixXf A=Eigen::MatrixXf::Zero(mStepCount,mStepCount);//solve the equation Ax=b
    Eigen::VectorXf x(mStepCount,1);//x is the unknown displacement vector
    Eigen::VectorXf b=Eigen::VectorXf::Zero(mStepCount,1);
    int row;

// node 0 - Eq. (1)
    row=0;
    A(row,0)=-h4;
    A(row,1)=-a2;
    A(row,2)=kb;
    A(row,2*N-2)=kb;
    A(row,2*N-1)=-a2;
    b(row)=-a1*delta0;
// node 1 - Eq. (2)
    row=1;
    A(row,1)=a1;
    A(row,2)=-a2;
    A(row,3)=kb;
    A(row,2*N-1)=kb;
    b(row)=a2*delta0;
// node 2 - Eq. (3)
    row=2;
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
    row=N-1;
    A(row,N-3)=kb;
    A(row,N-2)=-a2;
    A(row,N-1)=a1;
    A(row,N+1)=kb;
    b(row)=a2*deltaN;
// node N - Eq. (7)
    row=N;
    A(row,N-2)=kb;
    A(row,N-1)=-a2;
    A(row,N)=-h4;
    A(row,N+1)=-a2;
    A(row,N+2)=kb;
    b(row)=-a1*deltaN;
// node N+1 - Eq. (8)
    row=N+1;
    A(row,N-1)=kb;
    A(row,N+1)=a1;
    A(row,N+2)=-a2;
    A(row,N+3)=kb;
    b(row)=a2*deltaN;
// node N+2 - Eq. (9)
    row=N+2;
    A(row,N+1)=-a2;
    A(row,N+2)=a1;
    A(row,N+3)=-a2;
    A(row,N+4)=kb;
    b(row)=-kb*deltaN;
// for the nodes between node N+3 and node 2N-3 - Eq. (10)
    for(row=N+3;row<=2*N-3;row++)
    {
        A(row,row-2)=kb;
        A(row,row-1)=-a2;
        A(row,row)=a1;
        A(row,row+1)=-a2;
        A(row,row+2)=kb;
    }
// node 2N-2 - Eq. (11)
    row=2*N-2;
    A(row,2*N-4)=kb;
    A(row,2*N-3)=-a2;
    A(row,2*N-2)=a1;
    A(row,2*N-1)=-a2;
    b(row)=-kb*delta0;
// node 2N-1 - Eq. (12)
    row=2*N-1;
    A(row,1)=kb;
    A(row,2*N-3)=kb;
    A(row,2*N-2)=-a2;
    A(row,2*N-1)=a1;
    b(row)=a2*delta0;

    x = A.lu().solve(b);

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
        stretch(painter);
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
        Point point=mTyre.vertex(0);
        QPoint pixel;
        pixel.setX(point.x()+center.x());
        pixel.setY(point.y()+center.y());

        painter.drawLine(pixel,prevPixel);
    }
}

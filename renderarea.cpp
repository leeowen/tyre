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

float RenderArea::perimeter(Polygon &tmp,Eigen::MatrixXf &x)
{
    float Lap=0;
    int i=0;
    Vertex_iterator vn;
    for(Vertex_iterator vi=tmp.vertices_begin();vi!=tmp.vertices_end();++vi){
        vn=vi;
        if(vi!=tmp.vertices_begin())
        {
            float dL=sqrt(pow((vn->x()-vi->x()),2)+pow((vn->y()-vi->y()),2));//  Cord length between two adjacent node
            Lap+=dL;
        }
        i++;
    }
    Vertex_iterator vi=tmp.vertices_begin();
    float dL=sqrt(pow((vn->x()-vi->x()),2)+pow((vn->y()-vi->y()),2));//  Cord length between two adjacent node
    Lap+=dL;
    return Lap;
}

void RenderArea::stretch(QPainter &painter)
{
    float step=2*M_PI/mStepCount;
    mTyre.clear();
    Polygon tmp;
    for(int i=0;i<mStepCount;i++)
    {
        tmp.push_back(Point(-mRadius*cos(i*step),-mRadius*sin(i*step)));
    }

    int N=mStepCount/2;
    Eigen::MatrixXf x=Eigen::MatrixXf::Zero(mStepCount,2);
    Eigen::Vector2f delta0;
    delta0(0)=-mRadius*2*mStretchPercentage*0.01;
    delta0(1)=0;
    Eigen::Vector2f deltaN;
    deltaN(0)=mRadius*2*mStretchPercentage*0.01;
    deltaN(1)=0;
    x=ODEsolver(delta0,deltaN);

    x(0,0)=delta0(0);
    x(0,1)=delta0(1);
    x(N,0)=deltaN(0);
    x(N,1)=deltaN(1);

    float Lac=M_PI*mRadius*2;//perimeter
    float Lap=perimeter(tmp,x);

    //printf("Lap,Lac=%le %le\n",Lap,Lac);

    float y1=sqrt((mRadius)*(mRadius)*2-(mRadius-delta0(0))*(mRadius-delta0(0)));
    // Same perimeter
//    while (Lap-Lac>0.001) {

//    }

//    Lap=1.0*Lap;
//    y1=1.164; //Lap=9.424398;Lap1=9.424397;

    //  103% of the original perimeter (Stetch by 5%)
    //	Lap=1.03*Lap;
    //    y1=1.2667;  //Lap=9.707122; Lap1=9.707103

    //  106% of the original perimeter (Stretch by 10%)
    //	Lap=1.06*Lap;
    //    y1=1.366713;  //Lap=9.989854; Lap1=9.989853;

//    delta0(1)=mRadius-y1;
//    deltaN(1)=-delta0(1);
//    // printf("delta0,deltaN= %le %le\n",delta0,deltaN);
//    Eigen::MatrixXf y=ODEsolver(delta0,deltaN);

//    for(i=0;i<N/2;i++)
//    {
//        yi1[i]=yi[i]+y[i+3*N/2];
//        //		printf("%d %le %le %le\n",i,xi[i],x[i],xi1[i]);
//    }
//    for(i=N/2;i<2*N;i++)
//    {
//        yi1[i]=yi[i]+y[i-N/2];
//        //		printf("%d %le %le %le\n",i,xi[i],x[i],xi1[i]);
//    }
//    Lap1=0.0;
//    for(i=1;i<2*N;i++)
//    {
//        Lap1=Lap1+sqrt((xi1[i]-xi1[i-1])*(xi1[i]-xi1[i-1])+(yi1[i]-yi1[i-1])*(yi1[i]-yi1[i-1]));
//    }
//    Lap1=Lap1+sqrt((xi1[0]-xi1[2*N-1])*(xi1[0]-xi1[2*N-1])+(yi1[0]-yi1[2*N-1])*(yi1[0]-yi1[2*N-1]));
//    printf("Lap,Lap1,y1= %le %le %le\n",Lap,Lap1,y1);

    // traverse the vertices
    int n=0;

    for (Vertex_iterator vi = tmp.vertices_begin(); vi != tmp.vertices_end(); ++vi)
    {
        mTyre.push_back(Point(x(n,0)+vi->x(),x(n,1)+vi->y()));
        n++;
    }

}


Eigen::MatrixXf RenderArea::ODEsolver(Eigen::Vector2f &delta0,Eigen::Vector2f &deltaN)
{
    int N=mStepCount/2;
    float h=1.0/mStepCount;
    float a1=6.0*kb-2.0*ks*h*h;
    float a2=4.0*kb+ks*h*h;
    float h4=h*h*h*h;
    Eigen::MatrixXf A=Eigen::MatrixXf::Zero(mStepCount,mStepCount);//solve the equation Ax=b
    Eigen::MatrixXf x(mStepCount,2);//x is the unknown displacement vector
    Eigen::MatrixXf b=Eigen::MatrixXf::Zero(mStepCount,2);
    int row;

// node 0 - Eq. (1)
    row=0;
    A(row,0)=-h4;
    A(row,1)=-a2;
    A(row,2)=kb;
    A(row,2*N-2)=kb;
    A(row,2*N-1)=-a2;
    b(row,0)=-a1*delta0(0);
    b(row,1)=-a1*delta0(1);
// node 1 - Eq. (2)
    row=1;
    A(row,1)=a1;
    A(row,2)=-a2;
    A(row,3)=kb;
    A(row,2*N-1)=kb;
    b(row,0)=a2*delta0(0);
    b(row,1)=a2*delta0(1);
// node 2 - Eq. (3)
    row=2;
    A(row,1)=-a2;
    A(row,2)=a1;
    A(row,3)=-a2;
    A(row,4)=kb;
    b(row,0)=-kb*delta0(0);
    b(row,1)=-kb*delta0(1);
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
    b(row,0)=-kb*deltaN(0);
    b(row,1)=-kb*deltaN(1);
// node N-1 - Eq. (6)
    row=N-1;
    A(row,N-3)=kb;
    A(row,N-2)=-a2;
    A(row,N-1)=a1;
    A(row,N+1)=kb;
    b(row,0)=a2*deltaN(0);
    b(row,1)=a2*deltaN(1);
// node N - Eq. (7)
    row=N;
    A(row,N-2)=kb;
    A(row,N-1)=-a2;
    A(row,N)=-h4;
    A(row,N+1)=-a2;
    A(row,N+2)=kb;
    b(row,0)=-a1*deltaN(0);
    b(row,1)=-a1*deltaN(1);
// node N+1 - Eq. (8)
    row=N+1;
    A(row,N-1)=kb;
    A(row,N+1)=a1;
    A(row,N+2)=-a2;
    A(row,N+3)=kb;
    b(row,0)=a2*deltaN(0);
    b(row,1)=a2*deltaN(1);
// node N+2 - Eq. (9)
    row=N+2;
    A(row,N+1)=-a2;
    A(row,N+2)=a1;
    A(row,N+3)=-a2;
    A(row,N+4)=kb;
    b(row,0)=-kb*deltaN(0);
    b(row,1)=-kb*deltaN(1);
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
    b(row,0)=-kb*delta0(0);
    b(row,1)=-kb*delta0(1);
// node 2N-1 - Eq. (12)
    row=2*N-1;
    A(row,1)=kb;
    A(row,2*N-3)=kb;
    A(row,2*N-2)=-a2;
    A(row,2*N-1)=a1;
    b(row,0)=a2*delta0(0);
    b(row,1)=a2*delta0(1);

    //ciggj(aa,200,bb);
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
    }
}

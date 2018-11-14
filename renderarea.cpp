#include "renderarea.h"
#include<math.h>
#include<QDebug>

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
        break;
    case Stretch:
        setShapeColor(Qt::green);
        break;
    }
}

QPointF RenderArea::compute_Standard_Ellipse(float t)
{
    float x=cos(t)*mRadius;
    float y=sin(t)*mRadius;
    return QPointF(x,y);
}

void RenderArea::stretch(QPainter &painter)
{
    float step=2*M_PI/mStepCount;

    Eigen::MatrixXf b(mStepCount,2);

//	generate the circle
    for(int i=0;i<mStepCount;i++)
    {
        b(i,0)=-mRadius*cos(i*step);
        b(i,1)=-mRadius*sin(i*step);
    }

    ODEsolver(b);
    /*
//  Cord length between two adjacent nodes
    Lac=pi*d;
    dL=sqrt((xi[1]-xi[0])*(xi[1]-xi[0])+(yi[1]-yi[0])*(yi[1]-yi[0]));
    Lap=dL*2*N;
    printf("Lap,Lac=%le %le\n",Lap,Lac);

    int row=-1;

    for(i=0;i<2*N;i++)
    {
        xi1[i]=xi[i]+x[i];
//		printf("%d %le %le %le\n",i,xi[i],x[i],xi1[i]);
    }

    y1=sqrt((d/2)*(d/2)*2-(d/2-delta0)*(d/2-delta0));
// Same perimeter
    Lap=1.0*Lap;
    y1=1.164; //Lap=9.424398;Lap1=9.424397;

//  103% of the original perimeter (Stetch by 5%)
//	Lap=1.03*Lap;
//    y1=1.2667;  //Lap=9.707122; Lap1=9.707103

//  106% of the original perimeter (Stretch by 10%)
//	Lap=1.06*Lap;
//    y1=1.366713;  //Lap=9.989854; Lap1=9.989853;

    delta0=(d/2)-y1;
    deltaN=-delta0;
    printf("delta0,deltaN= %le %le\n",delta0,deltaN);
    ODEsolver(N,h4,a1,a2,kb,ks,delta0,deltaN,y);
    for(i=0;i<N/2;i++)
    {
        yi1[i]=yi[i]+y[i+3*N/2];
//		printf("%d %le %le %le\n",i,xi[i],x[i],xi1[i]);
    }
    for(i=N/2;i<2*N;i++)
    {
        yi1[i]=yi[i]+y[i-N/2];
//		printf("%d %le %le %le\n",i,xi[i],x[i],xi1[i]);
    }
    Lap1=0.0;
    for(i=1;i<2*N;i++)
    {
        Lap1=Lap1+sqrt((xi1[i]-xi1[i-1])*(xi1[i]-xi1[i-1])+(yi1[i]-yi1[i-1])*(yi1[i]-yi1[i-1]));
    }
    Lap1=Lap1+sqrt((xi1[0]-xi1[2*N-1])*(xi1[0]-xi1[2*N-1])+(yi1[0]-yi1[2*N-1])*(yi1[0]-yi1[2*N-1]));
    printf("Lap,Lap1,y1= %le %le %le\n",Lap,Lap1,y1);
}
*/
}


int RenderArea::ODEsolver(Eigen::MatrixXf &b)
{
    int i,j,row;
    float delta0=-mRadius*2*mStretchPercentage*0.01;
    float deltaN=mRadius*2*mStretchPercentage*0.01;
    float h=1.0/mStepCount;
    float a1=6.0*kb-2.0*ks*h*h;
    float a2=4.0*kb+ks*h*h;
    float h4=h*h*h*h;
    Eigen::MatrixXf aa(mStepCount,mStepCount);

    row=-1;
// node 0 - Eq. (1)
    row=row+1;
    aa(row,0)=-h4;
    aa(row,1)=-a2;
    aa(row,2)=kb;
    aa(row,mStepCount-2)=kb;
    aa(row,mStepCount-1)=-a2;
    b(row)=-a1*delta0;
// node 1 - Eq. (2)
    row=row+1;
    aa(row,1)=a1;
    aa(row,2)=-a2;
    aa(row,3)=kb;
    aa(row,mStepCount-1)=kb;
    b(row)=a2*delta0;
// node 2 - Eq. (3)
    row=row+1;
    aa(row,1)=-a2;
    aa(row,2)=a1;
    aa(row,3)=-a2;
    aa(row,4)=kb;
    b(row)=-kb*delta0;
// for the nodes between node 3 and node N-3 - Eq. (4)
    for(i=3;i<=mStepCount/2-3;i++)
    {
        row=row+1;
        aa(row,i-2)=kb;
        aa(row,i-1)=-a2;
        aa(row,i)=a1;
        aa(row,i+1)=-a2;
        aa(row,i+2)=kb;
    }
// node N-2 - Eq. (5)
    row=row+1;
    aa[row][N-4]=kb;
    aa[row][N-3]=-a2;
    aa[row][N-2]=a1;
    aa[row][N-1]=-a2;
    bb[row]=-kb*deltaN;
// node N-1 - Eq. (6)
    row=row+1;
    aa[row][N-3]=kb;
    aa[row][N-2]=-a2;
    aa[row][N-1]=a1;
    aa[row][N+1]=kb;
    bb[row]=a2*deltaN;
// node N - Eq. (7)
    row=row+1;
    aa[row][N-2]=kb;
    aa[row][N-1]=-a2;
    aa[row][N]=-h4;
    aa[row][N+1]=-a2;
    aa[row][N+2]=kb;
    bb[row]=-a1*deltaN;
// node N+1 - Eq. (8)
    row=row+1;
    aa[row][N-1]=kb;
    aa[row][N+1]=a1;
    aa[row][N+2]=-a2;
    aa[row][N+3]=kb;
    bb[row]=a2*deltaN;
// node N+2 - Eq. (9)
    row=row+1;
    aa[row][N+1]=-a2;
    aa[row][N+2]=a1;
    aa[row][N+3]=-a2;
    aa[row][N+4]=kb;
    bb[row]=-kb*deltaN;
// for the nodes between node N+3 and node 2N-3 - Eq. (10)
    for(i=N+3;i<=2*N-3;i++)
    {
        row=row+1;
        aa[row][i-2]=kb;
        aa[row][i-1]=-a2;
        aa[row][i]=a1;
        aa[row][i+1]=-a2;
        aa[row][i+2]=kb;
    }
// node 2N-2 - Eq. (11)
    row=row+1;
    aa[row][2*N-4]=kb;
    aa[row][2*N-3]=-a2;
    aa[row][2*N-2]=a1;
    aa[row][2*N-1]=-a2;
    bb[row]=-kb*delta0;
// node 2N-1 - Eq. (12)
    row=row+1;
    aa[row][1]=kb;
    aa[row][2*N-3]=kb;
    aa[row][2*N-2]=-a2;
    aa[row][2*N-1]=a1;
    bb[row]=a2*delta0;
    ciggj(aa,200,bb);
    for(i=0;i<200;i++)
    {
        x[i]=bb[i];
    }
    x[0]=delta0;
    x[N]=deltaN;
    */
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
        stretch(painter);
        break;
    }
    default:{
        break;
    }
    }

}

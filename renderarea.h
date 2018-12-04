#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include<QMessageBox>
#include<QDebug>
#include <QList>
#include<QListIterator>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_traits_2.h>
#include <CGAL/basic.h>
#include <CGAL/Cartesian.h>
#include<Eigen/Core>
#include<math.h>

typedef CGAL::Cartesian<double> R;
typedef CGAL::Polygon_traits_2<R> Traits;
typedef Traits::Point_2 Point;
typedef QList<Point> Container;
typedef CGAL::Polygon_2<Traits,Container> Polygon;
//typedef Container::iterator Vertex_iterator;
typedef Polygon::Vertex_iterator Vertex_iterator;
//typedef QListIterator<Point> Vertex_iterator;
#include <QPainter>
#include <QPen>
#include<Eigen/Core>
#include<Eigen/Dense>


class RenderArea : public QWidget
{
    Q_OBJECT//enable the use of signal and slot

public:
    explicit RenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;//Q_DECL_OVERRIDE is a preprocessor macro that expands to the C++11 keyword "override" if the project is built with the C++11 options on and the compiler supports it, or nothing otherwise. The "override" attribute allows the compiler to tell you when you try to overide a virtual function but get the function signature wrong.
    QSize sizeHint() const Q_DECL_OVERRIDE;
    enum ShapeType{Origin,Stretch};
    enum StretchType{Ellipse,Perimeter,Area,Fixed};
    void setShapeColor(QColor color){mShapeColor=color;mPen.setColor(mShapeColor);repaint();} //setter
    QColor getShapeColor() const {return mShapeColor;} //getter
    void setStretchFixedLength(float length){mStretchFixedLength=length;repaint();}
    int getStretchFixedLength()const{return mStretchFixedLength;}
    void setShape(ShapeType shape){mShape=shape;on_shape_changed();}
    int getStep()const{return mStepCount;}
    void setStep(int step){mStepCount=step;repaint();}
    ShapeType shape()const {return mShape;}
    void cleanup();
    void setStretchType(QString str);
    void saveToMaya(QString fileName);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;


signals:

public slots:

private:
    QColor mBackgroundColor;
    QColor mShapeColor;
    ShapeType mShape;
    StretchType mStretchType;
    int mStepCount;
    float mStretchFixedLength;
    float mRadius,ks,kb;
    QPen mPen;

    Point compute_Standard_Ellipse(float t);
    void on_shape_changed();
    float perimeter(Polygon &tmp);
    Polygon updateShape(Polygon& origin,bool isXcoord,Eigen::VectorXf &x);
    void stretch(QPainter &painter);
    void stretchOnY(Polygon &tmp);
    Eigen::VectorXf ODEsolver(float delta0,float deltaN);
    Polygon mTyre;


};

#endif // RENDERAREA_H

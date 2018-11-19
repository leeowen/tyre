#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include<QMessageBox>
#include<QDebug>
#include <QList>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_traits_2.h>
#include <CGAL/basic.h>
#include <CGAL/Cartesian.h>
#include<Eigen/Core>
#include<Eigen/Dense>
#include<math.h>


typedef CGAL::Cartesian<double> R;
typedef CGAL::Polygon_traits_2<R> Traits;
typedef Traits::Point_2 Point;
typedef QList<Point> Container;
typedef CGAL::Polygon_2<Traits,Container> Polygon;
typedef Container::iterator Vertex_iterator;
//typedef Polygon_2::Vertex_iterator VertexIterator;
#include <QPainter>
#include <QPen>
#include<Eigen/Core>
#include<Eigen/Dense>


class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;//Q_DECL_OVERRIDE is a preprocessor macro that expands to the C++11 keyword "override" if the project is built with the C++11 options on and the compiler supports it, or nothing otherwise. The "override" attribute allows the compiler to tell you when you try to overide a virtual function but get the function signature wrong.
    QSize sizeHint() const Q_DECL_OVERRIDE;
    enum ShapeType{Origin,Stretch};
    void setShapeColor(QColor color){mShapeColor=color;mPen.setColor(mShapeColor);repaint();} //setter
    QColor getShapeColor() const {return mShapeColor;} //getter
    void setStretchPercentage(int stretch){mStretchPercentage=stretch;}
    int getStretchPercentage()const{return mStretchPercentage;}
    void setShape(ShapeType shape){mShape=shape;on_shape_changed();}
    int getStep()const{return mStepCount;}
    void setStep(int step){mStepCount=step;repaint();}
    ShapeType shape()const {return mShape;}
    void cleanup();
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;


signals:

public slots:

private:
    QColor mBackgroundColor;
    QColor mShapeColor;
    ShapeType mShape;
    int mStepCount;
    int mStretchPercentage;
    float mRadius,ks,kb;
    QPen mPen;

    Point compute_Standard_Ellipse(float t);
    void on_shape_changed();

    void stretch(QPainter &painter);
    Eigen::Vector2f ODEsolver(float delta0, float deltaN);
    Polygon mTyre;


};

#endif // RENDERAREA_H

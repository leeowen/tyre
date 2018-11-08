#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include<QPainter>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;//Q_DECL_OVERRIDE is a preprocessor macro that expands to the C++11 keyword "override" if the project is built with the C++11 options on and the compiler supports it, or nothing otherwise. The "override" attribute allows the compiler to tell you when you try to overide a virtual function but get the function signature wrong.
    QSize sizeHint() const Q_DECL_OVERRIDE;
    enum ShapeType{Origin,Stretch};
    void setShapeColor(QColor color){mShapeColor=color;repaint();} //setter
    QColor getShapeColor() const {return mShapeColor;} //getter
    void setStretch(int stretch){mStretch=stretch;}
    int getStretch()const{return mStretch;}
    void setShape(ShapeType shape){mShape=shape;on_shape_changed();}
    int getStep()const{return mStepCount;}
    void setStep(int step){mStepCount=step;repaint();}
    ShapeType shape()const {return mShape;}
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;


signals:

public slots:

private:
    QColor mBackgroundColor=QColor(255,255,255);
    QColor mShapeColor;
    ShapeType mShape;
    int mStepCount=100;
    int mStretch;

    QPointF compute_Standard_Ellipse(float t);
    void on_shape_changed();
};

#endif // RENDERAREA_H

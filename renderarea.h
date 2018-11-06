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
    void setShapeColor(QColor color){mShapeColor=color;} //setter
    QColor getShapeColor() const {return mShapeColor;} //getter
    void setShape(ShapeType shape){mShape=shape;}
    ShapeType shape()const {return mShape;}
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;


signals:

public slots:

private:
    QColor mBackgroundColor;
    QColor mShapeColor;
    ShapeType mShape;
};

#endif // RENDERAREA_H

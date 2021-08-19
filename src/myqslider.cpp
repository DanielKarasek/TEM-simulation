#include "myqslider.h"


MyQSlider::MyQSlider(QWidget * parent)
    : QSlider(parent)
{
    this->setMinimumWidth(300);
    this->setMinimum(0);
    this->setMaximum(10000);

}

MyQSlider::MyQSlider(Qt::Orientation orientation, QWidget * parent)
    : QSlider(orientation, parent)
{
    this->setMinimumWidth(300);
    this->setMinimum(0);
    this->setMaximum(10000);
}


double MyQSlider::valueDouble()
{
    int val = dynamic_cast<QSlider*>(this)->value();
    double dd = static_cast<double>(val);
//    dd = dd / (100000 / max - min) + min;
    qDebug() << dd;
    double dif = 1.0 * (max - min) / (10000 - 0);
    dd = min + dif * (dd - 0);
    qDebug() << dd;

    return dd;
}

void MyQSlider::setValueDouble(double val)
{
//    int val = dynamic_cast<QSlider*>(this)->value();
    double dif = 1.0 * (10000 - 0) / (max - min);
    val = 0 + dif * (val - min);
    int dd = static_cast<int>(val);

    dynamic_cast<QSlider*>(this)->setValue(dd);
}

void MyQSlider::setDoubleMinimum(double val)
{
    min = val;
}

void MyQSlider::setDoubleMaximum(double val)
{
    max = val;
}




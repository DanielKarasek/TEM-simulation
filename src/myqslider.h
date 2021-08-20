#ifndef MYQSLIDER_H
#define MYQSLIDER_H

#include <QSlider>
#include <QStyleOptionSlider>
#include <QToolTip>
#include <iostream>
#include <QtDebug>

/*!
 * \brief Třída pro vlastní QSlider.
 * \author Daniel Karásek xkaras38, Filip Osvald, xosval04
 * \details
 * <a href="classMyQSlider.html">Scéna</a>
 * Je podtřídou QSlider. Umožňuje vytvořit vlastní metody pro slider. Tyto metody nastavují minimum a maximum slideru v hodnotach double přepočítané na int.
 * Dále umožňují získat a nastavit hodnotu slideru v hodnotach double. Je to použito při propojení labelů se slidery ve View.
 */
class MyQSlider : public QSlider
{
    Q_OBJECT
public:
    explicit MyQSlider(QWidget *parent = 0);
    explicit MyQSlider(Qt::Orientation orientation, QWidget *parent = 0);
    /*!
     * \brief valueDouble vrací hodnotu sldieru přepočítanou na double
     * \return double
     */
    double valueDouble();

    /*!
     * \brief setValueDouble nastaví hodnotu sldieru přepočítanou z double
     * \param val hodnota double
     */
    void setValueDouble(double val);

    /*!
     * \brief setDoubleMinimum nastaví hodnotu minima pro sldier přepočítanou z double
     * \param val hodnota double
     */
    void setDoubleMinimum(double val);

    /*!
     * \brief setDoubleMaximum nastaví hodnotu maxima pro slider přepočítanou z double
     * \param val hodnota double
     */
    void setDoubleMaximum(double val);

protected:

    double min {-3};
    double max {3};

};

#endif // MYQSLIDER_H

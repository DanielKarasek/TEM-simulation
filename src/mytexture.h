#ifndef MYTEXTURE_H
#define MYTEXTURE_H
#include <QObject>
#include <QVector>
#include <QEntity>


#include <QPainter>
#include <QPaintedTextureImage>
#include <cmath>

#include "typedefs.h"

/*!
 * \brief Třída pro texturu
 * \author Daniel Karásek, xkaras38
 * \details
 * Tato třída aplikuje texturu z dané bitmapy. Používá se na obarvení plošin(planes).
 */

class TextureImage : public Qt3DRender::QPaintedTextureImage
{
public:
  Bitmap m_bitmap;
  bool m_turnedDown;
  /*!
   * \brief TextureImage
   * \param bitmap bitmapa kterou chceme aplikovat
   * \param turnedDown jestli je plošina otočená směrem dolů- na jednu plošinu vykresluji 2 s opačným natočením
   *  (jelikož se mi nepodařilo vypnout backface culling a qt fóra mají pouze mnoho zobrazeno a žádné odpovědi a jedno(ok I solved,)
   *  bez vysvětlení jak :D)
   */
  TextureImage(Bitmap bitmap, bool turnedDown):m_bitmap{bitmap}{
    for (auto &value:m_bitmap){
      value *=255;
    }
    m_turnedDown = turnedDown;
  }
  /*!
   * \brief paint přetížená funkce, která strekturu vykreslí
   * \param painter QPainter používaný na kreslení
   */
  void paint(QPainter* painter)
  {


    double tmp_val;
    int countPerSide = static_cast<int>(std::sqrt(m_bitmap.size()));

    this->setSize(QSize(countPerSide*10, countPerSide*10));

    int offsetCoef = m_turnedDown ? countPerSide*(countPerSide-1) : 0;
    int pos;

    for (int x=0; x<countPerSide; x++){
      for(int y=0; y<countPerSide; y++){
        pos = m_turnedDown ? offsetCoef - y*countPerSide + x : y*countPerSide + x;
        tmp_val = m_bitmap[pos];
        painter->fillRect(x*10, y*10, 10, 10, QColor(tmp_val, tmp_val, tmp_val));
      }
    }
  }
};
#endif // MYTEXTURE_H

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "properties.h"

class Background : public QObject,  public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)

public:
    Background();
    void setBg(BackgroundState theState);

private:
    QTimer * itsAnimationTimer = new QTimer(this);
    int itsBgAnimCounter = 1;
    BackgroundState itsBackgroundState = NULLV;

    qreal x() const;
    qreal y() const;
    QPropertyAnimation * xAnimation = new QPropertyAnimation(this, "x", this);
    qreal m_x;
    QPropertyAnimation * yAnimation = new QPropertyAnimation(this,"y",this);
    qreal m_y;

    void stopAnim();
    void translateTo(const qreal &start, const qreal &yStart, const qreal &xEnd, const qreal &yEnd, const int& duration);
};

#endif // BACKGROUND_H

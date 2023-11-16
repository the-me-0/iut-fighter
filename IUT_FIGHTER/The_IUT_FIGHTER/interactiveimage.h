#ifndef INTERACTIVEIMAGE_H
#define INTERACTIVEIMAGE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsSceneMouseEvent>

class InteractiveImage : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    InteractiveImage(QPixmap pixmap = QPixmap(":/nullimg.png").scaled(QSize(150,150)));

signals:
    void clickOnBtnImg();

public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

#endif // INTERACTIVEIMAGE_H

#include "interactiveimage.h"


InteractiveImage::InteractiveImage(QPixmap pixmap) { setPixmap(QPixmap(pixmap)); }


/* Mouse press button detection on image */
void InteractiveImage::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::RightButton)
        emit clickOnBtnImg();
}

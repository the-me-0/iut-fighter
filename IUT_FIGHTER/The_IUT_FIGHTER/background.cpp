#include "background.h"

Background::Background()
{
    /* Loop to animate some of the fight maps */
    connect(itsAnimationTimer,&QTimer::timeout, this, [=]()
    {
        std::string file;
        switch (itsBackgroundState) {
        case MAPVEGA:
            file = ":/vega" + std::to_string(itsBgAnimCounter) + ".jpg";
            setPixmap(QPixmap(QString::fromStdString(file)).scaled(QSize(3702, 1080)));
            if(itsBgAnimCounter >= 4)
                itsBgAnimCounter = 0;
            break;
        case MAPNOAH:
            file = ":/noah" + std::to_string(itsBgAnimCounter) + ".jpg";
            setPixmap(QPixmap(QString::fromStdString(file)).scaled(QSize(3702, 1080)));
            if(itsBgAnimCounter >= 5)
                itsBgAnimCounter = 0;
            break;
        case MAPANTHO:
            file = ":/AnthonyMAP" + std::to_string(itsBgAnimCounter) + ".png";
            setPixmap(QPixmap(QString::fromStdString(file)).scaled(QSize(3702, 1080)));
            if(itsBgAnimCounter >= 4)
                itsBgAnimCounter = 0;
            break;
        case MAPNILS:
            file = ":/mapNils" + std::to_string(itsBgAnimCounter) + ".png";
            setPixmap(QPixmap(QString::fromStdString(file)).scaled(QSize(3702, 1080)));
            if(itsBgAnimCounter >= 3)
                itsBgAnimCounter = -1;
            break;

        case MAPARTHUR:
            file=":/MAP_ARTHUR" + std::to_string(itsBgAnimCounter) +".jpg";
            setPixmap(QPixmap(QString::fromStdString(file)).scaled(QSize(3702, 1080)));
            if(itsBgAnimCounter >= 4)
                itsBgAnimCounter = 0;
            break;
        }

        itsBgAnimCounter++;
    });
}


/* Initialize background depending on the scene state and stop animation timer if it's unnecessary */
void Background::setBg(BackgroundState theState)
{

    stopAnim();

    itsBackgroundState = theState;

    if(itsAnimationTimer->isActive())
        itsAnimationTimer->stop();
    switch (theState)
    {
    case MAPMAX:
        setPixmap(QPixmap(":/MAPMAX1.jpg").scaled(QSize(3702, 1080)));
        break;
    case MAPANTHO:
        itsAnimationTimer->start(100);
        break;
    case MAPNOAH:
        itsAnimationTimer->start(100);
        break;
    case MAPCYRIL:
        setPixmap(QPixmap(":/CYRILMAP1.jpg").scaled(QSize(3702, 1080)));
        break;
    case MAPARTHUR:
        itsAnimationTimer->start(100);
        break;
    case MAPNILS:
        itsAnimationTimer->start(100);
        break;
    case MAPVEGA: // Test map with test animation
        itsAnimationTimer->start(100);
        break;
    case MENUBACKGROUND:
    {
        QPixmap bgPix = QPixmap(":/menuBackground.png").scaled(QSize(1920*4, 1080*4));
        setPixmap(bgPix);
        // start animation (translation from top left hand corner to bottom right hand corner
        translateTo(-bgPix.width()*0.87,
                    -bgPix.height()*0.87,
                    0-1920/2,
                    0-1080/2,
                    30000);

    }
        break;
    case NULLV:
        setPixmap(QPixmap(":/nullimg.png").scaled(QSize(3702, 1080)));
        break;
    }
}


/*  Stop Animation if it's necessary */
void Background::stopAnim()
{
    if(xAnimation->Running)
        xAnimation->stop();
    if(yAnimation->Running)
        yAnimation->stop();
}


/* Template for translation, set it then start loop */
void Background::translateTo(const qreal &xStart,
                             const qreal &yStart,
                             const qreal &xEnd,
                             const qreal &yEnd,
                             const int &duration)
{

    xAnimation->setStartValue(xStart);
    xAnimation->setEndValue(xEnd);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(duration);
    xAnimation->start();

    yAnimation->setStartValue(yStart);
    yAnimation->setEndValue(yEnd);
    yAnimation->setEasingCurve(QEasingCurve::Linear);
    yAnimation->setDuration(duration);
    yAnimation->start();

    connect(xAnimation, &QPropertyAnimation::finished,[=](){
        xAnimation->start();
    });
    connect(yAnimation, &QPropertyAnimation::finished,[=](){
        yAnimation->start();
    });
}


qreal Background::y() const {return m_y;}
qreal Background::x() const {return m_x;}



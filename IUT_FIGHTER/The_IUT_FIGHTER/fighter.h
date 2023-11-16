#ifndef FIGHTER_H
#define FIGHTER_H

#include "properties.h"

class Fighter : public QObject,  public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_PROPERTY(QPointF coord READ coord WRITE setCoord)

private :
    int itsBaseHP;
    int itsHP;
    int itsPunchAttackCP;
    int itsKickAttackCP;
    int itsRoundWon = 0;       // nb of round won by the fighter (the player)
    int itsSpeed=1;
    int itsPowerNeed = 30;
    int itsPower=0;


    QString itsName = "errorName";

    Direction itsDirection = FACERIGHT;

    QPixmap itsPixmap;

    /* Movements of the fighter */
    qreal moveX = 0;
    QPropertyAnimation * xAnimation = new QPropertyAnimation(this,"x",this);

    qreal moveY = 0;
    QPropertyAnimation * yAnimation = new QPropertyAnimation(this,"y",this);

    // Left & Right
    bool isGoingLeft = false;
    bool isGoingRight = false;

    // Jump
    bool canJump = false;
    bool isJumping = false;
    bool isJumpingRight = false;
    bool isJumpingLeft = false;
    int groundPosition;
    QPointF m_coord;
    QPropertyAnimation * jumpAnimation = new QPropertyAnimation(this,"coord",this);
    int cameraMaxScreenLimit = 3702/2;
    int cameraMinScreenLimit = -3702/2;


    /* Attack of the fighter */
    bool canPunchAttack = true;
    bool canKickAttack = true;
    bool canAttack = true;
    bool legHasHit = false;
    bool legHasNotHit = false;
    bool canReplaceLegSecurity = true;

    QGraphicsPixmapItem * animationAdjustPixmap = nullptr;


    /* Hitbox */
    QGraphicsItemGroup * itsHitbox;
    QGraphicsRectItem * itsHitBoxRectSurface;
    QGraphicsRectItem * itsHitBoxBody;
    QGraphicsRectItem * itsHitBoxLeg;
    QPointF hitboxLegOrigiPos;
    QGraphicsRectItem * itsHitBoxArm;
    QPointF hitboxArmOrigiPos;
    QGraphicsRectItem * itsHitBoxHead;

    QTimer * itsUpdateHitboxTimer = new QTimer(this);

    /* anti-spam -windows pb- */
    AnimationStates itsASAnimation = STATIC;

    /* Sprites animation */
    QTimer * itsAnimTimer = new QTimer(this);
    int animCounter = 0;
    AnimationStates itsAnimationState = STATIC;
    QString AnimationStatesToStr(AnimationStates state);

    // Set to true to show hitbox
    bool showHitbox = false;
    int hitboxSizeRatio = 4;

private slots:
    void updateHitbox();
    void updateAnimation();


public:
    Fighter(int baseHP,
            int punchAttackCP,
            int kickAttackCP,
            const QString &itsName,
            int powerNeed = 30);

    void tank(int damage);

    vector <int> itsCaracteristics;

    virtual void specialAttack() = 0;
    virtual void removeSpecialAttack() = 0;

    /* Movements of the fighter */
    void walk();
    void jump();
    void jumpDirection();

    /* Special Attack fonctions */
    void addPower(int aPower);

    /* Attack of the fighter */
    void punchAttack();
    void kickAttack();
    QString hitMessageIfHit(Fighter * anOpponent);

    bool intersectOtherFighterBody(Fighter * opponent);
    bool fighterArmIntersectOpponent(QGraphicsRectItem *anOpponentMember);
    bool fighterLegIntersectOpponent(QGraphicsRectItem *anOpponentMember);

    /* End of the fight */
    bool isAlive();
    void winnerOrNot(Fighter * anOpponent);
    void displayEndRound(QString name);


    /* Getters */
    int getItsRoundWon() const;
    Direction getItsDirection() const;
    const QString &getItsName() const;
    int getItsBaseHP() const;
    int getItsHP() const;
    int getItsPunchAttackCP() const;
    int getItsKickAttackCP() const;
    bool getIsGoingLeft() const;
    bool getIsGoingRight() const;
    bool getIsJumping() const;
    bool getCanJump() const;
    bool getIsJumpingRight() const;
    bool getIsJumpingLeft() const;
    int getCameraMinScreenLimit() const;
    int getCameraMaxScreenLimit() const;
    bool getCanPunchAttack() const;
    int getItsPowerNeed() const;
    int getItsPower() const;
    QGraphicsRectItem *getItsHitBoxLeg() const;
    QGraphicsRectItem *getItsHitBoxHead() const;
    int getItsPunchAttack() const;
    int getItsKickAttack() const;


    QGraphicsRectItem *getItsHitBoxRectSurface() const;
    QGraphicsRectItem *getItsHitBoxBody() const;

    QPropertyAnimation *getXAnimation() const;
    QPropertyAnimation *getJumpAnimation() const;
    AnimationStates getItsAnimationState() const;

    qreal x() const;
    qreal y() const;
    QPointF coord() const;


    /* Setters */
    void setItsRoundWon(int newRoundWon);
    void setItsDirection(Direction newItsDirection);
    void setIsGoingLeft(bool newIsGoingLeft);
    void setIsGoingRight(bool newIsGoingRight);
    void setIsJumping(bool newIsJumping);
    void setIsJumpingRight(bool newIsJumpingRight);
    void setIsJumpingLeft(bool newIsJumpingLeft);
    void setCanJump(bool newCanJump);
    void setItsHP(int newItsHP);
    void setItsAnimationState(AnimationStates newItsAnimationState);
    void setCameraMinScreenLimit(int newCameraMinScreenLimit);
    void setCameraMaxScreenLimit(int newCameraMaxScreenLimit);
    void setCanAttack(bool newCanAttack);
    void setItsPowerNeed(int powerNeed);
    void setItsSpeed(int newItsSpeed);

    void setX(qreal newX);
    void setY(qreal newY);
    void setCoord(QPointF newCoord);
    void initCoord();
    void setItsPunchAttackCP(int newItsPunchAttackCP);
    void setItsKickAttackCP(int newItsKickAttackCP);
    void setItsPunchAttack(int newItsPunchAttackCP);
    void setItsKickAttack(int newItsKickAttackCP);

};

#endif // FIGHTER_H

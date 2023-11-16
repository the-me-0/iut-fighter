#ifndef KEY_H
#define KEY_H

#include "properties.h"


class Key : public QWidget
{
    Q_OBJECT

private:
    /* Prset attributs */
    QString presetName;

    /* First player keys */
    vector<Qt::Key> itsFirstPlayerKeys;
    Qt::Key jump1;
    Qt::Key crouch1;
    Qt::Key left1;
    Qt::Key right1;
    Qt::Key punch1;
    Qt::Key footKick1;

    /* Second player keys */
    vector<Qt::Key> itsSecondPlayerKeys;
    Qt::Key jump2;
    Qt::Key crouch2;
    Qt::Key left2;
    Qt::Key right2;
    Qt::Key punch2;
    Qt::Key footKick2;

    QFile * fileManager = nullptr;

    /* Initialize the default keys settings */
    void initDefaultConfig();

    /* used to split rode lines in a given vector */
    void split(QChar aChar,
               std::vector<QString> &aStringVector,
               QString aString);

public:
    Key();
    ~Key();

    bool isValidKey(int key);

    /* Translation */
    Qt::Key qStringToKey(QString key);
    QString keyToQString(Qt::Key);

    /* Getters */
    Qt::Key getJump1() const;
    Qt::Key getCrouch1() const;
    Qt::Key getLeft1() const;
    Qt::Key getRight1() const;
    Qt::Key getPunch1() const;
    Qt::Key getFootKick1() const;

    Qt::Key getJump2() const;
    Qt::Key getCrouch2() const;
    Qt::Key getLeft2() const;
    Qt::Key getRight2() const;
    Qt::Key getPunch2() const;
    Qt::Key getFootKick2() const;

    const vector<Qt::Key> &getItsFirstPlayerKeys() const;
    const vector<Qt::Key> &getItsSecondPlayerKeys() const;

public slots:
    void updateKeyPreset(QString aStringPreset);

};

#endif // KEY_H

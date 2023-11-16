#include "key.h"


Key::Key()
{
    fileManager = new QFile("keys.k");
    if (!QFile::exists("keys.k")) {
        initDefaultConfig();
    }

    /* Opening fileManager file to access the data */
    if (fileManager->open(QIODevice::ReadOnly)){
        /* Setting up the saved preset */
        updateKeyPreset(fileManager->readLine());

        /* Closing the fileManager */
        fileManager->close();
    }
}


/* Initialize the default keys settings */
void Key::initDefaultConfig()
{
    if (fileManager !=nullptr && fileManager->open(QIODevice::WriteOnly)){
        fileManager->write("Default config;Z;S;Q;D;SPACE;F;NUM8;NUM5;NUM4;NUM6;NUM0;NUM1");
        fileManager->close();
    }
}


Key::~Key() { delete fileManager; }


/* returns the corresponding qstring of a key */
QString Key::keyToQString(Qt::Key key)
{
    QString str ="UNKNOWN";
    if (key==Qt::Key_A) str =  "A";
    if (key==Qt::Key_B) str = "B";
    if (key==Qt::Key_C) str = "C";
    if (key==Qt::Key_D) str = "D";
    if (key==Qt::Key_E) str = "E";
    if (key==Qt::Key_F) str = "F";
    if (key==Qt::Key_G) str = "G";
    if (key==Qt::Key_H) str = "H";
    if (key==Qt::Key_I) str = "I";
    if (key==Qt::Key_J) str = "J";
    if (key==Qt::Key_K) str = "K";
    if (key==Qt::Key_L) str = "L";
    if (key==Qt::Key_M) str = "M";
    if (key==Qt::Key_N) str = "N";
    if (key==Qt::Key_O) str = "O";
    if (key==Qt::Key_P) str = "P";
    if (key==Qt::Key_Q) str = "Q";
    if (key==Qt::Key_R) str = "R";
    if (key==Qt::Key_S) str = "S";
    if (key==Qt::Key_T) str = "T";
    if (key==Qt::Key_U) str = "U";
    if (key==Qt::Key_V) str = "V";
    if (key==Qt::Key_W) str = "W";
    if (key==Qt::Key_X) str = "X";
    if (key==Qt::Key_Y) str = "Y";
    if (key==Qt::Key_Z) str = "Z";
    if (key==Qt::Key_Space) str = "SPACE";
    if (key==Qt::Key_0) str = "NUM0";
    if (key==Qt::Key_1) str = "NUM1";
    if (key==Qt::Key_2) str = "NUM2";
    if (key==Qt::Key_3) str = "NUM3";
    if (key==Qt::Key_4) str = "NUM4";
    if (key==Qt::Key_5) str = "NUM5";
    if (key==Qt::Key_6) str = "NUM6";
    if (key==Qt::Key_7) str = "NUM7";
    if (key==Qt::Key_8) str = "NUM8";
    if (key==Qt::Key_9) str = "NUM9";
    if (key==Qt::Key_Right) str = "RIGHT";
    if (key==Qt::Key_Left) str = "LEFT";
    if (key==Qt::Key_Up) str = "UP";
    if (key==Qt::Key_Down) str = "DOWN";
    if (key==Qt::Key_Alt) str = "ALT";
    if (key==Qt::Key_AltGr) str = "ALTGR";
    if (key==Qt::Key_Shift) str = "SHIFT";
    if (key==Qt::Key_Tab) str = "TAB";
    /* final case : unknown key or not valid */
    return str;
}


/* returns true if a key is valid, else false */
bool Key::isValidKey(int key)
{
    bool keyIsValid = false;
    std::vector<Qt::Key> validKeys = {Qt::Key_A,Qt::Key_B,Qt::Key_C,Qt::Key_D,Qt::Key_E,
                                      Qt::Key_F,Qt::Key_G,Qt::Key_H,Qt::Key_I,Qt::Key_J,
                                      Qt::Key_K,Qt::Key_L,Qt::Key_M,Qt::Key_N,Qt::Key_O,
                                      Qt::Key_Q,Qt::Key_R,Qt::Key_S,Qt::Key_T,
                                      Qt::Key_U,Qt::Key_V,Qt::Key_W,Qt::Key_X,Qt::Key_Y,
                                      Qt::Key_Z,Qt::Key_Space,Qt::Key_0,Qt::Key_1,Qt::Key_2,
                                      Qt::Key_3,Qt::Key_4,Qt::Key_5,Qt::Key_6,Qt::Key_7,
                                      Qt::Key_8,Qt::Key_9,Qt::Key_Right,Qt::Key_Left,
                                      Qt::Key_Up,Qt::Key_Down,Qt::Key_Alt,Qt::Key_AltGr,Qt::Key_Shift,Qt::Key_Tab};
    for (Qt::Key x : validKeys)
        if (x==key)
            keyIsValid = true;

    return keyIsValid;
}


/* returns the corresponding key */
Qt::Key Key::qStringToKey(QString key)
{
    Qt::Key keyToReturn = Qt::Key_unknown;
    if (key=="A") keyToReturn = Qt::Key_A;
    if (key=="B") keyToReturn = Qt::Key_B;
    if (key=="C") keyToReturn = Qt::Key_C;
    if (key=="D") keyToReturn = Qt::Key_D;
    if (key=="E") keyToReturn = Qt::Key_E;
    if (key=="F") keyToReturn = Qt::Key_F;
    if (key=="G") keyToReturn = Qt::Key_G;
    if (key=="H") keyToReturn = Qt::Key_H;
    if (key=="I") keyToReturn = Qt::Key_I;
    if (key=="J") keyToReturn = Qt::Key_J;
    if (key=="K") keyToReturn = Qt::Key_K;
    if (key=="L") keyToReturn = Qt::Key_L;
    if (key=="M") keyToReturn = Qt::Key_M;
    if (key=="N") keyToReturn = Qt::Key_N;
    if (key=="O") keyToReturn = Qt::Key_O;
    if (key=="Q") keyToReturn = Qt::Key_Q;
    if (key=="R") keyToReturn = Qt::Key_R;
    if (key=="S") keyToReturn = Qt::Key_S;
    if (key=="T") keyToReturn = Qt::Key_T;
    if (key=="U") keyToReturn = Qt::Key_U;
    if (key=="V") keyToReturn = Qt::Key_V;
    if (key=="W") keyToReturn = Qt::Key_W;
    if (key=="X") keyToReturn = Qt::Key_X;
    if (key=="Y") keyToReturn = Qt::Key_Y;
    if (key=="Z") keyToReturn = Qt::Key_Z;
    if (key=="SPACE") keyToReturn = Qt::Key_Space;
    if (key=="NUM0") keyToReturn = Qt::Key_0;
    if (key=="NUM1") keyToReturn = Qt::Key_1;
    if (key=="NUM2") keyToReturn = Qt::Key_2;
    if (key=="NUM3") keyToReturn = Qt::Key_3;
    if (key=="NUM4") keyToReturn = Qt::Key_4;
    if (key=="NUM5") keyToReturn = Qt::Key_5;
    if (key=="NUM6") keyToReturn = Qt::Key_6;
    if (key=="NUM7") keyToReturn = Qt::Key_7;
    if (key=="NUM8") keyToReturn = Qt::Key_8;
    if (key=="NUM9") keyToReturn = Qt::Key_9;
    if (key=="RIGHT") keyToReturn = Qt::Key_Right;
    if (key=="LEFT") keyToReturn = Qt::Key_Left;
    if (key=="UP") keyToReturn = Qt::Key_Up;
    if (key=="DOWN") keyToReturn = Qt::Key_Down;
    if (key=="ALT") keyToReturn = Qt::Key_Alt;
    if (key=="ALTGR") keyToReturn = Qt::Key_AltGr;
    if (key=="SHIFT") keyToReturn = Qt::Key_Shift;
    if (key=="TAB") keyToReturn = Qt::Key_Tab;
    /* final case : unknown key or not valid */
    return keyToReturn;
}


/* Split a string into a vector, between specified character */
void Key::split(QChar aChar, std::vector<QString> &aStringVector, QString aString)
{
    QString tempString = "";
    for (QChar c : aString) {
        if (c != aChar) {
            tempString += c;
        } else {
            aStringVector.push_back(tempString);
            tempString = "";
        }
    }
    aStringVector.push_back(tempString);
}


/* Update the key preset */
void Key::updateKeyPreset(QString aStringPreset)
{
    /* sequence the preset */
    std::vector<QString> lineVector;
    split(';',lineVector,aStringPreset);

    /* Setting the given preset */
    presetName = lineVector[0];
    jump1 = qStringToKey(lineVector[1]);
    crouch1 = qStringToKey(lineVector[2]);
    left1 = qStringToKey(lineVector[3]);
    right1 = qStringToKey(lineVector[4]);
    punch1 = qStringToKey(lineVector[5]);
    footKick1 = qStringToKey(lineVector[6]);
    jump2 = qStringToKey(lineVector[7]);
    crouch2 = qStringToKey(lineVector[8]);
    left2 = qStringToKey(lineVector[9]);
    right2 = qStringToKey(lineVector[10]);
    punch2 = qStringToKey(lineVector[11]);
    footKick2 = qStringToKey(lineVector[12]);

    // Adding keys in a vector for each player
    itsFirstPlayerKeys.clear();

    itsFirstPlayerKeys.push_back(jump1);
    itsFirstPlayerKeys.push_back(crouch1);
    itsFirstPlayerKeys.push_back(left1);
    itsFirstPlayerKeys.push_back(right1);
    itsFirstPlayerKeys.push_back(punch1);
    itsFirstPlayerKeys.push_back(footKick1);

    // Adding keys in a vector for each player
    itsSecondPlayerKeys.clear();

    itsSecondPlayerKeys.push_back(jump2);
    itsSecondPlayerKeys.push_back(crouch2);
    itsSecondPlayerKeys.push_back(left2);
    itsSecondPlayerKeys.push_back(right2);
    itsSecondPlayerKeys.push_back(punch2);
    itsSecondPlayerKeys.push_back(footKick2);

    /* Save the preset in the file */
    fileManager->remove();
    if (fileManager->open(QIODevice::WriteOnly)) {
        QTextStream outStream(fileManager);
        outStream << aStringPreset;
        fileManager->close();
    }
}


/* Getters */
Qt::Key Key::getJump1() const { return jump1; }

Qt::Key Key::getCrouch1() const { return crouch1; }

Qt::Key Key::getLeft1() const { return left1; }

Qt::Key Key::getRight1() const { return right1; }

Qt::Key Key::getPunch1() const { return punch1; }

Qt::Key Key::getFootKick1() const { return footKick1; }

Qt::Key Key::getJump2() const { return jump2; }

Qt::Key Key::getCrouch2() const { return crouch2; }

Qt::Key Key::getLeft2() const { return left2; }

Qt::Key Key::getRight2() const { return right2; }

Qt::Key Key::getPunch2() const { return punch2; }

Qt::Key Key::getFootKick2() const { return footKick2; }

const vector<Qt::Key> &Key::getItsFirstPlayerKeys() const { return itsFirstPlayerKeys; }

const vector<Qt::Key> &Key::getItsSecondPlayerKeys() const { return itsSecondPlayerKeys; }


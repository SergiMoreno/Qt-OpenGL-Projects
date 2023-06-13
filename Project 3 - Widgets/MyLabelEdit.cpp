#include "MyLabelEdit.h"
// constructor
MyLabelEdit::MyLabelEdit(QWidget *parent)
    : QLabel(parent) {
}
// implementació slots

// encarregat de posar "o" al MyLabel i d'emetre una senyal amb igual probabilitat tant
// a esquerra com a dreta
void MyLabelEdit::ballPath() {
    QString K = "o";
    setText(K);
    int i = qrand() % 2;
    if (i == 0) {
        emit sendBallE();
    } else {
        emit sendBallD();
    }
}

// augmenta el nombre de bolles que s'han jugat
void MyLabelEdit::increaseContador() {
    int K = text().toInt();
    setNum(K+1);
}

// posa a 0 el comptador de bolles
void MyLabelEdit::restartContador() {
    setNum(0);
}

// posa "-" al MyLabel i emet una nova senyal de restart
void MyLabelEdit::restartPath() {
    QString K = "-";
    setText(K);
    emit emitRestart();
}

// posa "-" al MyLabel, afegeix un petit delay i emet una nova senyal de remove a un altre MyLabel
void MyLabelEdit::removePath() {
    QString K = "-";
    setText(K);
    QThread::usleep(500);
    emit emitRemove();
}

// en el moment en el que el darrer MyLabel és reseteat, podem iniciar el llançament
// d'una nova bolla, emetent la senyal pathCleaned
void MyLabelEdit::removeLastPath() {
    QString K = "-";
    setText(K);
    QThread::usleep(500);
    emit pathCleaned();
}

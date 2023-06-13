#include <QLabel> 
#include <QRandomGenerator>
#include <QThread>
class MyLabelEdit: public QLabel
{    
    Q_OBJECT
    public:   
        MyLabelEdit (QWidget *parent);
    public slots:      
        void ballPath();
        void increaseContador();
        void restartPath();
        void removePath();
        void removeLastPath();
        void restartContador();
    signals:
        void sendBallE();
        void sendBallD();
        void emitRemove();
        void emitRestart();
        void pathCleaned();
};

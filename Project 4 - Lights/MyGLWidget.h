// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    //virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void carregaShaders ();
    //virtual void viewTransform ();
    virtual glm::mat4 calculaPatricioTG();

  private:
    int printOglError(const char file[], int line, const char func[]);
    
    // método para iniciar los parámetros del foco
    void iniFocus();
    // método para calcular los nuevos valores para los focos de escena y de patricio
    void calculFocus ();
    
    // uniform locations
    GLuint posFocusLoc, colFocusLoc, posFocusLocE, colFocusLocE, posFocusLocP, colFocusLocP;

    //vectores iniciales de las posiciones de los focos de escena y del patricio
    glm::vec3 posFocusPat;
    glm::vec3 posFocusEsc;
    // vectores iniciales de color de focos
    glm::vec3 colFocusCam;
    glm::vec3 colFocusEsc;
    glm::vec3 colFocusPat;
    
    // vector inicial de posicion de patricio
    glm::vec3 posPatricio;
    // valor de rotacion inicial del patricio
    float gradoRotacion;
};

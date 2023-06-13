#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    
    virtual void homerTransform (int nHomer);
    
    virtual void iniCamera ();
    virtual void iniEscena (glm::vec3 min, glm::vec3 max);
    
    virtual void projectTransform ();
    virtual void viewTransform ();

    virtual void mouseMoveEvent(QMouseEvent *event);

  private:
    int printOglError(const char file[], int line, const char func[]);
    
    // Método para volver a setear los valores iniciales
    void reset ();
    
    // Atributos para calcular los parámetros de cámara
    glm::vec3 centre;
    glm::vec3 minC, maxC;
    float angleIni;
    
    // Ángulos para posicionar las figuras de la forma adecuada
    float gradoRotacion, gradoPosicion;
    
    // Atributos iniciales junto con sus mínimos y máximos
    int nHomer, nHomerMax, nHomerMin;
    float radiCercle, radiCercleMax, radiCercleMin;
    
    // Ángulos de Euler
    float psi, theta;
    
    // Modo de la cámara
    bool cameraMode; // if true then perspective else ortho
    
    // Atributos que almacenarán las coordenadas previas x,y del ratón
    float px, py;
};

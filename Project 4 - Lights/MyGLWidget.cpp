// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {

  case Qt::Key_Right:
      if (posPatricio.x < 9.0f) {
          posPatricio.x++;
      }
    break;

  case Qt::Key_Left:
      if (posPatricio.x > 1.0f) {
        posPatricio.x--;
      }
    break;

  case Qt::Key_Up:
      if (posPatricio.z > 1.0f) {
          posPatricio.z--;
      }
    break;

  case Qt::Key_Down:
      if (posPatricio.z < 9.0f) {
          posPatricio.z++;
      }
    break;

  case Qt::Key_R:
      gradoRotacion += float(M_PI)/4.0f;  // Modificamos ángulo de rotación en sentido anti-horario
    break;

  case Qt::Key_C:
      if (colFocusCam.x == 0) { // Encendemos foco cámara
          colFocusCam = glm::vec3 (0.9,0.2,0.2);
      } else {                  // Apagamos
          colFocusCam = glm::vec3 (0,0,0);
      }
      glUniform3fv(colFocusLoc, 1, &colFocusCam[0]);
    break;

  case Qt::Key_E:
      if (colFocusEsc.x == 0) { // Encendemos foco escena
          colFocusEsc = glm::vec3 (0.2,0.9,0.2);
      } else {                  // Apagamos
          colFocusEsc = glm::vec3 (0,0,0);
      }
      glUniform3fv(colFocusLocE, 1, &colFocusEsc[0]);
    break;

  case Qt::Key_P:
      if (colFocusPat.x == 0) { // Encendemos foco patricio
          colFocusPat = glm::vec3 (0.2,0.2,0.9);
      } else {                  // Apagamos
          colFocusPat = glm::vec3 (0,0,0);
      }
      glUniform3fv(colFocusLocP, 1, &colFocusPat[0]);
    break;

  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::initializeGL() {
    // Iniciamos la posición del patricio
    posPatricio = glm::vec3(5,0,2);
    // Inicializamos el ángulo de rotación del patricio a 0º
    gradoRotacion = 0.0f;
    LL4GLWidget::initializeGL();
    carregaShaders();
    iniFocus();
}

void MyGLWidget::iniFocus() {
    colFocusCam = glm::vec3 (0.9,0.2,0.2);
    colFocusEsc = glm::vec3 (0.2,0.9,0.2);

    glm::vec3 posFocusCam = glm::vec3 (0,0,0);
    posFocusEsc = glm::vec3 (8.0,3.0,2.0);

    colFocusPat = glm::vec3 (0.2,0.2,0.9);
    // posicionamos el foco del patricio respecto a la posición de su mano derecha
    posFocusPat = glm::vec3 (centreBasePatr[0] - dimCapsaPatr[0]/2.0f, centreBasePatr[1] + dimCapsaPatr[1]/2.0f, centreBasePatr[2]);

    calculFocus();

    glUniform3fv(posFocusLoc, 1, &posFocusCam[0]);
    glUniform3fv(colFocusLoc, 1, &colFocusCam[0]);
    glUniform3fv(colFocusLocE, 1, &colFocusEsc[0]);
    glUniform3fv(colFocusLocP, 1, &colFocusPat[0]);
}

void MyGLWidget::paintGL() {
    // Recalculamos el valor de los focos de escena y de patricio
    calculFocus();
    LL4GLWidget::paintGL();
}

void MyGLWidget::calculFocus() {
    glm::vec3 posFocusEscCalcul = glm::vec3(View * glm::vec4(posFocusEsc, 1.));
    glm::vec3 posFocusPatCalcul = glm::vec3(View * calculaPatricioTG() * glm::vec4(posFocusPat, 1.));

    glUniform3fv(posFocusLocE, 1, &posFocusEscCalcul[0]);
    glUniform3fv(posFocusLocP, 1, &posFocusPatCalcul[0]);
}

glm::mat4 MyGLWidget::calculaPatricioTG()
{
    // reimplementamos el cálculo de la TG, añadiendo la rotación y con el vector de posPatricio
  glm::mat4 tg = glm::translate(glm::mat4(1.f), posPatricio);
  tg = glm::scale(tg, glm::vec3(escala, escala, escala));
  tg = glm::rotate(tg, gradoRotacion, glm::vec3 (0,1,0));
  tg = glm::translate(tg, -centreBasePatr);
  return tg;
}

void MyGLWidget::carregaShaders() {
    // Obtenemos los respectivos identificadores de los uniform de focos
    posFocusLoc = glGetUniformLocation (program->programId(), "posFocusC");
    colFocusLoc = glGetUniformLocation (program->programId(), "colFocusC");
    posFocusLocE = glGetUniformLocation (program->programId(), "posFocusE");
    colFocusLocE = glGetUniformLocation (program->programId(), "colFocusE");
    posFocusLocP = glGetUniformLocation (program->programId(), "posFocusP");
    colFocusLocP = glGetUniformLocation (program->programId(), "colFocusP");
}

#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

void MyGLWidget::initializeGL ()
{
    // Inicializamos la óptica de la cámara a perpectiva
    cameraMode = true;

    // Inicializamos el ángulo en el que se encuentra el 1r homer a 270º
    gradoPosicion = float(M_PI)+float(M_PI)/2.0f;

    // Inicializamos valores de número de homer
    nHomer = 10;
    nHomerMax = 15;
    nHomerMin = 1;

    // Inicializamos el ángulo de rotación de los homers a 45º
    gradoRotacion = 0.0f;

    // Inicializamos valores círculo
    radiCercle = 4.0f;
    radiCercleMax = 5.0f;
    radiCercleMin = 1.5f;

    //Inicializamos ángulos de Euler
    theta = float(M_PI)/4.0f;
    psi = 0.0f;

    LL2GLWidget::initializeGL();
}

void MyGLWidget::paintGL() {
    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //  glViewport (0, 0, ample, alt);

      // Esborrem el frame-buffer
      glClear (GL_COLOR_BUFFER_BIT);
      for (int i = 0;i < nHomer;i++) {
          // Carreguem la transformació de model

          homerTransform(i);

          // Activem el VAO per a pintar el homer
          glBindVertexArray (VAO_Homer);

          // pintem
          glDrawArrays (GL_TRIANGLES, 0, modelHomer.faces().size() * 3);

          // Aumentamos el ángulo de rotación del homer
          gradoRotacion += (float(M_PI*2))/nHomer;
     }


      terraTransform();

      // Activem el VAO per a pintar el homer
      glBindVertexArray (VAO_Terra);
      // pintem
      glDrawArrays (GL_TRIANGLES, 0, 6);

      glBindVertexArray (0);
}

void MyGLWidget::homerTransform(int i)
{
  // Matriu de transformació de model
  float gradoCirculo = (float(M_PI*2))/nHomer;
  glm::mat4 transform (1.0f);
  // posicionamos el homer correctamente en el círculo
  transform = glm::translate(transform, glm::vec3(radiCercle*cos(gradoPosicion + i*gradoCirculo),0,radiCercle*sin(gradoPosicion + i*gradoCirculo)));
  // trasladamos el homer al centro del círculo
  transform = glm::translate(transform, glm::vec3(midaTerra/2,0,midaTerra/2));
  transform = glm::rotate (transform, (-gradoRotacion), glm::vec3 (0,1,0));
  // altura del homer a 1.83m
  transform = glm::scale(transform, glm::vec3(1, (1.83f/midaModelHomer), 1));
  transform = glm::translate(transform, -baseHomer);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

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

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::iniCamera() {
    iniEscena(glm::vec3 (0,0,0), glm::vec3 (midaTerra,1.83,midaTerra));

    vrp = centre;
    float distance = 2*radiEscena;

    // obtenemos los valores para los parámetros de la cámara con óptica perspectiva
    if (cameraMode == true) {
        angleIni = asin(radiEscena/distance);
        fov = 2*angleIni;
    } else { // obtenemos los valores para los parámetros de la cámara con óptica ortogonal
        obs = vrp + distance*glm::vec3(0,1,0);
        up = glm::vec3(0,0,-1);
    }

    ra = 1.0f;
    znear = distance - radiEscena;
    zfar = distance + radiEscena;

    viewTransform();
    projectTransform();
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);

  if (cameraMode == true) Proj = glm::perspective (fov, ra, znear, zfar);// caso de óptica perspectiva
  else Proj = glm::ortho (-radiEscena, radiEscena, -radiEscena, radiEscena, znear, zfar);        // caso de óptica ortogonal
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);

  // caso de óptica perspectiva
  if (cameraMode == true) {
      View = glm::translate(View, glm::vec3(0,0,-2*radiEscena));
      View = glm::rotate (View, theta, glm::vec3 (1,0,0));
      View = glm::rotate (View, -psi, glm::vec3 (0,1,0));
      View = glm::translate(View, glm::vec3(-vrp));
  } else View = glm::lookAt (obs, vrp, up); // caso de óptica ortogonal

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::iniEscena(glm::vec3 min, glm::vec3 max) {
    centre = 0.5f*(max+min);
    radiEscena = 0.5f*glm::distance(max, min);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
    makeCurrent();
    switch (event->key()) {
    case Qt::Key_Plus:
        if (nHomer < nHomerMax) nHomer++;   // Aumentamos nº de Homers
        break;
    case Qt::Key_Minus:
        if (nHomer > nHomerMin) nHomer--;   // Decrementamos nº de Homers
        break;
    case Qt::Key_Up:
        if (radiCercle < radiCercleMax) radiCercle += 0.5f; // Aumentamos radio del círculo
        break;
    case Qt::Key_Down:
        if (radiCercle > radiCercleMin) radiCercle -= 0.5f; // Decrementamos radio del círculo
        break;
    case Qt::Key_Right:
        gradoRotacion += float(M_PI)/36.0f;
        gradoPosicion += float(M_PI)/36.0f;  // Modificamos ángulo de rotación en sentido horario
        break;
    case Qt::Key_Left:
        gradoRotacion -= float(M_PI)/36.0f;
        gradoPosicion -= float(M_PI)/36.0f;  // Modificamos ángulo de rotación en sentido anti-horario
        break;
    case Qt::Key_C:
        cameraMode = !cameraMode;   // Modificamos la óptica de la cámara
        iniCamera();
        break;
    case Qt::Key_R:
        reset(); // Volvemos a dar los valores iniciales a los atributos existentes
        break;
      default: event->ignore(); break;
    }
    update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();

  xClick = e->x();
  yClick = e->y();

  // Modificamos los ángulos de Euler según el desplazamiento realizado por el usuario mediante el uso del mouse
  if(xClick > px) psi -= (xClick-px)*factorAngleX;
  else if(xClick < px) psi += (xClick-px)*factorAngleX;

  if(yClick > py) theta -= (yClick-py)*factorAngleY;
  else if(yClick < py) theta += (yClick-py)*factorAngleY;

  px = xClick;
  py = yClick;
  viewTransform();

  update ();
}

void MyGLWidget::reset() {
    // Inicializamos la óptica de la cámara a perpectiva
    cameraMode = true;

    // Inicializamos valores de número de homer
    nHomer = 10;

    // Inicializamos el ángulo de rotación de los homers a 45º
    gradoRotacion = 0.0f;

    // Inicializamos el ángulo en el que se encuentra el 1r homer a 270º
    gradoPosicion = float(M_PI)+float(M_PI)/2.0f;

    // Inicializamos valores círculo
    radiCercle = 4.0f;

    //Inicializamos ángulos de Euler
    theta = float(M_PI)/4.0f;
    psi = 0.0f;

    // Volvemos a inicializar los parámetros de la cámara
    iniCamera();
}


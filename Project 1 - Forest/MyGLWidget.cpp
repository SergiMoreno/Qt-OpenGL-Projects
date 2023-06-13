#include "MyGLWidget.h"

#include <iostream>

const float MyGLWidget::midaArbre[NUM_ARBRES] = {
    0.30, 0.25, 0.37, 0.31,
    0.40, 0.42, 0.39, 0.40,
    0.59, 0.65, 0.55, 0.61
};
const glm::vec3 MyGLWidget::posicioArbre[NUM_ARBRES] = {
    glm::vec3(-0.7,0.5,0),
    glm::vec3(-0.3,0.6,0),
    glm::vec3( 0.2,0.4,0),
    glm::vec3( 0.5,0.5,0),
    glm::vec3(-0.8, 0.05,0),
    glm::vec3(-0.3,-0.1,0),
    glm::vec3( 0.0, 0.1,0),
    glm::vec3( 0.6, 0.0,0),
    glm::vec3(-0.6,-0.7,0),
    glm::vec3(-0.2,-0.9,0),
    glm::vec3( 0.3,-0.6,0),
    glm::vec3( 0.7,-0.8,0),
};
const glm::vec3 MyGLWidget::colorArbre[NUM_ARBRES] = {
    glm::vec3(0.1, 0.5, 0.1),
    glm::vec3(0, 0.4, 0.2),
    glm::vec3(0.15, 0.5, 0.15),
    glm::vec3(0.2, 0.4, 0.2),
    glm::vec3(0.3, 0.4, 0.2),
    glm::vec3(0.3, 0.5, 0.25),
    glm::vec3(0.25, 0.5, 0.35),
    glm::vec3(0, 0.5, 0),
    glm::vec3(0.24, 0.35, 0.24),
    glm::vec3(0, 0.4, 0),
    glm::vec3(0, 0.4, 0.25),
    glm::vec3(0.15, 0.4, 0.25)
};
const glm::vec3 MyGLWidget::colorTronc(0.6, 0.38, 0.25);


MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  mode = true;  // Inicialitzam la copa amb la seva forma triangular
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersTronc();
  creaBuffersCopa();
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);

  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Bucle per dibuixar els diferents arbres que formen part del bosc
  for (int i = 0;i < NUM_ARBRES;i++) {
    modelTransformArbre(posicioArbre[i], midaArbre[i]);

    // Pintem tronc
    glBindVertexArray(VAOTronc);
    glUniform3fv (colorLoc,1,&colorTronc[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Pintem copa
    glUniform3fv (colorLoc,1,&colorArbre[i][0]);
    // Condicional en funció de si la copa a ser dibuixada ha de ser triangular o quadrada
    if (mode == true) {
        glBindVertexArray(VAOCopa[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    } else {
        glBindVertexArray(VAOCopa[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
      case Qt::Key_C :
        mode = !mode;
      break;
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::modelTransformArbre (const glm::vec3& posicio, float mida)
{
  // Codi per a la TG necessària
  glm::mat4 TG(1.0);
  TG = glm::translate (TG, posicio);    // Traslladam cada objecte a la posició que li pertoqui
  TG = glm::scale (TG, glm::vec3 (mida/midaModel));    // Aplicam l'escalat a cada objecte en funció de l'altura inicial del model
  TG = glm::translate (TG, glm::vec3 (0, offsetBase, 0));   // Traslladam l'objecte al centre
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::creaBuffersTronc ()
{
  glm::vec3 Vertices[6];  // vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-0.1, -0.8, 0);
  Vertices[1] = glm::vec3( 0.1, -0.8, 0);
  Vertices[2] = glm::vec3(-0.1,  0.0, 0);
  Vertices[3] = glm::vec3( 0.1,  0.0, 0);
  Vertices[4] = glm::vec3(-0.1,  0.0, 0);
  Vertices[5] = glm::vec3( 0.1, -0.8, 0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el tronc
  glGenVertexArrays(1, &VAOTronc);
  glBindVertexArray(VAOTronc);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersCopa ()
{
  glm::vec3 VerticesT[3];  // Quatre vèrtexs amb X, Y i Z
  VerticesT[0] = glm::vec3( 0, 0.8, 0);
  VerticesT[1] = glm::vec3(-0.4, -0.4, 0);
  VerticesT[2] = glm::vec3( 0.4, -0.4, 0);

  glm::vec3 VerticesQ[6];  // Set vèrtexs amb X, Y i Z
  VerticesQ[0] = glm::vec3(-0.5, 0.8, 0);
  VerticesQ[1] = glm::vec3(-0.5, -0.2, 0);
  VerticesQ[2] = glm::vec3( 0.5, -0.2, 0);

  VerticesQ[3] = glm::vec3( 0.5, 0.8, 0);
  VerticesQ[4] = glm::vec3(-0.5, 0.8, 0);
  VerticesQ[5] = glm::vec3( 0.5, -0.2, 0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar la copa
  glGenVertexArrays(2, &VAOCopa[0]);
  // Copa triangular
  glBindVertexArray(VAOCopa[0]);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO[2];
  glGenBuffers(2, &VBO[0]);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesT), VerticesT, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);

  // Copa quadrada
  glBindVertexArray(VAOCopa[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesQ), VerticesQ, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");

  // Obtenim els identificadors dels uniforms
  colorLoc = glGetUniformLocation (program->programId(), "color");
  transLoc = glGetUniformLocation (program->programId(), "TG");
}

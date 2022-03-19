#include "window.h"

#include <QGuiApplication>
#include <QApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>

#include "mainwindow.h"

class HyperboloidWindow : public OpenGLWindow
{
public:
    using OpenGLWindow::OpenGLWindow;

    void initialize() override;
    void render() override;
    void createInterface ();

private:
    GLint m_posAttr = 0;
    GLint m_normalAttr = 0;
    GLint m_matrixUniform = 0;
    GLint m_lightPosUniform = 0;
    GLint m_surfaceRGBUniform = 0;
    GLint m_ambientLightRGBUniform = 0;
    GLint m_lightSourceRGBUniform = 0;
    GLint m_highlightRGBUniform = 0;
    GLint m_time = 0;

    QOpenGLShaderProgram *m_program = nullptr;
    int m_frame = -1;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QSurfaceFormat format;
    HyperboloidWindow window;

    window.setFormat(format);

    MainWindow mw;
    QObject::connect (&mw, SIGNAL (angleXchanged (float)), &window, SLOT (angleXchanged (float)));
    QObject::connect (&mw, SIGNAL (angleYchanged (float)), &window, SLOT (angleYchanged (float)));
    QObject::connect (&mw, SIGNAL (angleZchanged (float)), &window, SLOT (angleZchanged (float)));
    QObject::connect (&mw, SIGNAL (lightPosXchanged (float)), &window, SLOT (lightPosXchanged (float)));
    QObject::connect (&mw, SIGNAL (lightPosYchanged (float)), &window, SLOT (lightPosYchanged (float)));
    QObject::connect (&mw, SIGNAL (lightPosZchanged (float)), &window, SLOT (lightPosZchanged (float)));
    QObject::connect (&mw, SIGNAL (approxChanged (int)), &window, SLOT (approxChanged (int)));
    QObject::connect (&mw, SIGNAL (animationOn (bool)), &window, SLOT (animationOn (bool)));

    QWidget * widget = QWidget::createWindowContainer (&window, &mw);
    widget->resize(800, 600);
    mw.show ();
    widget->show ();

    window.setAnimating(true);

    return app.exec();
}

static const char *vertexShaderSource =
"attribute vec4 posAttr;\n"
"attribute vec4 normalAttr;\n"
"varying vec3 n;\n"
"varying vec3 v;\n"
"uniform mat4 matrix;\n"
"void main() {\n"
"   v = (matrix * posAttr).xyz;\n"
"   n = normalize ((matrix * normalAttr).xyz);\n"
"   gl_Position = vec4 (v, 1.0);\n"
"};\n";

static const char *fragmentShaderSource =
"varying vec3 n;\n"
"varying vec3 v;\n"
"uniform highp vec3 lightPos;\n"
"uniform vec3 surfaceRGB;\n"
"uniform vec3 ambientLightRGB;\n"
"uniform vec3 lightSourceRGB;\n"
"uniform vec3 highlightRGB;\n"
"uniform float time;\n"
"void main() {\n"
"   vec3 l = normalize (lightPos - v);\n"
"   vec3 col;\n"
"   float sc = max (dot (n, l), 0.0);\n"
"   float r = max (dot (2.0 * n * sc - l, l), 0.0);\n"
"   r = pow (r, 10.0);\n"
"   if (time != -1.0) {\n"
"      ambientLightRGB *= sin (time);\n"
"   }\n"
"   col.x = 1.0 * ambientLightRGB.x * surfaceRGB.x + 2.0 * lightSourceRGB.x * surfaceRGB.x * sc + highlightRGB.x * lightSourceRGB.x * r;\n"
"   col.y = 1.0 * ambientLightRGB.y * surfaceRGB.y + 2.0 * lightSourceRGB.y * surfaceRGB.y * sc + highlightRGB.y * lightSourceRGB.y * r;\n"
"   col.z = 1.0 * ambientLightRGB.z * surfaceRGB.z + 2.0 * lightSourceRGB.z * surfaceRGB.z * sc + highlightRGB.z * lightSourceRGB.z * r;\n"
"   gl_FragColor = vec4 (col, 1.0);\n"
"}\n";

void HyperboloidWindow::initialize()
{

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);

    m_normalAttr = m_program->attributeLocation("normalAttr");
    Q_ASSERT(m_normalAttr != -1);
    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
    m_lightPosUniform = m_program->uniformLocation("lightPos");
    Q_ASSERT(m_lightPosUniform != -1);
    m_surfaceRGBUniform = m_program->uniformLocation("surfaceRGB");
    Q_ASSERT(m_surfaceRGBUniform != -1);
    m_ambientLightRGBUniform = m_program->uniformLocation("ambientLightRGB");
    Q_ASSERT(m_ambientLightRGBUniform != -1);
    m_lightSourceRGBUniform = m_program->uniformLocation("lightSourceRGB");
    Q_ASSERT(m_lightSourceRGBUniform != -1);
    m_highlightRGBUniform = m_program->uniformLocation("highlightRGB");
    Q_ASSERT(m_highlightRGBUniform != -1);
    m_time = m_program->uniformLocation("time");
    Q_ASSERT(m_time != -1);
}

void HyperboloidWindow::render()
{
    if (animationTurnedOn) {
        ++ m_frame;
    }
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix1, matrix2, matrix3, matrix;
    matrix1.rotate (m_anglex, 1, 0, 0);
    matrix2.rotate (m_angley, 0, 1, 0);
    matrix3.rotate (m_anglez, 0, 0, 1);
    matrix = matrix3 * matrix2 * matrix1;

    m_program->setUniformValue(m_matrixUniform, matrix);
    m_program->setUniformValue(m_lightPosUniform, lightPos);
    m_program->setUniformValue(m_surfaceRGBUniform, QVector3D (0.1, 0.1, 0.5));
    m_program->setUniformValue(m_ambientLightRGBUniform, QVector3D (0.0, 0.0, 0.3));
    m_program->setUniformValue(m_lightSourceRGBUniform, QVector3D (0.784, 0.784, 0.392));
    m_program->setUniformValue(m_highlightRGBUniform, QVector3D (1.0, 1.0, 1.0));
    float time;
    if (animationTurnedOn) {
        time = 1.0f * m_frame / screen()->refreshRate();
    }
    else {
        time = - 1.0f;
    }
    m_program->setUniformValue(m_time, time);

    void * verticesArray = hb.GetVerticesArray ();

    void * normalsArray = hb.GetNormalsArray ();
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, verticesArray);
    glVertexAttribPointer (m_normalAttr, 3, GL_FLOAT, GL_FALSE, 0, normalsArray);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_normalAttr);

    int * indicesArray = hb.GetIndicesArray ();
    glDrawElements (GL_TRIANGLE_FAN, hb.approximation * 2 + 4, GL_UNSIGNED_INT, indicesArray);
    for (int i = 0; i < hb.approximation - 1; i ++) {
        int mShift = hb.approximation * 2 + 4 + i * ((hb.approximation * 2 + 2) * 2 + 2);
        glDrawElements (GL_QUAD_STRIP, (hb.approximation * 2 + 2) * 2 + 2, GL_UNSIGNED_INT, indicesArray + mShift);
    }
    int mShift = hb.approximation * 2 + 4 + (hb.approximation - 1) * ((hb.approximation * 2 + 2) * 2 + 2);
    glDrawElements (GL_TRIANGLE_FAN, hb.approximation * 2 + 4, GL_UNSIGNED_INT, indicesArray + mShift);

    glDisableVertexAttribArray(m_normalAttr);
    glDisableVertexAttribArray(m_posAttr);

    m_program->release();
}

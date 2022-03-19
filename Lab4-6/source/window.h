#include <QWindow>
#include <QOpenGLFunctions>

#include <QVector3D>
#include "hyperboloid.h"

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = nullptr);
    ~OpenGLWindow();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

    float m_anglex;
    float m_angley;
    float m_anglez;
    QVector3D lightPos;
    Hyperboloid hb;
    bool animationTurnedOn;


public slots:
    void renderLater();
    void renderNow();
    void angleXchanged (float angleX) {
        m_anglex = angleX;
        //render ();
    }
    void angleYchanged (float angleY) {
        m_angley = angleY;
        //render ();
    }
    void angleZchanged (float angleZ) {
        m_anglez = angleZ;
        //render ();
    }
    void lightPosXchanged (float posX) {
        lightPos.setX (posX);
    }
    void lightPosYchanged (float posY) {
        lightPos.setY (posY);
    }
    void lightPosZchanged (float posZ) {
        lightPos.setZ (posZ);
    }
    void approxChanged (int approx) {
        hb = Hyperboloid (approx);
    }
    void animationOn (bool turnedOn) {
        animationTurnedOn = turnedOn;
    }

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;

private:
    bool m_animating = false;

    QOpenGLContext *m_context = nullptr;
    QOpenGLPaintDevice *m_device = nullptr;
};

#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QDebug>
#include <QVector>
#include <QtMath>
#include <QMatrix4x4>
#include <QVector4D>


QVector <QPair <int, int>> pointArray;
double siz;
double angle = 0.0;
int xx = 150;

int apro = 100;
float step = 1.0f / apro;

QVector <int> vx = {100, 100, 200, 200, 500, 400};
QVector <int> vy = {100, 400, 200, 500, 100, 300};

/**/
int x1 = 100, z1 = 100;
int x2 = 100, y2 = 400;
int x3 = 200, y3 = 200;
int x4 = 200, y4 = 500;
int x5 = 500, y5 = 100;
int x6 = 400, y6 = 300;
/**/
bool back = false;

QMatrix4x4 createBasisMatrix()
       {
            QMatrix4x4 res (1, -3,  3, -1,
                            4,  0, -6,  3,
                            1,  3,  3, -3,
                            0,  0,  0,  1);
            return res / 6.0;
        }

QVector4D point4D(int x, int y){
    return QVector4D(x, y, 0, 0);
}

QMatrix4x4 ConvectToMatrix(QVector4D v1, QVector4D v2, QVector4D v3, QVector4D v4){
    QMatrix4x4 res (v1.x(), v2.x(), v3.x(), v4.x(),
                    v1.y(), v2.y(), v3.y(), v4.y(),
                    v1.z(), v2.z(), v3.z(), v4.z(),
                    v1.w(), v2.w(), v3.w(), v4.w());
    return res;
}

void getPointArray ();

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    if (this->width () - xx < this->height ()) {
        siz = (float)(this->width () - xx) / 100;
    }
    else {
        siz = (float)this->height() / 100;
    }
    getPointArray ();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::resizeEvent (QResizeEvent * e) {
    Q_UNUSED (e);
    if (this->width () - xx < this->height ()) {
        siz = (float)(this->width () - xx) / 100;
    }
    else {
        siz = (float)this->height () / 100;
    }
    getPointArray ();
    this->update();
}

void Widget::paintEvent (QPaintEvent * e){
    Q_UNUSED (e);
    QPainter qp (this);
    QPen pen (Qt::black, 2, Qt::SolidLine);
    qp.setPen (pen);
    getPointArray ();
    draw (&qp);

}

void Widget::draw (QPainter * qp) {
    QPen pen (Qt::black, 2, Qt::SolidLine);
    qp->setPen (pen);
    /**/
    if (!back){
        for (int i = 0; i < 5; i++){
            qp->drawLine (pointArray [i].first,
                          pointArray [i].second,
                          pointArray [i + 1].first,
                          pointArray [i + 1].second);
        }
    }
    pen = QPen (Qt::red, 10, Qt::SolidLine);
    qp->setPen (pen);
    for (int i = 0; i < 6; i++){
        qp->drawLine (pointArray [i].first,
                      pointArray [i].second,
                      pointArray [i].first,
                      pointArray [i].second);
    }
    pen = QPen (Qt::green, 4, Qt::SolidLine);
    qp->setPen (pen);
    for (int i = 6; i < apro*3+5; i++){
        qp->drawLine (pointArray [i].first,
                      pointArray [i].second,
                      pointArray [i+1].first,
                      pointArray [i+1].second);
    }
    /**/
}

void getPointArray () {
    //int shiftX = (width - xx) / 2 + xx, shiftY = height / 2;
    pointArray.clear();
    for (int i = 0; i < 6; i++){
        pointArray.append (QPair <int, int> (xx+vx[i], vy[i]));
    }
    QMatrix4x4 base = createBasisMatrix();
    for (int i = 0; i < 6 - 3; i++){
        QVector4D p0 = point4D(vx[i], vy[i]);
        QVector4D p1 = point4D(vx[i+1], vy[i+1]);
        QVector4D p2 = point4D(vx[i+2], vy[i+2]);
        QVector4D p3 = point4D(vx[i+3], vy[i+3]);
        QMatrix4x4 P = ConvectToMatrix(p0, p1, p2, p3);
        QMatrix4x4 tmp = P * base;
        for (int j = 0; j < apro; j++){
            float t = step*j;
            QVector4D T (1, t, pow(t, 2), pow(t, 3));
            QVector4D res = tmp*T;
            //qDebug() << res.x() << " " << res.y() << " " << res.z();
            pointArray.append (QPair <int, int> (xx+res.x(), res.y()));
        }
    }

}

void Widget::on_spinBox_valueChanged(int arg1)
{
    vx[0] = arg1;
    this->update();
}


void Widget::on_spinBox_2_valueChanged(int arg1)
{
    vy[0] = arg1;
    this->update();
}


void Widget::on_spinBox_4_valueChanged(int arg1)
{
    vx[1] = arg1;
    this->update();
}


void Widget::on_spinBox_3_valueChanged(int arg1)
{
    vy[1] = arg1;
    this->update();
}


void Widget::on_spinBox_6_valueChanged(int arg1)
{
    vx[2] = arg1;
    this->update();
}


void Widget::on_spinBox_5_valueChanged(int arg1)
{
    vy[2] = arg1;
    this->update();
}


void Widget::on_spinBox_8_valueChanged(int arg1)
{
    vx[3] = arg1;
    this->update();
}


void Widget::on_spinBox_7_valueChanged(int arg1)
{
    vy[3] = arg1;
    this->update();
}



void Widget::on_spinBox_10_valueChanged(int arg1)
{
    vx[4] = arg1;
    this->update();
}


void Widget::on_spinBox_11_valueChanged(int arg1)
{
    vy[4] = arg1;
    this->update();
}


void Widget::on_spinBox_9_valueChanged(int arg1)
{
    vx[5] = arg1;
    this->update();
}


void Widget::on_spinBox_12_valueChanged(int arg1)
{
    vy[5] = arg1;
    this->update();
}


void Widget::on_checkBox_clicked(bool checked)
{
    back = checked;
    this->update();
}


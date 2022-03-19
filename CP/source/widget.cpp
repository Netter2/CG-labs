#include "widget.h"
#include "ui_widget.h"

QVector <QVector4D> updots (51);
QVector <QVector4D> downdots (51);
QVector4D P1 (0, 5, -5, 1);
QVector4D P2 (0, 6, 0, 1);
QVector4D P3 (0, 5, 5, 1);
QVector <QVector4D> curve (51);
int approximation = 10;
int xshift;
int yshift;
bool invis = 1;
bool grid = 0;
QVector4D upcenter (0, 0, 100, 1);
QVector4D downcenter (0, 0, -100, 1);
QPointF p1;
QPointF p2;
QPointF p3;
QPointF p4;
QPointF p5;
QPointF p6;
QPointF p7;
int ambient = 0;
QMatrix4x4 work;
QMatrix4x4 scalematrix;
QMatrix4x4 xrotmatrix;
QMatrix4x4 yrotmatrix;
QMatrix4x4 zrotmatrix;
QMatrix4x4 blank (1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1);

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent (QPaintEvent * e) {
    Q_UNUSED(e);
    QPainter qp(this);
    QPen pen(Qt::black, 1, Qt::SolidLine);
    QPen pen2(Qt::red, 3, Qt::SolidLine);
    QPen pen3(Qt::blue, 10, Qt::SolidLine);
    //qp.setBrush(Qt::green);
    qp.setPen(pen);


    float step = 1.0 / approximation;
    for (int i = 0; i < approximation + 1; i ++) {
        float t = step * i;
        curve [i] = pow ((1 - t), 2) * P1 + 2 * (1 - t) * P2 + t * t * P3;
    }



    for (int j = 0; j < approximation; j ++) {
        work = blank * scalematrix * xrotmatrix * yrotmatrix * zrotmatrix;
        upcenter = curve [j + 1];
        downcenter = curve [j];

        for (int i = 0; i <= approximation; i ++) {
            updots [i].setX(2 * pow (sin(i * 2 * M_PI / approximation), 3) + curve [j + 1].x ());
            updots [i].setY(2 * pow (cos(i * 2 * M_PI / approximation), 3) + curve [j + 1].y ());
            updots [i].setZ(curve [j + 1].z ());
            updots [i].setW(1);
            downdots [i].setX(2 * pow (sin(i * 2 * M_PI / approximation), 3) + curve [j].x ());
            downdots [i].setY(2 * pow (cos(i * 2 * M_PI / approximation), 3) + curve [j].y ());
            downdots [i].setZ(curve [j].z ());
            downdots [i].setW(1);
        }
        float scalar;
        p1 = QPointF ((updots [0] * work).x() + xshift, (updots [0] * work).y() + yshift);
        p2 = QPointF ((updots [1] * work).x() + xshift, (updots [1] * work).y() + yshift);
        p3 = QPointF ((updots [2] * work).x() + xshift, (updots [2] * work).y() + yshift);
        QVector3D v ((p2 - p1).x(), (p2 - p1).y(), ((updots [1] - updots [0]) * work).z());
        QVector3D w ((p3 - p2).x(), (p3 - p2).y(), ((updots [2] - updots [1]) * work).z());
        QVector4D vmult (v.y() * w.z() - v.z() * w.y(),
                         v.z() * w.x() - v.x() * w.z(),
                         v.x() * w.y() - v.y() * w.x(),
                         1);
        //vmult *= -1;
        v = QVector3D ((curve [j + 1] - curve [j]).x(), (curve [j + 1] - curve [j]).y(), (curve [j + 1] - curve [j]).z());
        w = QVector3D (vmult.x(), vmult.y(), vmult.z());
        v.normalize();
        w.normalize();
        float xf = acos (v.x() * w.x());
        float yf = acos (v.y() * w.y());
        float zf = acos (v.z() * w.z());
        QMatrix4x4 xfm (1, 0,        0,          0,
                        0, cos (xf), - sin (xf), 0,
                        0, sin (xf), cos (xf),   0,
                        0, 0,        0,          1);
        work *= xfm;
        QMatrix4x4 yfm (cos (yf),   0, sin (yf), 0,
                        0,             1, 0,           0,
                        - sin (yf), 0, cos (yf), 0,
                        0,             0, 0,           1);
        work *= yfm;
        QMatrix4x4 zfm (cos (zf), - sin (zf), 0, 0,
                        sin (zf), cos (zf),   0, 0,
                        0,           0,             1, 0,
                        0,           0,             0, 1);
        work *= zfm;
        /*vmult = QVector4D (v.y() * w.z() - v.z() * w.y(),
                           v.z() * w.x() - v.x() * w.z(),
                           v.x() * w.y() - v.y() * w.x(),
                           1);
        float f = acos (v.x() * w.x() + v.y() * w.y() + v.z() * w.z());

        QMatrix4x4 rotationMatrix = QMatrix4x4 (
                        cos (f) + (1 - cos (f)) * vmult.x() * vmult.x()    , (1 - cos (f)) * vmult.x() * vmult.y() - vmult.z() * sin (f), (1 - cos (f)) * vmult.x() * vmult.z() + vmult.y() * sin (f), 0,
                        (1 - cos (f)) * vmult.y() * vmult.x() + vmult.z() * sin (f), cos (f) + (1 - cos (f)) * vmult.y() * vmult.y()    , (1 - cos (f)) * vmult.y() * vmult.z() - vmult.x() * sin (f), 0,
                        (1 - cos (f)) * vmult.z() * vmult.x() - vmult.y() * sin (f), (1 - cos (f)) * vmult.z() * vmult.y() + vmult.x() * sin (f), cos (f) + (1 - cos (f)) * vmult.z() * vmult.z()    , 0,
                        0                                  , 0                                  , 0                                  , 1);


        work *= rotationMatrix;*/
        for (int i = 0; i < approximation; i ++) {
            p1 = QPointF ((updots [i] * work).x() + xshift, (updots [i] * work).y() + yshift);
            p2 = QPointF ((updots [i + 1] * work).x() + xshift, (updots [i + 1] * work).y() + yshift);
            p3 = QPointF ((downdots [i + 1] * work).x() + xshift, (downdots [i + 1] * work).y() + yshift);
            p4 = QPointF ((downdots [i] * work).x() + xshift, (downdots [i] * work).y() + yshift);
            p5 = QPointF ((upcenter * work).x() + xshift, (upcenter * work).y() + yshift);
            p6 = QPointF ((downcenter * work).x() + xshift, (downcenter * work).y() + yshift);

            qp.setPen(pen);
            scalar = ((p2.x() - p1.x ()) * (p3.y() - p2.y()) - (p2.y() - p1.y()) * (p3.x() - p2.x()));

            if (((scalar > 0) && (invis)) || (!invis)) {
                QPointF points [4] = {p1, p2, p3, p4};
                qp.drawPolygon (points, 4);
            }



            /*scalar = ((p5.x() - p1.x ()) * (p2.y() - p5.y()) - (p5.y() - p1.y()) * (p2.x() - p5.x()));
            if (((scalar > 0) && (invis)) || (!invis)) {
                QPointF points [3] = {p1, p5, p2};
                qp.drawPolygon (points, 3);
            }

            scalar = ((p6.x() - p3.x ()) * (p4.y() - p6.y()) - (p6.y() - p3.y()) * (p4.x() - p6.x()));
            if (((scalar > 0) && (invis)) || (!invis)) {
                QPointF points [3] = {p4, p3, p6};
                qp.drawPolygon (points, 3);
            }*/
        }

        /*p1 = QPointF ((updots [0] * work).x() + xshift, (updots [0] * work).y() + yshift);
        p2 = QPointF ((updots [1] * work).x() + xshift, (updots [1] * work).y() + yshift);
        p3 = QPointF ((updots [2] * work).x() + xshift, (updots [2] * work).y() + yshift);

        QPointF points [approximation];
        QVector3D v ((p2 - p1).x(), (p2 - p1).y(), ((updots [1] - updots [0]) * work).z());
        QVector3D w ((p3 - p2).x(), (p3 - p2).y(), ((updots [2] - updots [1]) * work).z());
        QVector4D vmult (v.y() * w.z() - v.z() * w.y(),
                         v.z() * w.x() - v.x() * w.z(),
                         v.x() * w.y() - v.y() * w.x(),
                         1);
        v = QVector3D (vmult.x(), vmult.y(), vmult.z());
        w = QVector3D ((curve [j + 1] - curve [j]).x(), (curve [j + 1] - curve [j]).y(), (curve [j + 1] - curve [j]).z());
        v.normalize();
        w.normalize();
        vmult = QVector4D (v.y() * w.z() - v.z() * w.y(),
                           v.z() * w.x() - v.x() * w.z(),
                           v.x() * w.y() - v.y() * w.x(),
                           1);
        float f = acos (v.x() * w.x() + v.y() * w.y() + v.z() * w.z());

        QMatrix4x4 rotationMatrix = QMatrix4x4 (
                        cos (f) + (1 - cos (f)) * vmult.x() * vmult.x()    , (1 - cos (f)) * vmult.x() * vmult.y() - vmult.z() * sin (f), (1 - cos (f)) * vmult.x() * vmult.z() + vmult.y() * sin (f), 0,
                        (1 - cos (f)) * vmult.y() * vmult.x() + vmult.z() * sin (f), cos (f) + (1 - cos (f)) * vmult.y() * vmult.y()    , (1 - cos (f)) * vmult.y() * vmult.z() - vmult.x() * sin (f), 0,
                        (1 - cos (f)) * vmult.z() * vmult.x() - vmult.y() * sin (f), (1 - cos (f)) * vmult.z() * vmult.y() + vmult.x() * sin (f), cos (f) + (1 - cos (f)) * vmult.z() * vmult.z()    , 0,
                        0                                  , 0                                  , 0                                  , 1);
        p1 = QPointF ((updots [0] * work * rotationMatrix).x() + xshift, (updots [0] * work).y() + yshift);
        p2 = QPointF ((updots [1] * work * rotationMatrix).x() + xshift, (updots [1] * work).y() + yshift);
        p3 = QPointF ((updots [2] * work * rotationMatrix).x() + xshift, (updots [2] * work).y() + yshift);
        //scalar = ((p2.x() - p1.x()) * (p3.y() - p2.y()) - (p2.y() - p1.y()) * (p3.x() - p2.x()));
        //qDebug () << p3.y() << p2.y();
        scalar = ((p2 - p1).x() * (p3 - p2).y() - (p2 - p1).y() * (p3 - p2).x());
        if (((scalar > 0) && (invis)) || (!invis)) {

            for (int i = 0; i < approximation + 1; i ++) {
                points [i].setX ((updots [i] * work * rotationMatrix).x() + xshift);
                points [i].setY ((updots [i] * work * rotationMatrix).y() + yshift);
            }
            qp.drawPolygon (points, approximation);
        }*/

        /*p1 = QPointF ((downdots [2] * work).x() + xshift, (downdots [2] * work).y() + yshift);
        p2 = QPointF ((downdots [1] * work).x() + xshift, (downdots [1] * work).y() + yshift);
        p3 = QPointF ((downdots [0] * work).x() + xshift, (downdots [0] * work).y() + yshift);
        scalar = ((p2.x() - p1.x()) * (p3.y() - p2.y()) - (p2.x() - p1.x()) * (p3.y() - p2.y()));
        if (((scalar > 0) && (invis)) || (!invis)) {
            QPointF points [approximation];
            QVector3D v ((p2 - p1).x(), (p2 - p1).y(), ((updots [1] - updots [2]) * work).z());
            QVector3D w ((p3 - p2).x(), (p3 - p2).y(), ((updots [0] - updots [1]) * work).z());
            QVector4D vmult (v.y() * w.z() - v.z() * w.y(),
                             v.z() * w.x() - v.x() * w.z(),
                             v.x() * w.y() - v.y() * w.x(),
                             1);
            v = QVector3D (vmult.x(), vmult.y(), vmult.z());
            w = QVector3D ((curve [j + 1] - curve [j]).x(), (curve [j + 1] - curve [j]).y(), (curve [j + 1] - curve [j]).z());
            v.normalize();
            w.normalize();
            vmult = QVector4D (v.y() * w.z() - v.z() * w.y(),
                               v.z() * w.x() - v.x() * w.z(),
                               v.x() * w.y() - v.y() * w.x(),
                               1);
            float f = acos (v.x() * w.x() + v.y() * w.y() + v.z() * w.z());

            QMatrix4x4 rotationMatrix = QMatrix4x4 (
                            cos (f) + (1 - cos (f)) * vmult.x() * vmult.x()    , (1 - cos (f)) * vmult.x() * vmult.y() - vmult.z() * sin (f), (1 - cos (f)) * vmult.x() * vmult.z() + vmult.y() * sin (f), 0,
                            (1 - cos (f)) * vmult.y() * vmult.x() + vmult.z() * sin (f), cos (f) + (1 - cos (f)) * vmult.y() * vmult.y()    , (1 - cos (f)) * vmult.y() * vmult.z() - vmult.x() * sin (f), 0,
                            (1 - cos (f)) * vmult.z() * vmult.x() - vmult.y() * sin (f), (1 - cos (f)) * vmult.z() * vmult.y() + vmult.x() * sin (f), cos (f) + (1 - cos (f)) * vmult.z() * vmult.z()    , 0,
                            0                                  , 0                                  , 0                                  , 1);


            for (int i = 0; i < approximation + 1; i ++) {
                points [i].setX ((downdots [i] * work * rotationMatrix).x() + xshift);
                points [i].setY ((downdots [i] * work * rotationMatrix).y() + yshift);
            }

            scalar = ((p2.x() - p1.x()) * (p3.y() - p2.y()) - (p2.x() - p1.x()) * (p3.y() - p2.y()));
            p1 = QPointF ((downdots [2] * work * rotationMatrix).x() + xshift, (downdots [2] * work).y() + yshift);
            p2 = QPointF ((downdots [1] * work * rotationMatrix).x() + xshift, (downdots [1] * work).y() + yshift);
            p3 = QPointF ((downdots [0] * work * rotationMatrix).x() + xshift, (downdots [0] * work).y() + yshift);
            qp.drawPolygon (points, approximation);
        }*/

        QPointF points [approximation];

        p1 = QPointF ((updots [0] * work).x() + xshift, (updots [0] * work).y() + yshift);
        p2 = QPointF ((updots [1] * work).x() + xshift, (updots [1] * work).y() + yshift);
        p3 = QPointF ((updots [2] * work).x() + xshift, (updots [2] * work).y() + yshift);

        scalar = ((p2 - p1).x() * (p3 - p2).y() - (p2 - p1).y() * (p3 - p2).x());
        if (j >= approximation - 1) {
            if (((scalar > 0) && (invis)) || (!invis)) {
                for (int i = 0; i < approximation + 1; i ++) {
                    points [i].setX ((updots [i] * work).x() + xshift);
                    points [i].setY ((updots [i] * work).y() + yshift);
                }
                qp.drawPolygon (points, approximation);
            }
        }

        if (j == 0) {
            if (((scalar < 0) && (invis)) || (!invis)) {
                for (int i = 0; i < approximation + 1; i ++) {
                    points [i].setX ((downdots [i] * work).x() + xshift);
                    points [i].setY ((downdots [i] * work).y() + yshift);
                }
                qp.drawPolygon (points, approximation);
            }
        }

       qp.setPen(pen2);
       qp.drawLine((curve [j] * work).x () + xshift, (curve [j] * work).y () + yshift, (curve [j + 1] * work).x () + xshift, (curve [j + 1] * work).y () + yshift);
       qp.setPen(pen3);
       qp.drawLine((curve [j] * work).x () + xshift, (curve [j] * work).y () + yshift, (curve [j] * work).x () + xshift, (curve [j] * work).y () + yshift);
       qp.drawLine((curve [j + 1] * work).x () + xshift, (curve [j + 1] * work).y () + yshift, (curve [j + 1] * work).x () + xshift, (curve [j + 1] * work).y () + yshift);


    }



}

void Widget::resizeEvent (QResizeEvent * e){
    Q_UNUSED(e);
    xshift = (this->width() - 220) / 2;
    yshift = this->height() / 2;
    ui->groupBox_0->setGeometry(this->width() - 220, 0, ui->groupBox_0->width(), ui->groupBox_0->height());
}

void Widget::on_horizontalSlider_1_sliderMoved(int position)
{
    approximation = position;
    this->update();
}

void Widget::on_horizontalSlider_2_sliderMoved(int position)
{
    scalematrix = QMatrix4x4 (position, 0,        0,        0,
                              0,        position, 0,        0,
                              0,        0,        position, 0,
                              0,        0,        0,        1);
    this->update ();
}

void Widget::on_horizontalSlider_3_sliderMoved(int position)
{
    float angle = position / 180.0 * M_PI;
    xrotmatrix = QMatrix4x4 (1, 0,           0,             0,
                             0, cos (angle), - sin (angle), 0,
                             0, sin (angle), cos (angle),   0,
                             0, 0,           0,             1);
    this->update ();
}

void Widget::on_horizontalSlider_4_sliderMoved(int position)
{
    float angle = position / 180.0 * M_PI;
    yrotmatrix = QMatrix4x4 (cos (angle),   0, sin (angle), 0,
                             0,             1, 0,           0,
                             - sin (angle), 0, cos (angle), 0,
                             0,             0, 0,           1);
    this->update ();
}

void Widget::on_horizontalSlider_5_sliderMoved(int position)
{
    float angle = position  / 180.0 * M_PI;
    zrotmatrix = QMatrix4x4 (cos (angle), - sin (angle), 0, 0,
                             sin (angle), cos (angle),   0, 0,
                             0,           0,             1, 0,
                             0,           0,             0, 1);
    this->update ();
}

void Widget::on_checkBox_clicked(bool checked)
{
    invis = checked;
    this->update ();
}

void Widget::on_checkBox_2_clicked(bool checked)
{
    grid = checked;
    this->update ();
}



void Widget::on_doubleSpinBox_valueChanged(double arg1)
{
    P1.setX(arg1);
    this->update();
}

void Widget::on_doubleSpinBox_3_valueChanged(double arg1)
{
    P1.setY(arg1);
    this->update();
}

void Widget::on_doubleSpinBox_2_valueChanged(double arg1)
{
    P1.setZ(arg1);
    this->update();
}

void Widget::on_doubleSpinBox_4_valueChanged(double arg1)
{
    P2.setX(arg1);
    this->update();
}

void Widget::on_doubleSpinBox_5_valueChanged(double arg1)
{
    P2.setY(arg1);
    this->update();
}

void Widget::on_doubleSpinBox_6_valueChanged(double arg1)
{
    P2.setZ(arg1);
    this->update();
}

void Widget::on_doubleSpinBox_7_valueChanged(double arg1)
{
    P3.setX(arg1);
    this->update();
}

void Widget::on_doubleSpinBox_8_valueChanged(double arg1)
{
    P3.setY(arg1);
    this->update();
}

void Widget::on_doubleSpinBox_9_valueChanged(double arg1)
{
    P3.setZ(arg1);
    this->update();
}

#include "widget.h"
#include "ui_widget.h"

QVector <QVector4D> updots (51);
QVector <QVector4D> downdots (51);
int approximation = 3;
int xshift;
int yshift;
bool invis = 1;
bool grid = 0;
QVector4D upcenter (0, -1, 0, 1);
QVector4D downcenter (0, 1, 0, 1);
QVector4D lamp (5, -5, 5, 1);
QPointF p1;
QPointF p2;
QPointF p3;
QPointF p4;
QPointF p5;
QPointF p6;
QPointF p7;
int blankcolorr = 255;
int blankcolorg = 255;
int blankcolorb = 255;
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

float mult (QVector4D v1, QVector4D v2, QVector4D v3) {
    QVector4D r (v1.y() * v2.z() - v1.z() * v2.y(), v1.z() * v2.x() - v1.x() * v2.z(), v1.x() * v2.y() - v1.y() * v2.x(), 1);
    r.normalize();
    v3.normalize();
    return r.x() * v3.x() + r.y() * v3.y() + r.z() * v3.z();
}

void Widget::paintEvent (QPaintEvent * e) {
    Q_UNUSED(e);
    QPainter qp(this);
    QPen pen(Qt::black, 1, Qt::SolidLine);
    //QPen pen2(Qt::green, 1, Qt::SolidLine);
    //QColor c (50, 50, 50);
    //QColor c (125 / 2, 0, 0);

    //qp.setBrush(c);
    qp.setPen(pen);
    //qp.setBrush(Qt::green);

    work = blank * scalematrix * xrotmatrix * yrotmatrix * zrotmatrix;

    qp.drawRect((lamp * work).x() + xshift - 1, (lamp * work).y() + yshift - 1, 2, 2);

    for (int i = 0; i <= approximation; i ++) {
        updots [i].setX(cos (i * 2 * M_PI / approximation));
        updots [i].setY(-1);
        updots [i].setZ(sin (i * 2 * M_PI / approximation));
        updots [i].setW(1);
        downdots [i].setX(3 * cos (i * 2 * M_PI / approximation));
        downdots [i].setY(1);
        downdots [i].setZ(3 * sin (i * 2 * M_PI / approximation));
        downdots [i].setW(1);
    }
    for (int i = 0; i < approximation; i ++) {
        p1 = QPointF ((updots [i] * work).x() + xshift, (updots [i] * work).y() + yshift);
        p2 = QPointF ((updots [i + 1] * work).x() + xshift, (updots [i + 1] * work).y() + yshift);
        p3 = QPointF ((downdots [i + 1] * work).x() + xshift, (downdots [i + 1] * work).y() + yshift);
        p4 = QPointF ((downdots [i] * work).x() + xshift, (downdots [i] * work).y() + yshift);
        p5 = QPointF ((upcenter * work).x() + xshift, (upcenter * work).y() + yshift);
        p6 = QPointF ((downcenter * work).x() + xshift, (downcenter * work).y() + yshift);
        p7 = QPointF ((lamp * work).x() + xshift, (lamp * work).y() + yshift);
        QVector4D AB (p2.x() - p1.x(), p2.y() - p1.y(), (updots [i + 1] * work).z() - (updots [i] * work).z(), 1);
        QVector4D BC (p3.x() - p2.x(), p3.y() - p2.y(), (downdots [i + 1] * work).z() - (updots [i + 1] * work).z(), 1);
        QVector4D CD (p4.x() - p3.x(), p4.y() - p3.y(), (downdots [i] * work).z() - (downdots [i + 1] * work).z(), 1);
        QVector4D DA (p1.x() - p4.x(), p1.y() - p4.y(), (updots [i] * work).z() - (downdots [i] * work).z(), 1);
        QVector4D BE (p5.x() - p2.x(), p5.y() - p2.y(), (upcenter * work).z() - (updots [i + 1] * work).z(), 1);
        QVector4D EA (p1.x() - p5.x(), p1.y() - p5.y(), (updots [i] * work).z() - (upcenter * work).z(), 1);
        QVector4D DF (p6.x() - p4.x(), p6.y() - p4.y(), (downcenter * work).z() - (downdots [i] * work).z(), 1);
        QVector4D FC (p3.x() - p6.x(), p3.y() - p6.y(), (downdots [i + 1] * work).z() - (downcenter * work).z(), 1);
        QVector4D OA (p1.x() - p7.x(), p1.y() - p7.y(), (updots [i] * work).z() - (lamp * work).z(), 1);
        QVector4D OB (p2.x() - p7.x(), p2.y() - p7.y(), (updots [i + 1] * work).z() - (lamp * work).z(), 1);
        QVector4D OC (p3.x() - p7.x(), p3.y() - p7.y(), (downdots [i + 1] * work).z() - (lamp * work).z(), 1);
        QVector4D OD (p4.x() - p7.x(), p4.y() - p7.y(), (downdots [i] * work).z() - (lamp * work).z(), 1);
        QVector4D OE (p5.x() - p7.x(), p5.y() - p7.y(), (upcenter * work).z() - (lamp * work).z(), 1);
        QVector4D OF (p6.x() - p7.x(), p6.y() - p7.y(), (downcenter * work).z() - (lamp * work).z(), 1);
        float scalar = ((p2.x() - p1.x ()) * (p3.y() - p2.y()) - (p2.y() - p1.y()) * (p3.x() - p2.x()));

        if (((scalar > 0) && (invis)) || (!invis)) {
            QPointF points [4] = {p1, p2, p3, p4};

            float A =  mult (DA, AB, OA);
            float B =  mult (AB, BC, OB);
            float C =  mult (BC, CD, OC);
            float D =  mult (CD, DA, OD);
            float general = qMax ((A + B + C + D) / 4.0, 0.0);
            QColor c (blankcolorr * general + ambient, blankcolorg * general + ambient, blankcolorb * general + ambient);
            qp.setBrush(c);
            //qDebug () << c;
            qp.drawPolygon (points, 4);
        }

        scalar = ((p5.x() - p1.x ()) * (p2.y() - p5.y()) - (p5.y() - p1.y()) * (p2.x() - p5.x()));
        if (((scalar > 0) && (invis)) || (!invis)) {
            QPointF points [3] = {p1, p5, p2};
            float A = mult (EA, AB, OA);
            float B = mult (AB, BE, OB);
            float E = mult (BE, EA, OE);
            float general = qMax ( - (A + B + E) / 3.0, 0.0);
            QColor c (blankcolorr * general + ambient, blankcolorg * general + ambient, blankcolorb * general + ambient);
            qp.setBrush(c);
            qp.drawPolygon (points, 3);
        }

        scalar = ((p6.x() - p3.x ()) * (p4.y() - p6.y()) - (p6.y() - p3.y()) * (p4.x() - p6.x()));
        if (((scalar > 0) && (invis)) || (!invis)) {
            QPointF points [3] = {p4, p3, p6};
            float C = mult (FC, CD, OC);
            float D = mult (CD, DF, OD);
            float F = mult (DF, FC, OF);
            float general = qMax ( - (C + D + F) / 3.0, 0.0);
            QColor c (blankcolorr * general + ambient, blankcolorg * general + ambient, blankcolorb * general + ambient);
            qp.setBrush(c);
            qp.drawPolygon (points, 3);
        }
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

void Widget::on_horizontalSlider_6_sliderMoved(int position)
{
    blankcolorr = position;
    this->update();
}

void Widget::on_horizontalSlider_7_sliderMoved(int position)
{
    blankcolorg = position;
    this->update();
}

void Widget::on_horizontalSlider_8_sliderMoved(int position)
{
    blankcolorb = position;
    this->update();
}

void Widget::on_horizontalSlider_9_sliderMoved(int position)
{
    ambient = position;
    this->update();
}

void Widget::on_horizontalSlider_10_sliderMoved(int position)
{
    lamp.setX(position);
    this->update();
}

void Widget::on_horizontalSlider_11_sliderMoved(int position)
{
    lamp.setY(- position);
    this->update();
}

void Widget::on_horizontalSlider_12_sliderMoved(int position)
{
    lamp.setZ(position);
    this->update();
}

#include "widget.h"
#include "ui_widget.h"

float scale = 1;
int xshift;
int yshift;
int zshift;
bool iso = 0;
bool invis = 0;
QMatrix4x4 work;
QMatrix4x4 scalematrix;
QMatrix4x4 xrotmatrix;
QMatrix4x4 yrotmatrix;
QMatrix4x4 zrotmatrix;
QMatrix4x4 blank (1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1);
QMatrix4x4 isox (1, 0,                         0,                           0,
                 0, cos (35.0 / 180.0 * M_PI), - sin (35.0 / 180.0 * M_PI), 0,
                 0, sin (35.0 / 180.0 * M_PI), cos (35.0 / 180.0 * M_PI),   0,
                 0, 0,                         0,                           1);

QMatrix4x4 isoy (cos (M_PI / 4),   0, sin (M_PI / 4), 0,
                 0,                1, 0,              0,
                 - sin (M_PI / 4), 0, cos (M_PI / 4), 0,
                 0,                0, 0,              1);

QVector <QVector4D> dots = {QVector4D (cos (0), sin (0), -1, 1),                        //0
                            QVector4D (cos (M_PI / 5), sin (M_PI / 5), -1, 1),          //1
                            QVector4D (cos (2 * M_PI / 5), sin (2 * M_PI / 5), -1, 1),  //2
                            QVector4D (cos (3 * M_PI / 5), sin (3 * M_PI / 5), -1, 1),  //3
                            QVector4D (cos (4 * M_PI / 5), sin (4 * M_PI / 5), -1, 1),  //4
                            QVector4D (cos (M_PI), sin (M_PI), -1, 1),                  //5
                            QVector4D (cos (6 * M_PI / 5), sin (6 * M_PI / 5), -1, 1),  //6
                            QVector4D (cos (7 * M_PI / 5), sin (7 * M_PI / 5), -1, 1),  //7
                            QVector4D (cos (8 * M_PI / 5), sin (8 * M_PI / 5), -1, 1),  //8
                            QVector4D (cos (9 * M_PI / 5), sin (9 * M_PI / 5), -1, 1),  //9

                            QVector4D (cos (0), sin (0), 1, 1),                        //10
                            QVector4D (cos (M_PI / 5), sin (M_PI / 5), 1, 1),          //11
                            QVector4D (cos (2 * M_PI / 5), sin (2 * M_PI / 5), 1, 1),  //12
                            QVector4D (cos (3 * M_PI / 5), sin (3 * M_PI / 5), 1, 1),  //13
                            QVector4D (cos (4 * M_PI / 5), sin (4 * M_PI / 5), 1, 1),  //14
                            QVector4D (cos (M_PI), sin (M_PI), 1, 1),                  //15
                            QVector4D (cos (6 * M_PI / 5), sin (6 * M_PI / 5), 1, 1),  //16
                            QVector4D (cos (7 * M_PI / 5), sin (7 * M_PI / 5), 1, 1),  //17
                            QVector4D (cos (8 * M_PI / 5), sin (8 * M_PI / 5), 1, 1),  //18
                            QVector4D (cos (9 * M_PI / 5), sin (9 * M_PI / 5), 1, 1)   //19
                           };

QVector <QVector <int>> figure = {QVector <int> {0, 1, 11, 10, 0},
                                  QVector <int> {1, 2, 12, 11, 1},
                                  QVector <int> {2, 3, 13, 12, 2},
                                  QVector <int> {3, 4, 14, 13, 3},
                                  QVector <int> {4, 5, 15, 14, 4},
                                  QVector <int> {5, 6, 16, 15, 5},
                                  QVector <int> {6, 7, 17, 16, 6},
                                  QVector <int> {7, 8, 18, 17, 7},
                                  QVector <int> {8, 9, 19, 18, 8},
                                  QVector <int> {9, 0, 10, 19, 9},
                                  QVector <int> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0},
                                  QVector <int> {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 10},
                                 };

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
    QPen pen(Qt::black, 2, Qt::SolidLine);
    qp.setPen(pen);

    if (iso) {
        work = blank * scalematrix * xrotmatrix * yrotmatrix * zrotmatrix * isox * isoy;
    } else {
        work = blank * scalematrix * xrotmatrix * yrotmatrix * zrotmatrix;
    }


    for (int i = 0; i < figure.size (); i ++) {
        float scalar = (((dots [figure [i] [1]] * work).x () - (dots [figure [i] [0]] * work).x ()) * ((dots [figure [i] [2]] * work).y () - (dots [figure [i] [1]] * work).y ())) -
                       (((dots [figure [i] [1]] * work).y () - (dots [figure [i] [0]] * work).y ()) * ((dots [figure [i] [2]] * work).x () - (dots [figure [i] [1]] * work).x ()));

        if (i == 10) {
            scalar *= -1;
        }

        if (((scalar > 0) && (invis)) || (!invis)) {
            for (int j = 0; j < figure [i].size () - 1 ; j ++) {
                qp.drawLine((dots [figure [i] [j]] * work).x () + xshift,
                            (dots [figure [i] [j]] * work).y () + yshift,
                            (dots [figure [i] [j + 1]] * work).x () + xshift,
                            (dots [figure [i] [j + 1]] * work).y ()+ yshift
                           );
            }
        }

    }

}

void Widget::resizeEvent (QResizeEvent * e) {
    Q_UNUSED(e);
    xshift = (this->width() - 220) / 2;
    yshift = this->height() / 2;
    ui->groupBox_0->setGeometry(this->width() - 220, 0, ui->groupBox_0->width(), ui->groupBox_0->height());
}

void Widget::on_horizontalSlider_1_sliderMoved(int position)
{
    if (iso) {
        scale = position * sqrt(2.0/3.0);
    } else {
        scale = position;
    }
    scalematrix = QMatrix4x4 (scale, 0,     0,     0,
                              0,     scale, 0,     0,
                              0,     0,     scale, 0,
                              0,     0,     0,     1);
    this->update ();
}



void Widget::on_horizontalSlider_2_sliderMoved(int position)
{
    float angle = position / 180.0 * M_PI;
    xrotmatrix = QMatrix4x4 (1, 0,           0,             0,
                             0, cos (angle), - sin (angle), 0,
                             0, sin (angle), cos (angle),   0,
                             0, 0,           0,             1);
    this->update ();
}

void Widget::on_horizontalSlider_3_sliderMoved(int position)
{
    float angle = position / 180.0 * M_PI;
    yrotmatrix = QMatrix4x4 (cos (angle),   0, sin (angle), 0,
                             0,             1, 0,           0,
                             - sin (angle), 0, cos (angle), 0,
                             0,             0, 0,           1);
    this->update ();
}

void Widget::on_horizontalSlider_4_sliderMoved(int position)
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
    iso = checked;
    this->update();
}

void Widget::on_checkBox_2_clicked(bool checked)
{
    invis = checked;
    this->update();
}

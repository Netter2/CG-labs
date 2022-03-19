#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QVector>
#include <QtMath>
#include <QDebug>

float a = 0;
float A = 1;
float B = 1;
int approx = 1;
float t;
QVector <QPair <int, int>> dots (1000001);
int ycentershift = 0;
int xcentershift = 0;
int yshift = 0;
int xshift = 0;
float angle;
int ybuffer = 0;
int xbuffer = 0;
int yshiftbuffer = 0;
int xshiftbuffer = 0;

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

void drawLines (QPainter *qp) {
    QPen pen(Qt::black, 2, Qt::SolidLine);
    qp->setPen(pen);

    for (int i = 0; i < approx; i ++) {
        //qp->drawLine(dots [i].first + xcentershift + xshift, dots [i].second + ycentershift + yshift, dots [i + 1].first + xcentershift + xshift, dots [i+ 1].second + ycentershift + yshift);
        qp->drawLine(round ((dots [i].first + xshift) * cos (angle) - (dots [i].second - yshift) * sin (angle) + xcentershift),
                     round ((dots [i].first + xshift) * sin (angle) + (dots [i].second - yshift) * cos (angle) + ycentershift),
                     round((dots [i + 1].first + xshift) * cos (angle) - (dots [i + 1].second - yshift) * sin (angle) + xcentershift),
                     round((dots [i + 1].first + xshift) * sin (angle) + (dots [i + 1].second - yshift) * cos (angle) + ycentershift));
    }
    QPen pen2(Qt::blue, 2, Qt::SolidLine);
    qp->setPen(pen2);
    qp->drawLine(0, ycentershift, (xcentershift + 220) * 2, ycentershift);
    qp->drawLine(xcentershift, 0, xcentershift, ycentershift * 2);
    qp->drawText(xcentershift + 5, 15, "Y");
    qp->drawText(xcentershift * 2 - 10, ycentershift + 15, "X");
}

void Widget::paintEvent (QPaintEvent * e) {

    Q_UNUSED(e);
    QPainter qp(this);
    approx = ui->lineEdit_1->text().toInt();
    A = ui->lineEdit_2->text().toInt();
    B = ui->lineEdit_3->text().toInt();
    a = ui->lineEdit_4->text().toInt();
    angle = ui->lineEdit_5->text().toInt() / 180.0 * M_PI;
    //xshift = ui->lineEdit_6->text().toInt();
    //yshift = -ui->lineEdit_7->text().toInt();

    t = (B - A) / approx;
    for (int i = 0; i < approx + 1; i ++) {
        int x = round (a / (A + i * t) * cos (A + i * t/* + angle*/));
        int y = - round (a / (A + i * t) * sin (A + i * t/* + angle*/));
        dots [i] = QPair <int, int> (x, y);
    }

    drawLines(&qp);
}

void Widget::resizeEvent (QResizeEvent * e) {
    Q_UNUSED(e);
    xcentershift = (this->width() - 220) / 2;
    ycentershift = this->height() / 2;
    ui->groupBox_0->setGeometry(this->width() - 220, 0, ui->groupBox_0->width(), ui->groupBox_0->height());
}


void Widget::mousePressEvent(QMouseEvent *e) {
    xbuffer = e->x();
    ybuffer = e->y();
}

void Widget::mouseMoveEvent(QMouseEvent *e) {
    xshift = - xbuffer + e->x() + xshiftbuffer;
    yshift = ybuffer - e->y() + yshiftbuffer;
    this->update();
    qDebug () << xshift << yshift;
}

void Widget::mouseReleaseEvent (QMouseEvent *e) {
    Q_UNUSED(e);
    xshiftbuffer = xshift;
    yshiftbuffer = yshift;
}

void Widget::on_horizontalSlider_1_sliderMoved(int position)
{
    ui->lineEdit_1->setText(QString::number(position));
    this->update();
}

void Widget::on_horizontalSlider_2_sliderMoved(int position)
{
    ui->lineEdit_2->setText(QString::number(position));
    if (position > ui->lineEdit_3->text().toInt()) {
         ui->lineEdit_3->setText(QString::number(position));
         ui->horizontalSlider_3->setSliderPosition(position);
    }
    this->update();
}

void Widget::on_horizontalSlider_3_sliderMoved(int position)
{
    ui->lineEdit_3->setText(QString::number(position));
    if (position < ui->lineEdit_2->text().toInt()) {
         ui->lineEdit_2->setText(QString::number(position));
         ui->horizontalSlider_2->setSliderPosition(position);
    }
    this->update();
}

void Widget::on_horizontalSlider_4_sliderMoved(int position)
{
    ui->lineEdit_4->setText(QString::number(position));
    this->update();
}

void Widget::on_horizontalSlider_5_sliderMoved(int position)
{
    ui->lineEdit_5->setText(QString::number(position));
    this->update();
}

void Widget::on_horizontalSlider_6_sliderMoved(int position)
{
    ui->lineEdit_6->setText(QString::number(position));
    this->update();
}

void Widget::on_horizontalSlider_7_sliderMoved(int position)
{
    ui->lineEdit_7->setText(QString::number(position));
    this->update();
}

void Widget::on_lineEdit_1_editingFinished()
{
    ui->horizontalSlider_1->setSliderPosition(ui->lineEdit_1->text().toInt());
    this->update();
}

void Widget::on_lineEdit_2_editingFinished()
{
    ui->horizontalSlider_2->setSliderPosition(ui->lineEdit_2->text().toInt());
    if (ui->lineEdit_2->text().toInt() > ui->lineEdit_3->text().toInt()) {
        ui->lineEdit_3->setText(ui->lineEdit_2->text());
        ui->horizontalSlider_3->setSliderPosition(ui->lineEdit_2->text().toInt());
    }
    this->update();
}

void Widget::on_lineEdit_3_editingFinished()
{
    ui->horizontalSlider_3->setSliderPosition(ui->lineEdit_3->text().toInt());
    if (ui->lineEdit_2->text().toInt() > ui->lineEdit_3->text().toInt()) {
        ui->lineEdit_2->setText(ui->lineEdit_3->text());
        ui->horizontalSlider_2->setSliderPosition(ui->lineEdit_3->text().toInt());
    }
    this->update();
}

void Widget::on_lineEdit_4_editingFinished()
{
    ui->horizontalSlider_4->setSliderPosition(ui->lineEdit_4->text().toInt());
    this->update();
}

void Widget::on_lineEdit_5_editingFinished()
{
    ui->horizontalSlider_5->setSliderPosition(ui->lineEdit_5->text().toInt());
    this->update();
}

void Widget::on_lineEdit_6_editingFinished()
{
    ui->horizontalSlider_6->setSliderPosition(ui->lineEdit_6->text().toInt());
    this->update();
}

void Widget::on_lineEdit_7_editingFinished()
{
    ui->horizontalSlider_7->setSliderPosition(ui->lineEdit_7->text().toInt());
    this->update();
}



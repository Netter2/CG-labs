#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    emit angleXchanged (float (value));
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    emit angleYchanged (float (value));
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    emit angleZchanged (float (value));
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    emit lightPosXchanged ( float (value) / 100.0);
}

void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    emit lightPosYchanged ( float (value) / 100.0);
}

void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
    emit lightPosZchanged (float (value) / 100.0);
}

void MainWindow::on_horizontalSlider_7_valueChanged(int value)
{
    emit approxChanged (value);
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    emit animationOn (checked);
}

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QVector4D>
#include <QDebug>
#include <QPainter>
#include <QtMath>
#include <QMatrix4x4>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void paintEvent (QPaintEvent * e);

    void resizeEvent (QResizeEvent * e);

private slots:
    void on_horizontalSlider_1_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_3_sliderMoved(int position);

    void on_horizontalSlider_4_sliderMoved(int position);

    void on_horizontalSlider_5_sliderMoved(int position);

    void on_checkBox_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

    void on_horizontalSlider_6_sliderMoved(int position);

    void on_horizontalSlider_9_sliderMoved(int position);

    void on_horizontalSlider_8_sliderMoved(int position);

    void on_horizontalSlider_7_sliderMoved(int position);

    void on_horizontalSlider_10_sliderMoved(int position);

    void on_horizontalSlider_11_sliderMoved(int position);

    void on_horizontalSlider_12_sliderMoved(int position);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H

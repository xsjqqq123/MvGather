#include "setcolorwidget.h"

SetColorWidget::SetColorWidget(QWidget *parent) :
    FBaseDialog(parent)
{
    //setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    this->normalSize = QSize(400,300);
    setColorCenterWin = new SetColorCenterWin(this);
    connect(setColorCenterWin,SIGNAL(closeS()),this,SLOT(close()));
    getTitleBar()->getTitleLabel()->setText("选择颜色");
    this->layout()->addWidget(setColorCenterWin);
}

#include "myinputdialog.h"

MyInputDialog::MyInputDialog(QWidget *parent) :
    QWidget(parent)
{
    txtOrImgLbl = new QLabel;
    inputL = new QLineEdit;
    okBtn = new QPushButton;
    okBtn->setText("确定");
    spaceItem = new QSpacerItem(60,25);
    mainLayout = new QGridLayout(this);
    mainLayout->setSpacing(3);
    mainLayout->setMargin(3);

    mainLayout->addWidget(txtOrImgLbl,0,0,1,1);
    mainLayout->addWidget(inputL,0,1,1,3);
    mainLayout->addItem(spaceItem,1,0,1,3);
    mainLayout->addWidget(okBtn,1,3,1,1);

    connect(okBtn,SIGNAL(clicked()),this,SLOT(emitAccept()));
    connect(inputL,SIGNAL(returnPressed()),this,SLOT(emitAccept()));
}

void MyInputDialog::setPixMap(QPixmap pix)
{
    txtOrImgLbl->setPixmap(pix);
}

void MyInputDialog::setLblText(QString txt)
{
    txtOrImgLbl->setText(txt);
}

void MyInputDialog::setLineEditText(QString txt)
{
    inputL->setText(txt);
}

QString MyInputDialog::getText()
{
    return inputL->text();
}

void MyInputDialog::emitAccept()
{
    emit acceptSignal(inputL->text());
}

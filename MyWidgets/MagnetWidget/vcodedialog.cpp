#include "vcodedialog.h"
#include "ui_vcodedialog.h"

VcodeDialog::VcodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VcodeDialog)
{
    ui->setupUi(this);
}

VcodeDialog::~VcodeDialog()
{
    delete ui;
}

void VcodeDialog::setPixmap(QPixmap pix)
{
    ui->label->setPixmap(pix);
}

QString VcodeDialog::getText()
{
    return  ui->lineEdit->text();
}

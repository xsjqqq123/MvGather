#include "prefersourcewidget.h"
#include "ui_prefersourcewidget.h"
#include <QSettings>
preferSourceWidget::preferSourceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::preferSourceWidget)
{
    ui->setupUi(this);
    QSettings settings("MvGather", "xusongjie");
    QString preferSource = settings.value("app/preferSource", "letv#youku#tudou#imgotv#fun#funsion#hunantv#qq#qiyi#sohu#pptv#pps#1905").toString();
    ui->listWidget->addItems(preferSource.split("#"));

}

preferSourceWidget::~preferSourceWidget()
{
    delete ui;
}

void preferSourceWidget::on_saveBtn_clicked()
{
    QStringList list_temp;
    int totalRows = ui->listWidget->count();
    for(int i=0;i<totalRows;i++)
    {
        list_temp.append(ui->listWidget->item(i)->text());
    }
    QSettings settings("MvGather", "xusongjie");
    settings.setValue("app/preferSource", list_temp.join("#"));
    emit closeSig();
}

void preferSourceWidget::on_upBtn_clicked()
{
    int currentItemIndex = ui->listWidget->currentRow();
    if(currentItemIndex <= 0)
    {
        return;
    }
    QListWidgetItem *currentItem= ui->listWidget->takeItem(ui->listWidget->currentRow());
    ui->listWidget->insertItem(currentItemIndex-1,currentItem);
    ui->listWidget->setCurrentItem(currentItem);
}

void preferSourceWidget::on_downBtn_clicked()
{
    int currentItemIndex = ui->listWidget->currentRow();
    if(currentItemIndex >= ui->listWidget->count()-1||currentItemIndex<0 )
    {
        return;
    }
    QListWidgetItem *currentItem= ui->listWidget->takeItem(ui->listWidget->currentRow());
    ui->listWidget->insertItem(currentItemIndex+1,currentItem);
    ui->listWidget->setCurrentItem(currentItem);
}

void preferSourceWidget::on_resetBtn_clicked()
{
    QString str("letv#youku#tudou#imgotv#fun#funsion#hunantv#qq#qiyi#sohu#pptv#pps#1905");
    ui->listWidget->clear();
    ui->listWidget->addItems(str.split("#"));
}

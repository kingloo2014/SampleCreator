#include "qsizewidget.h"
#include "ui_qsizewidget.h"

QSizeWidget::QSizeWidget(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::QSizeWidget)
{
    ui->setupUi(this);
    connect(ui->commitButton, &QPushButton::clicked, this, &QSizeWidget::onClickCommit);
}

QSizeWidget::~QSizeWidget()
{
    delete ui;
}

void QSizeWidget::onClickCancel()
{
    ui->widthEdit->clear();
    ui->heightEdit->clear();
}

void QSizeWidget::onClickCommit()
{
    int width = ui->widthEdit->text().toInt();
    int height = ui->heightEdit->text().toInt();
    emit CommitResize(width, height);
    this->hide();
}

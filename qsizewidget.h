#ifndef QSIZEWIDGET_H
#define QSIZEWIDGET_H

#include <QDialog>

namespace Ui {
class QSizeWidget;
}

class QSizeWidget : public QDialog
{
    Q_OBJECT

public:
    explicit QSizeWidget(QDialog *parent = 0);
    ~QSizeWidget();

protected slots:
    void onClickCancel();
    void onClickCommit();

signals:
    void CommitResize(int width, int height);
private:
    Ui::QSizeWidget *ui;
};

#endif // QSIZEWIDGET_H

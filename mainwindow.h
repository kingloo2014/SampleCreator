#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPoint>
#include <QMenu>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

signals:
    void mouse_move(QPoint pos);

protected:
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void showImage();
    void nextImage();

private:
    QStringList m_listSample;
    QString m_sampleDirPath;
    int m_curSelImg;

    QMenu *m_optMenu;
    QAction *m_SelAction;
};

#endif // MAINWINDOW_H

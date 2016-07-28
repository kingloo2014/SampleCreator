#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPoint>
#include <QMenu>
#include <QAction>
#include "qflipthread.h"


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

private:


public slots:
    void showImage();
    void nextImage();
    void flipResponse();
    void getSetInfo();
    void flipNextImg(int idx);
    void shuffeSamples();
    void rotateSamples();
    void rotateNext(const QImage& img);
public:
    QStringList m_listSample;
    QString m_sampleDirPath;

private:
    int m_curSelImg;

    QMenu *m_fileMenu;
    QAction *m_SelAction;

    QMenu *m_optMenu;
    QAction *m_CropAction;
    QAction *m_FlipAction;
    QAction *m_shuffleAction;
    QAction *m_rotateAction;

    QFlipThread* m_flipThread;
    QFlipThread *m_shuffleThread;
    QFlipThread *m_rotateThread;
};

#endif // MAINWINDOW_H

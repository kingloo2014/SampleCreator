#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QFileDialog>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_curSelImg(0),
    m_sampleDirPath("")
{
    ui->setupUi(this);
    //connect(ui->importSource, SIGNAL(clicked()), this, SLOT(showImage()));
    connect(this, SIGNAL(mouse_move(QPoint)), ui->imageView, SLOT(getMousePos(QPoint)));
    connect(ui->imageView, SIGNAL(right_mouseClicked()), this, SLOT(nextImage()));
    setMouseTracking(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    ui->imageView->setPalette(pal);
    ui->imageView->setFocus();
    ui->progressBar->setEnabled(true);
    ui->progressBar->setValue(0);

    menuBar()->setFont(QFont(tr("微软雅黑"), 10, 75, 1));
    m_optMenu = menuBar()->addMenu(tr("数据选择"));
    m_SelAction = new QAction(tr("选择样本集"),this);
    m_SelAction->setShortcuts(QKeySequence::New);
    m_SelAction->setStatusTip(tr("选择包含图片的文件夹"));
    connect(m_SelAction, &QAction::triggered, this, &MainWindow::showImage);
    m_optMenu->addAction(m_SelAction);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showImage()
{
    m_listSample.clear();
    m_curSelImg = 0;
    ui->progressBar->reset();

    QDir posDir;
    if(!posDir.exists("pos")){
        posDir.mkdir("pos");
    }

    QDir negDir;
    if(!negDir.exists("neg")){
        negDir.mkdir("neg");
    }

    QFileDialog dialog(this, "请选择包含图片的文件夹");
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    if(dialog.exec())
    {
        QStringList strDirPath = dialog.selectedFiles();
        m_sampleDirPath = strDirPath[0];
        qDebug() << *strDirPath.begin();

        QDir dir(strDirPath[0]);
        dir.setFilter(QDir::Files);
        QStringList filters;
        filters << "*.jpg"<<"*.bmp"<<"*.png";
        dir.setNameFilters(filters);
        m_listSample = dir.entryList();

        if(!m_listSample.empty()){
            update();
            QString strImgPath = strDirPath[0] + "/" + m_listSample[m_curSelImg];
            QImage img(strImgPath);
            //QImage sImg = img.scaled(ui->imageView->size());
            ui->imageView->setImgSrc(img,m_listSample[m_curSelImg]);
            ui->progressBar->setRange(0, m_listSample.size());
            ui->progressBar->setValue(0);
        }
        else{
            ui->statusBar->showMessage("you had selected a empty directory without an image");
            return ;
        }
    }

    ui->statusBar->showMessage("ready");
}

void MainWindow::nextImage()
{
    if(m_listSample.empty())
        return ;
    m_curSelImg++;
    ui->progressBar->setValue(m_curSelImg);
    if(m_curSelImg < m_listSample.size()){
        update();
        QString strImgPath = m_sampleDirPath + "/" + m_listSample[m_curSelImg];
        QImage img(strImgPath);
        //QImage sImg = img.scaled(ui->imageView->size());
        ui->imageView->setImgSrc(img,m_listSample[m_curSelImg]);
        ui->statusBar->showMessage(m_listSample[m_curSelImg]);
    }
    else{
        ui->progressBar->setValue(0);
        ui->statusBar->showMessage("finished");
    }
}



void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    emit mouse_move(event->pos());
}

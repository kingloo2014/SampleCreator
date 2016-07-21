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
    m_fileMenu = menuBar()->addMenu(tr("File"));
    m_optMenu = menuBar()->addMenu(tr("Option"));

    m_SelAction = new QAction(tr("&Select Image Sets"),this);
    m_SelAction->setIcon(QIcon(":/res/addFile.png"));
    m_SelAction->setStatusTip(tr("select a directory including images"));
    connect(m_SelAction, &QAction::triggered, this, &MainWindow::getSetInfo);
    m_fileMenu->addAction(m_SelAction);

    m_FlipAction = new QAction(tr("&Flip Horizontally"),this);
    m_FlipAction->setIcon(QIcon(":/res/flip.png"));
    m_FlipAction->setStatusTip(tr("Flip samples at the horizontal orientation"));
    connect(m_FlipAction, &QAction::triggered, this, &MainWindow::flipResponse);
    m_optMenu->addAction(m_FlipAction);

    m_CropAction = new QAction(tr("&Crop ROI"),this);
    m_CropAction->setIcon(QIcon(":/res/crop.png"));
    m_CropAction->setStatusTip(tr("Flip samples at the horizontal orientation"));
    connect(m_FlipAction, &QAction::triggered, this, &MainWindow::showImage);
    m_optMenu->addAction(m_CropAction);

    m_flipThread = new QFlipThread();
    m_flipThread->setFunc(&m_sampleDirPath, &m_listSample);
    connect(m_flipThread, SIGNAL(nextImage(int)), this, SLOT(flipNextImg(int)));

}

MainWindow::~MainWindow()
{
    delete m_SelAction;
    delete m_FlipAction;
    delete m_flipThread;
    delete ui;
}

void MainWindow::flipResponse(){
    QDir flipDir;
    if(!flipDir.exists("flip")){
        flipDir.mkdir("flip");
    }
    m_flipThread->start();

}

void MainWindow::flipNextImg(int idx){
    ui->statusBar->showMessage(m_listSample[idx]);
    ui->progressBar->setValue(idx);
}



void MainWindow::getSetInfo(){
    m_listSample.clear();
    m_curSelImg = 0;
    ui->progressBar->reset();

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
            ui->progressBar->setRange(0, m_listSample.size());
            ui->progressBar->setValue(0);

            QString strMsg("");
            strMsg.sprintf("the current directory has %d images", m_listSample.size());
            ui->statusBar->showMessage(strMsg);
        }
        else{
            ui->statusBar->showMessage("you had selected a empty directory without an image");
            return ;
        }
    }
}


void MainWindow::showImage()
{
    QDir posDir;
    if(!posDir.exists("pos")){
        posDir.mkdir("pos");
    }

    QDir negDir;
    if(!negDir.exists("neg")){
        negDir.mkdir("neg");
    }

    if(!m_listSample.empty()){
        update();
        QString strImgPath = m_sampleDirPath + "/" + m_listSample[m_curSelImg];
        QImage img(strImgPath);
        ui->imageView->setImgSrc(img,m_listSample[m_curSelImg]);
    }
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



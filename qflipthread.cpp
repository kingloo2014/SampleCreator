#include "qflipthread.h"

QFlipThread::QFlipThread(QObject *parent):QThread(parent)
{
}

void QFlipThread::run(){
    int nsize = m_pImgList->size();
    for(int i = 0; i < nsize; i++){
        QString strName("");
        strName = *m_pImgDir + "/" + (*m_pImgList)[i];
        QImage img(strName);
        QImage rotateImg = flipImage(img);
        QString strFlipName("");
        strFlipName.sprintf("flip/fh_%s",(*m_pImgList)[i].toUtf8().data());
        rotateImg.save(strFlipName);
        Q_EMIT nextImage(i+1);
    }

    this->quit();
}

QImage QFlipThread::flipImage(const QImage& img_src, bool horizon)
{
    QImage img_res(img_src.size(), img_src.format());
    int width = img_src.width();
    int height = img_src.height();
    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            img_res.setPixel(w, h, img_src.pixel(width - w - 1,h));
        }
    }

    return img_res;
}

void QFlipThread::setFunc(QString* pImgDir, QStringList* pImgList ){
        m_pImgDir = pImgDir;
        m_pImgList = pImgList;
    }

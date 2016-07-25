#include "qflipthread.h"

QFlipThread::QFlipThread(QObject *parent):QThread(parent),
    m_type(NULL_TYPE)
{
}

QFlipThread::QFlipThread(type_thread type){
    m_type = type;
}

void QFlipThread::run(){
    int nsize = m_pImgList->size();
    switch (m_type) {
    case FLIP:
        for(int i = 0; i < nsize; i++){
            QString strName("");
            strName = *m_pImgDir + "/" + (*m_pImgList)[i];
            QImage img(strName);
            QImage rotateImg = flipImage(img);
            QString strFlipName("");
            strFlipName.sprintf("flip/fh_%s",(*m_pImgList)[i].toUtf8().data());
            rotateImg.save(strFlipName);
            Q_EMIT nextImage(i);
        }
        break;
    case SHUFFLE:
        shuffleImages(*m_pImgList);
        break;
    default:
        break;
    }


}

QImage QFlipThread::flipImage(const QImage& img_src, bool horizon)
{
    QImage img_res(img_src.size(), img_src.format());
    int width = img_src.width();
    int height = img_src.height();
    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            img_res.setPixelColor(w, h, img_src.pixelColor(width - w - 1,h));
        }
    }

    return img_res;
}

void QFlipThread::shuffleImages(const QStringList& list){
    std::vector<int> vecIdx(list.size(), 0);
    for(int i = 0; i < vecIdx.size(); i++){
        vecIdx[i] = i;
    }

    std::random_shuffle(vecIdx.begin(), vecIdx.end());
    for(int i = 0; i < vecIdx.size(); i++){
        QString strSrcName("");
        strSrcName = *m_pImgDir + "/" + list[i];
        QImage img(strSrcName);
        QString strName("");
        strName = *m_pImgDir + "/" + list[vecIdx[i]];
        img.save(strName);
        Q_EMIT nextImage(i);
    }
}

void QFlipThread::setFunc(QString* pImgDir, QStringList* pImgList){
        m_pImgDir = pImgDir;
        m_pImgList = pImgList;
    }

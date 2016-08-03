#ifndef QFLIPTHREAD_H
#define QFLIPTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>

enum type_thread{
    FLIP = 0,
    SHUFFLE,
    ROTATE,
    RESIZE,
    NULL_TYPE
};


class QFlipThread : public QThread
{
    Q_OBJECT
public:
    explicit QFlipThread(QObject *parent = 0);
    explicit QFlipThread(type_thread type);
    void setFunc(QString* pImgDir, QStringList* pImgList);

protected:
    virtual void run();

private:

     void shuffleImages(const QStringList& list);
     void rotateSets(const QStringList& list);
     void resizeSets(const QStringList& list, int w, int h);

private:
     QImage flipImage(const QImage& img_src, bool horizon = true);
     QImage rotateImage(const QImage& img_src, qreal angle);
     QImage resizeImage(const QImage& img_src, int w, int h);
private:
    QString* m_pImgDir;
    QStringList* m_pImgList;
    type_thread m_type;
public:
    int m_rsWidth;
    int m_rsHeight;
signals:
    void nextImage(int idx);
    void rotateSingle(const QImage& img);
public slots:    
    void getResizePara(int w, int h);
};

#endif // QFLIPTHREAD_H

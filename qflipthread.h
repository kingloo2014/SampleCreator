#ifndef QFLIPTHREAD_H
#define QFLIPTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>

enum type_thread{
    FLIP = 0,
    SHUFFLE,
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
     QImage flipImage(const QImage& img_src, bool horizon = true);
     void shuffleImages(const QStringList& list);
private:
    QString* m_pImgDir;
    QStringList* m_pImgList;
    type_thread m_type;
signals:
    void nextImage(int idx);
public slots:
};

#endif // QFLIPTHREAD_H

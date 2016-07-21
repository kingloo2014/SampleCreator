#ifndef QFLIPTHREAD_H
#define QFLIPTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>


class QFlipThread : public QThread
{
    Q_OBJECT
public:
    explicit QFlipThread(QObject *parent = 0);
    void setFunc(QString* pImgDir, QStringList* pImgList);

protected:
    virtual void run();

private:
     QImage flipImage(const QImage& img_src, bool horizon = true);

private:
    QString* m_pImgDir;
    QStringList* m_pImgList;
signals:
    void nextImage(int idx);
public slots:
};

#endif // QFLIPTHREAD_H

#ifndef QCLICKEDLABEL_H
#define QCLICKEDLABEL_H

#include <QLabel>
#include <QPainter>
#include <QRect>
#include <vector>
#include <QImage>

#define MV_STEP 5

class QClickLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QClickLabel(QWidget *parent = 0);
    void setImgSrc(const QImage& img, const QString& strImgName);
    QImage getImgROI(const QRect& rect_roi);
    ~QClickLabel();
signals:
    void clicked();
    void right_mouseClicked();
protected:

    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
    virtual void resizeEvent(QResizeEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void paintEvent(QPaintEvent *);
private:
    bool posInRect(const QPoint &pos, const QRect &rect);
    void setImgRegion(QImage& img, const QRect& rect);
public slots:
    void getMousePos(QPoint pos);
private:
    QPoint m_tlPos;
    QPoint m_brPos;
    QPoint m_mvPos;

    QRect m_curRect;
    QImage m_img;
    QImage m_scaleImg;

    float m_fxratio;
    float m_fyratio;

    QString m_strCurImgName;

    std::vector<QRect> m_ROIs;
};

#endif // QCLICKEDLABEL_H

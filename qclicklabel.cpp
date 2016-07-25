#include "qclicklabel.h"
#include <QMouseEvent>
#include <qdebug.h>
#include <QRect>
#include <QDateTime>

QClickLabel::QClickLabel(QWidget *parent) : QLabel(parent),
    m_ROIs(0),
    m_fxratio(1.f),
    m_fyratio(1.f)
{

}

QClickLabel::~QClickLabel()
{
}

void QClickLabel::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if(event->button() == Qt::MouseButton::LeftButton){
        if(QRect(m_tlPos,pos).width() >= 10 && QRect(m_tlPos,pos).height() >= 10){
            if(pos.x() > m_tlPos.x() && pos.y() > m_tlPos.y())
                m_ROIs.emplace_back(QRect(m_tlPos,pos));
            else
                m_ROIs.emplace_back(QRect(pos, m_tlPos));

            repaint();
        }
    }


    return ;
}

void QClickLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint curPos = event->pos();
    if(event->button() == Qt::MouseButton::LeftButton){
        for(auto iter = m_ROIs.begin(); iter != m_ROIs.end(); ){
            if(posInRect(curPos, *iter)){
                iter = m_ROIs.erase(iter);
            }
            else
                iter++;
        }
    }
    repaint();
}


QImage QClickLabel::getImgROI(const QRect& rect_roi)
{
    return m_img.copy(rect_roi.x(), rect_roi.y(), rect_roi.width(), rect_roi.height());
}

void QClickLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::MouseButton::LeftButton)
        m_tlPos = ev->pos();
    else if(ev->button() == Qt::MouseButton::RightButton)
    {
        QDateTime time = QDateTime::currentDateTime();
        QString strTime = time.toString("MMddhhmmsszzz");
        QImage temp = m_img;
        srand(1);

        QFile posFile("pos.txt");
        bool bOpen = posFile.open(QIODevice::ReadWrite |QIODevice::Text | QIODevice::Append);
        bOpen &= posFile.atEnd();
        QString strDescName("");
        strDescName.sprintf("%s %d",m_strCurImgName.toUtf8().data(), m_ROIs.size());

        //保存正样本
        foreach(QRect region, m_ROIs){
            QRect rectSrc(region.x()*m_fxratio, region.y()*m_fyratio, region.width()*m_fxratio, region.height()*m_fyratio);
            QImage obj = getImgROI(rectSrc);
            setImgRegion(temp, rectSrc);

            int rand_num = rand() % 1000;
            QString strImgName("");
            strImgName.sprintf("pos/%s%d.bmp", strTime.toUtf8().data(), rand_num);
            obj.save(strImgName);

            QString strTemp("");
            strTemp.sprintf(" %d %d %d %d", rectSrc.x(), rectSrc.y(), rectSrc.width(), rectSrc.height());
            strDescName.append(strTemp);
        }

        strDescName += "\r\n";
        if(bOpen){
            posFile.write(strDescName.toUtf8().data(), strDescName.length());
            posFile.close();
        }


        //保存负样本
        QString strNegName("");
        strNegName.sprintf("neg/%s.jpg", strTime.toUtf8().data());
        temp.save(strNegName);
        m_ROIs.clear();
        Q_EMIT right_mouseClicked();
    }

    return ;
}

void QClickLabel::setImgRegion(QImage& img, const QRect& rect)
{
    for(int i = 0; i < rect.width(); i++)
        for(int j = 0; j < rect.height();j++){
            QPoint tlPos = rect.topLeft();
            QColor color(0,0,0);
            img.setPixel(tlPos.x() + i, tlPos.y() + j,  color.rgb());
        }
}

void QClickLabel::mouseMoveEvent(QMouseEvent *ev)
{
    //Qt::MouseButton btn_s = ev->button();
    if(ev->button() == Qt::NoButton)
    {
         m_mvPos = ev->pos();
         m_curRect = QRect(m_tlPos, m_mvPos);
         repaint();
         setFocus();
    }
}

void QClickLabel::resizeEvent(QResizeEvent *event)
{
    QSize new_size = event->size();
    QSize old_size = event->oldSize();
    float fxratio = new_size.width()*1.f/old_size.width();
    float fyratio = new_size.height()*1.f/old_size.height();

    for(std::vector<QRect>::iterator iter = m_ROIs.begin(); iter != m_ROIs.end(); iter++) {
        iter->setRect(iter->left() * fxratio, iter->top() * fyratio, iter->width() * fxratio, iter->height() * fyratio);
    }
}

void QClickLabel::setImgSrc(const QImage& img, const QString& strImgName)
{
    m_img = img;
    m_strCurImgName = strImgName;
}

void QClickLabel::paintEvent(QPaintEvent *)
{
    QPainter *m_painter = new QPainter(this);
    m_painter->setPen(QPen(Qt::red,2,Qt::DashLine));//设置画笔形式
    //m_painter->setBrush(QBrush(Qt::red,Qt::LinearGradientPattern));//设置画刷形式

    if(!m_img.isNull()){
        m_scaleImg = m_img.scaled(this->size());
        m_fxratio = m_img.width()*1.f / this->size().width();
        m_fyratio = m_img.height()*1.f / this->size().height();
        m_painter->drawImage(0,0, m_scaleImg);
    }
    foreach (auto &elem, m_ROIs) {
        m_painter->drawRect(elem);
    }

    if(m_curRect.width() * m_curRect.height() > 0){
        m_painter->drawRect(m_curRect);
        m_curRect = QRect(0,0,0,0);
    }

    m_painter->end();
    delete m_painter;
}


void QClickLabel::keyPressEvent(QKeyEvent *event)
{
    QPoint curPos = cursor().pos();
    curPos = mapFromGlobal(curPos);

    bool enable = false;
    int iSel = -1;
    int idx = 0;
    foreach(auto &elem, m_ROIs){
        if(posInRect(curPos, elem)){
            enable = true;
            iSel = idx;
        }
        idx++;
    }

    int keyAscii = event->key();
    if(enable){
        switch (keyAscii) {
        case 'A':
            m_ROIs[iSel].adjust(-MV_STEP, 0,0,0);
            break;
        case 'S':
            m_ROIs[iSel].adjust(MV_STEP,0,0,0);
            break;
        case 'D':
            m_ROIs[iSel].adjust(0,0,-MV_STEP, 0);
            break;
        case 'F':
            m_ROIs[iSel].adjust(0,0, MV_STEP, 0);
            break;
        case 'W':
            m_ROIs[iSel].adjust(0,- MV_STEP, 0,0);
            break;
        case 'E':
            m_ROIs[iSel].adjust(0, MV_STEP,0,0);
            break;
        case 'X':
            m_ROIs[iSel].adjust(0,0,0, -MV_STEP);
            break;
        case 'C':
            m_ROIs[iSel].adjust(0,0,0,MV_STEP);
            break;
        default:
            break;
        }
    }
    repaint();
}

bool QClickLabel::posInRect(const QPoint &pos, const QRect &rect)
{
    QPoint center = rect.center();
    if(abs(center.x() - pos.x()) < rect.width()/2 && abs(center.y() - pos.y()) < rect.height()/2)
        return true;
    else
        return false;
}

void QClickLabel::getMousePos(QPoint pos)
{
}



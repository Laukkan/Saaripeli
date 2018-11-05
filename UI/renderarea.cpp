#include "renderarea.hh"

#include <QPainter>
#include <QRect>
#include <cmath>

RenderArea::RenderArea(QWidget *parent) :
    _parent(parent), _hexpen(QPen(Qt::black, 2))
{

}

void RenderArea::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);

    int width = static_cast<int>(_parent->width() * 0.85);
    int height = static_cast<int>(_parent->height() * 0.85);
    int y = _parent->height() / 2 - height/2;
    int x  = _parent->width() / 2 - width/2;
    painter.drawRect(x, y, width, height);

    painter.setPen(_hexpen);
    painter.setBrush(Qt::yellow);
    drawHex(&painter);
}

void RenderArea::drawHex(QPainter* painter)
{
    int x = this->width() / 2;
    int y = this->height() / 2;

    int hexW = static_cast<int>(20 * sqrt(3));
    int hexH = 20 * 2;

    static const QPoint points[6] = {
        QPoint(x, y),
        QPoint(x + hexW / 2, y + hexH / 4),
        QPoint(x + hexW / 2, y + hexH * 3/4),
        QPoint(x, y + hexH),
        QPoint(x - hexW / 2, y + hexH * 3/4),
        QPoint(x - hexW / 2, y + hexH / 4),
    };
    painter->drawPolygon(points, 6);
}

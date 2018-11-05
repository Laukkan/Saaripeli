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

    QPointF points[6];

    Center center(x, y);
    int size = 30;
    int side = 0;

    while (side <= 5) {
        points[side] = pointyHexCorner(center, size, side);
        side++;
    }
    painter->drawPolygon(points, 6);
}

QPointF RenderArea::pointyHexCorner(Center center, int size, int side)
{
    double angle_deg = 60 * side - 30;
    double angle_rad = M_PI / 180 * angle_deg;

    return QPointF(center.x + size * cos(angle_rad),
                  center.y + size * sin(angle_rad));
}

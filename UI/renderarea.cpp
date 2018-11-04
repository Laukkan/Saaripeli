#include "renderarea.hh"

#include <QPainter>
#include <QRect>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{

}

void RenderArea::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::darkGreen);
    painter.drawRect(200, 200, 800, 600);

    drawHex(&painter);
}

void RenderArea::drawHex(QPainter* painter)
{
    static const QPoint points[6] = {
        QPoint(200, 210),
        QPoint(220, 210),
        QPoint(225, 220),
        QPoint(220, 230),
        QPoint(210, 230),
        QPoint(205, 220),
    };

    painter->setBrush(Qt::red);
    painter->drawPolygon(points, 6);
}

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

    int rows = 9;
    int middleRow = rows-(rows-1)/2;
    int middleRowWidth = 9;

    for(int row=0; row < rows; row++){
        int numberOfHexes = middleRowWidth-abs((middleRow-1-row));
        x = (this->width() / 2) -((numberOfHexes+0.5)/2*52);
        y = this->height() / 2 - (rows/2*46) + (row*46);

        Center center(x, y);

        drawRowOfHexes(&painter,center,numberOfHexes);
    }

}

void RenderArea::drawRowOfHexes(QPainter *painter, Center rightMostCenter, int numberOfHexes)
{
    for(int hex=0; hex < numberOfHexes; hex++){
        drawHex(painter,rightMostCenter);
        rightMostCenter.x += 52;
    }
}

void RenderArea::drawHex(QPainter* painter, Center center)
{

    //int x = this->width() / 2;
    //int y = this->height() / 2;

    QPointF points[6];

    //Center center(x, y);
    int size = 30;
    int side = 0;

    while (side <= 5) {
        points[side] = pointyHexCorner(center, size, side);
        side++;
    }
    painter->drawPolygon(points, 6);


    /*int z = x+26;
    int l = y+46;
    Center center2(z,l);
    int side2 = 0;
    while (side2 <= 5) {
        points[side2] = pointyHexCorner(center2, size, side2);
        side2++;
    }
    painter->drawPolygon(points, 6);*/
}


QPointF RenderArea::pointyHexCorner(Center center, int size, int side)
{
    double angle_deg = 60 * side - 30;
    double angle_rad = M_PI / 180 * angle_deg;

    return QPointF(center.x + size * cos(angle_rad),
                  center.y + size * sin(angle_rad));
}

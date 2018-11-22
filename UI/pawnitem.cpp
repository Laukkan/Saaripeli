#include "pawnitem.hh"
#include "helpers.hh"

#include <QDrag>
#include <QCursor>
#include <QMimeData>
#include <QPainter>

namespace Student {

PawnItem::PawnItem(std::shared_ptr<Common::Pawn> pawn):
    _pawn(pawn)
{
    _pawnImage = QPixmap("./Assets/pawn.jpg");
    setPixmap(_pawnImage);
    setOffset(Helpers::cubeToPixel(_pawn->getCoordinates()));
    setAcceptDrops(true);
}

void PawnItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setCursor(Qt::ClosedHandCursor);

    event->accept();
}

void PawnItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    setCursor(Qt::OpenHandCursor);
}

void PawnItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QDrag* drag = new QDrag(parentObject());
    QMimeData* mime = new QMimeData;
    drag->setMimeData(mime);


    // Greyed out picture of the pawn
    QPixmap tempPixmap = _pawnImage;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(_pawnImage.rect(), QColor(127, 127, 127, 127));
    painter.end();
    mime->setImageData(tempPixmap);

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
        setParentItem(nullptr);
    }
    setCursor(Qt::OpenHandCursor);
}

}

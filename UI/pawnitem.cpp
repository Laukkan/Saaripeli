#include "pawnitem.hh"
#include "helpers.hh"
#include "constants.hh"

#include <QDrag>
#include <QCursor>
#include <QMimeData>
#include <QPainter>
#include <QString>


namespace Student {

PawnItem::PawnItem(QString color,
                   std::shared_ptr<Common::Pawn> pawn,
                   HexItem* parent):
    _pawn(pawn), _color(color)
{
    _pawnImage.load(PathConstants::PAWN_IMAGES.at(_color));
    setPixmap(_pawnImage.scaled(SizeConstants::P_PIX_SIZE,
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation));
    setOffset(parent->getPawnPosition(pawn->getId()));

    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setCursor(Qt::OpenHandCursor);
    setParent(parent);
}

QString PawnItem::getColor()
{
    return _color;
}

int PawnItem::getId() const
{
    return _pawn->getId();
}

void PawnItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::ClosedHandCursor);
}

void PawnItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    setCursor(Qt::OpenHandCursor);
}

void PawnItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
    QDrag* drag = new QDrag(parent());
    QMimeData* mime = new QMimeData;
    drag->setMimeData(mime);

    // Move information of the current parent and pawn Id
    mime->setParent(parent());
    mime->setText("pawn;" + QString::number(_pawn->getId()));

    drag->setPixmap(_pawnImage);
    drag->exec();
    setCursor(Qt::OpenHandCursor);
}

}

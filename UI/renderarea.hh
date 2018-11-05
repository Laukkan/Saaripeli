#ifndef RENDERAREA_HH
#define RENDERAREA_HH

#include <QWidget>
#include <QPen>
#include <QBrush>

struct Center
{
    int x = 0;
    int y = 0;
    Center(int x, int y) : x(x), y(y) {}
};

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QWidget* _parent;
    QPen _hexpen;

    void drawHex(QPainter* painter, Center center);

    void drawRowOfHexes(QPainter* painter, Center rightMostCenter, int numberOfHexes);

    QPointF pointyHexCorner(Center center, int size, int side);
};

#endif // RENDERAREA_HH

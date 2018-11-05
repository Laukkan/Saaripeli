#ifndef RENDERAREA_HH
#define RENDERAREA_HH

#include <QWidget>
#include <QPen>
#include <QBrush>

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

    void drawHex(QPainter* painter);
};

#endif // RENDERAREA_HH

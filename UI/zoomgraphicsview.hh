#ifndef ZOOMGRAPHICSVIEW_HH
#define ZOOMGRAPHICSVIEW_HH
#include <QGraphicsView>
#include <QMainWindow>
#include <QWheelEvent>

class ZoomGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    ZoomGraphicsView();

public slots:
    //void scalingTime();

    //void animationFinsihed();
private:
    //int _scheduledScalings;
    //double _currentZoom;
    /**
     * @brief wheelEvent
     * @param event
     */
    virtual void wheelEvent(QWheelEvent* event) override;
};

#endif // ZOOMGRAPHICSVIEW_HH

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <renderarea.hh>

#include <QMainWindow>
#include <QGraphicsView>


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void drawGameBoard(QGraphicsScene* scene);

    void drawRowOfHexes(QGraphicsScene* scene,
                        Center rightMostCenter,
                        int numberOfHexes);

    QPointF pointyHexCorner(QGraphicsScene* scene, int size, int side);

signals:

public slots:

private:
    RenderArea* _renderArea;
};

#endif // MAINWINDOW_HH

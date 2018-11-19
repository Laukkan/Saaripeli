#ifndef STARTDIALOG_HH
#define STARTDIALOG_HH
#include <QDialog>

class StartDialog : public QDialog
{
    Q_OBJECT
public:
    StartDialog();
    int getPlayers();

signals:
    void confirmed(int _players);

public slots:
    void playersChange(const QString & text);
    void confirm();

private:
    int _players;
};

#endif // STARTDIALOG_HH

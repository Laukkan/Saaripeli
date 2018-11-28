#ifndef STARTDIALOG_HH
#define STARTDIALOG_HH
#include <QDialog>

namespace Student {


class StartDialog : public QDialog
{
    Q_OBJECT
public:
    StartDialog();
    int getPlayers();

public slots:
    void playersChange(const QString & text);

private:
    int _players;
};

}

#endif // STARTDIALOG_HH

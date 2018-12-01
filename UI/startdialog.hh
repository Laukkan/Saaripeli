#ifndef STARTDIALOG_HH
#define STARTDIALOG_HH
#include <QDialog>

namespace Student {


class StartDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief StartDialog's constructor. Constructs all of the elements
     * required for the dialog.
     */
    StartDialog();

    /**
     * @brief getPlayers - returns the value of _players
     * @return _players
     */
    int getPlayers();

public slots:
    /**
     * @brief playersChange - updates the _players when the QComboBox text
     * is changed.
     * @param text - the new changed text (number)
     */
    void playersChange(const QString &text);

private:
    /**
     * @brief _players - stores the QComboBox's value
     */
    int _playersAmount;
};

}

#endif // STARTDIALOG_HH

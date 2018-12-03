#include "startdialog.hh"
#include "constants.hh"

#include <memory>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <string>
#include <QPushButton>

//

namespace Student {

StartDialog::StartDialog() : _playersAmount(1)
{
   QGridLayout* dialogLayout = new QGridLayout(this);
   QLabel* playersLabel = new QLabel("Number of players", this);

   QComboBox* playersAmount = new QComboBox(this);
   connect(playersAmount, &QComboBox::currentTextChanged,
           this, &StartDialog::playersChange);

   QStringList allowedAmounts;
   for (int i = GameConstants::MIN_PLAYERS; i <= GameConstants::MAX_PLAYERS;
        ++i)
   {
        allowedAmounts.append(QString::number(i));
   }
   playersAmount->addItems(allowedAmounts);

   QPushButton* okButton = new QPushButton("Confirm", this);
   connect(okButton, &QPushButton::pressed, this, &StartDialog::accept);

   dialogLayout->addWidget(playersLabel);
   dialogLayout->addWidget(playersAmount);
   dialogLayout->addWidget(okButton);

   setLayout(dialogLayout);
}

int StartDialog::getPlayers()
{
    return _playersAmount;
}

void StartDialog::playersChange(const QString &text)
{
    _playersAmount = text.toInt();
}


}

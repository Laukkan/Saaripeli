#include "startdialog.hh"
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
   QLabel* playersLabel = new QLabel(this);


   QComboBox* playersAmount = new QComboBox(this);
   connect(playersAmount, &QComboBox::currentTextChanged,
           this, &StartDialog::playersChange);

   QPushButton* okButton = new QPushButton("Confirm", this);
   connect(okButton, &QPushButton::pressed, this, &StartDialog::accept);

   playersLabel->setText("Number of players");

   QStringList allowedAmounts = {"1", "2", "3"};
   playersAmount->addItems(allowedAmounts);

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

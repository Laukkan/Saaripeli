#include "startdialog.hh"
#include <memory>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <string>
#include <QPushButton>

//

namespace Student {

StartDialog::StartDialog()
{
   _players = 1;

   QGridLayout* dialogLayout = new QGridLayout(this);
   QLabel* playersLabel = new QLabel(this);
   QComboBox* playersAmount = new QComboBox(this);
   QPushButton* okButton = new QPushButton(this);

   playersLabel->setText("Number of players");
   dialogLayout->addWidget(playersLabel);

   QStringList allowedAmounts = {"1", "2", "3"};
   playersAmount->addItems(allowedAmounts);
   dialogLayout->addWidget(playersAmount);

   okButton->setText("Confirm");
   dialogLayout->addWidget(okButton);

   setLayout(dialogLayout);

   connect(playersAmount, &QComboBox::currentTextChanged,
           this, &StartDialog::playersChange);
   connect(okButton,&QPushButton::pressed,this,&StartDialog::confirm);
   connect(okButton,&QPushButton::pressed,this,&StartDialog::accept);

}

int StartDialog::getPlayers()
{
    return _players;
}

void StartDialog::playersChange(const QString &text)
{
    _players = text.toInt();
}

void StartDialog::confirm()
{
    emit confirmed(_players);
}

}

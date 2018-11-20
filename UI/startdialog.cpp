#include "startdialog.hh"
#include <memory>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <string>
#include <QPushButton>

namespace Student {

StartDialog::StartDialog()
{
   _players = 1;

   QGridLayout* dialogLayout = new QGridLayout();
   QLabel* playersLabel = new QLabel;
   QComboBox* playersAmount = new QComboBox;
   QPushButton* okButton = new QPushButton(this);

   playersLabel->setText("Number of players");
   dialogLayout->addWidget(playersLabel);

   for(int i = 1; i <= 4; i++){
       playersAmount->addItem(QString::number(i));
   }
   dialogLayout->addWidget(playersAmount);

   okButton->setText("Confirm");
   dialogLayout->addWidget(okButton);

   setLayout(dialogLayout);

   connect(playersAmount, &QComboBox::currentTextChanged,this, &StartDialog::playersChange);
   connect(okButton,&QPushButton::pressed,this,&StartDialog::confirm);
   connect(okButton,&QPushButton::pressed,this,&StartDialog::accept);

}

int StartDialog::getPlayers()
{
    return _players;
}

void StartDialog::playersChange(const QString & text)
{
    _players = text.toInt();
}

void StartDialog::confirm()
{
    emit confirmed(_players);
}

}

#include <mainwindow.hh>
#include <ioexception.hh>
#include <formatexception.hh>
#include <startdialog.hh>

#include <memory>
#include <QApplication>
#include <QMessageBox>


/**
 * @brief criticalError - Opens a critical error box with the message
 * @param message
 */
void criticalError(const std::string &message);


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/ak47_icon.png"));

    Student::MainWindow m;
    Student::StartDialog startDialog;

    if (startDialog.exec() != QDialog::Accepted) {
        return 0;
    }
    try {
        m.initBoard(startDialog.getPlayers(), false);
    }
    catch (Common::IoException &e) {
        criticalError(e.msg());
        return 0;
    }
    catch (Common::FormatException &e) {
        criticalError(e.msg());
        return 0;
    }
    m.show();
    return a.exec();
}

void criticalError(const std::string &message)
{
    QMessageBox error;
    error.critical(nullptr,
                   QString::fromStdString("Error"),
                   QString::fromStdString(message));
}

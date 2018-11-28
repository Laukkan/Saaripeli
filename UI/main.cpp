#include <mainwindow.hh>
#include <ioexception.hh>
#include <formatexception.hh>
#include <startdialog.hh>

#include <memory>
#include <QApplication>
#include <QMessageBox>

void criticalError(QMessageBox &error, const std::string &message);

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/ak47_icon.png"));

    QMessageBox error;
    Student::MainWindow m;
    Student::StartDialog startDialog;

    if (startDialog.exec() != QDialog::Accepted) {
        error.warning(nullptr,
                       QString::fromStdString("Error"),
                       QString::fromStdString("Dialog was exited."));
        return 0;
    }
    try {
        m.initBoard(startDialog.getPlayers());
    }
    catch (Common::IoException &e) {
        criticalError(error, e.msg());
        return 0;
    }
    catch (Common::FormatException &e) {
        criticalError(error, e.msg());
        return 0;
    }
    m.show();
    return a.exec();
}

void criticalError(QMessageBox &error, const std::string &message)
{
    error.critical(nullptr,
                   QString::fromStdString("Error"),
                   QString::fromStdString(message));
}

#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    // Load the help file
    QFile help(":/docs/docs/help.html");
    if (help.open(QFile::ReadOnly)) {
        QTextStream stream(&help);
        QString contentHtml;

        // Convert the readed data to html
        contentHtml = stream.readAll();
        ui->textEdit->setHtml(contentHtml);
    }

}

HelpDialog::~HelpDialog()
{
    delete ui;
}

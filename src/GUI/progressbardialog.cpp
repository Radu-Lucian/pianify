#include "progressbardialog.h"
#include "ui_progressbardialog.h"

ProgressBarDialog::ProgressBarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressBarDialog)
{
    ui->setupUi(this);
	ui->progressBar->setMinimum(0);
	ui->progressBar->setMaximum(0);
	ui->progressBar->setValue(0);
}

ProgressBarDialog::~ProgressBarDialog()
{
    delete ui;
}

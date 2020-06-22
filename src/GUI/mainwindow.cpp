#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "converterdialog.h"
#include "instrumentrecognizerdialog.h"

#include <QPixmap>

#include <directories.h>

#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	
	connect(ui->converterWindowButton, SIGNAL(clicked()), this, SLOT(onConverterWindowButtonClicked()));
	connect(ui->recognizeWindowButton, SIGNAL(clicked()), this, SLOT(onInstrumentRecognizerWindowButtonClicked()));

	QPalette pal = palette();
	pal.setColor(QPalette::Background, Qt::white);

	ui->widget_main_body->setAutoFillBackground(true);
	ui->widget_main_body->setPalette(pal);

	ui->widget_title->setAutoFillBackground(true);
	ui->widget_title->setPalette(pal);

	static const auto resourcesPath = fs::path(directories::getResourcesDir());
	static constexpr auto logoPathString = std::string_view("soundwave2.jpg");
	static const auto logoPath = fs::path(logoPathString);

	QPixmap pix(((resourcesPath / logoPathString).string()).data());
	ui->label_picture->setPixmap(pix);

}

void MainWindow::onConverterWindowButtonClicked()
{
	ConverterDialog* secDialog = new ConverterDialog(this);
	secDialog->setFixedSize(1110, 675);
	secDialog->setModal(true);
	secDialog->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onInstrumentRecognizerWindowButtonClicked()
{
	InstrumentRecognizerDialog* recognizer = new InstrumentRecognizerDialog(this);
	recognizer->setFixedSize(860, 450);
	recognizer->setModal(true);
	recognizer->show();
}

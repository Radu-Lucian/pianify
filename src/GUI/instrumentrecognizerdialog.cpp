#include "instrumentrecognizerdialog.h"
#include "ui_instrumentrecognizerdialog.h"

#include "qmessagebox.h"

#include <filesystem>
#include <string_view>

#include "directories.h"

#include <QPixmap>
#include <QFileDialog>

#include <WaveFile.h>
#include <FileWriter.h>
#include <Model.h>
#include "MidiExporter.h"
#include "FourierTransform.h"
#include "aquila/source/FramesCollection.h"

namespace fs = std::filesystem;

InstrumentRecognizerDialog::InstrumentRecognizerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstrumentRecognizerDialog)
{
    ui->setupUi(this);
	ui->recognizerProgressBar->setVisible(false);

	connect(ui->backButton, SIGNAL(clicked()), this, SLOT(onBackButtonClicked()));
	connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(onLoadButtonClicked()));
	connect(ui->recognizeButton, SIGNAL(clicked()), this, SLOT(onInstrumentRecognizerButtonClicked()));
	connect(this, SIGNAL(loadButtonWasClicked()), this, SLOT(disableGroupBox()));
	connect(this, SIGNAL(loadButtonWasClicked()), this, SLOT(setLabelsEmpty()));
	connect(this, SIGNAL(loadingIsDone()), this, SLOT(enableGroupBox()));
	connect(this, SIGNAL(processingIsDone()), this, SLOT(setProcessBarInvisible()));
	connect(this, SIGNAL(InstrumentRecognizerButtonWasClicked()), this, SLOT(startProcessingBar()));
	connect(this, SIGNAL(InstrumentRecognizerButtonWasClicked()), this, SLOT(disableRecognizeButton()));
	connect(this, SIGNAL(recognitionIsDone()), this, SLOT(showMessageBox()));
	connect(this, SIGNAL(recognitionIsDone()), this, SLOT(enableRecognizeButton()));

	QPalette pal = palette();
	pal.setColor(QPalette::Background, Qt::white);
	ui->windowGroupBox->setAutoFillBackground(true);
	ui->windowGroupBox->setPalette(pal);

	auto resourcePath = fs::path(directories::getResourcesDir());
	static constexpr std::string_view pianoLogoFileName = "piano.jpg";
	static constexpr std::string_view trumpetLogoFileName = "trumpet.jpg";

	QPixmap pianoPixmap((resourcePath / pianoLogoFileName).string().data());
	QPixmap trumpetPixmap((resourcePath / trumpetLogoFileName).string().data());

	ui->labelPicturePiano->setPixmap(pianoPixmap.scaled(270,120,Qt::KeepAspectRatio));
	ui->labelPictureTrumpet->setPixmap(trumpetPixmap.scaled(270, 120, Qt::KeepAspectRatio));
}

InstrumentRecognizerDialog::~InstrumentRecognizerDialog()
{
    delete ui;
}

void InstrumentRecognizerDialog::recognizeInstuments()
{
	const int kExtractNumberForAI = ui->extractNumberSpinBox->value();
	constexpr int kTrumpetNumberToConvert = 56;
	constexpr int kTrumpetNumberBefore = 1;
	constexpr int kPianoNumber = 0;

	auto midiExporter = std::make_unique<MidiExporter>(*m_loadedWaveFile);
	m_midiOutPath = std::string(directories::getMidiGeneratedDir()) + directories::kPathSeparator + m_loadedFileName + ".mid";
	auto fourierType = utilities::FourierType::ShortTimeFourierTransformAquila;

	auto instrumentsAparitions = midiExporter->writeToFile(m_midiOutPath, fourierType, Extractor::ExtractionType::Fixed, kExtractNumberForAI, true);
	midiExporter->endWrite();

	auto numberOfWindows = instrumentsAparitions[1] + instrumentsAparitions[0];
	double trumpetProb = (static_cast<double>(instrumentsAparitions[1]) / numberOfWindows) * 10000;
	int trumpetIntProb = trumpetProb;
	double trumpetDoubleProb = trumpetIntProb / 100;
	double pianoProb = 100.0 - trumpetDoubleProb;
	ui->pianoLabel->setText(QString(std::to_string(pianoProb).c_str()) + QString("%"));
	ui->trumpetLabel->setText(QString(std::to_string(trumpetDoubleProb).c_str()) + QString("%"));
	ui->recognizeButton->setEnabled(false);
	emit recognitionIsDone();
}

void InstrumentRecognizerDialog::onInstrumentRecognizerButtonClicked()
{
	emit InstrumentRecognizerButtonWasClicked();
	std::thread recognizerThread([this] {recognizeInstuments(); });
	recognizerThread.detach();
}


void InstrumentRecognizerDialog::enableGroupBox()
{
	ui->windowGroupBox->setEnabled(true);
}

void InstrumentRecognizerDialog::disableGroupBox()
{
	ui->windowGroupBox->setEnabled(false);
	ui->windowGroupBox->repaint();   //fixes buttons not being disabled before plot
}

void InstrumentRecognizerDialog::startProcessingBar()
{
	ui->recognizerProgressBar->setVisible(true);
	ui->recognizerProgressBar->setMinimum(0);
	ui->recognizerProgressBar->setMaximum(0);
	ui->recognizerProgressBar->setValue(0);
}

void InstrumentRecognizerDialog::setProcessBarInvisible()
{
	ui->recognizerProgressBar->setVisible(false);
}

void InstrumentRecognizerDialog::showMessageBox()
{
		
		emit processingIsDone();//ui->progressBar->setVisible(false);
		QMessageBox::information(this, "Instrument recognition results", QString::fromStdString("The instrument recognition has succedeed! \nThe file was written to: " + m_midiOutPath), QMessageBox::Ok);
	
}

void InstrumentRecognizerDialog::setLabelsEmpty()
{
	ui->pianoLabel->setText("Piano: 0%");
	ui->trumpetLabel->setText("Trumpet: 0%");
}

void InstrumentRecognizerDialog::disableRecognizeButton()
{
	ui->recognizeButton->setEnabled(false);
}

void InstrumentRecognizerDialog::enableRecognizeButton()
{
	ui->recognizeButton->setEnabled(true);
}

void InstrumentRecognizerDialog::onLoadButtonClicked()
{
	emit loadButtonWasClicked();
	const auto filePath = QFileDialog::getOpenFileName(this, "Opening audio file..", QString::fromUtf8(directories::getAudioSamplesDir()), "*.wav").toStdString();
	m_loadedFileName = fs::path(filePath).stem().string();

	try
	{
		m_loadedWaveFile = std::make_unique<WaveFile>(filePath);
		ui->recognizeButton->setEnabled(true);
	}
	catch (const std::invalid_argument&)
	{
		ui->recognizeButton->setEnabled(false);
		ui->selectedSongLabel->setText("No audio file loaded");
		emit loadingIsDone();
		return;
	}
	
	ui->selectedSongLabel->setText("Current loaded file: " + QString::fromStdString(m_loadedFileName + ".wav"));
	emit loadingIsDone();
	ui->recognizeButton->setEnabled(true);
}

void InstrumentRecognizerDialog::onBackButtonClicked()
{
	this->close();
}

#include "converterdialog.h"
#include "ui_converterdialog.h"
#include "progressbardialog.h"

#include <QString>

#include <filesystem>
#include <thread>
#include <string>

#include <directories.h>

#include <WaveFile.h>
#include <FourierTransform.h>
#include <Extractor.h>
#include <MidiExporter.h>
#include <MidiGenerator.h>
#include <vector>

namespace fs = std::filesystem;
using utilities::FourierType;

ConverterDialog::ConverterDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConverterDialog)
{
	ui->setupUi(this);

	ui->customPlotTime->setInteraction(QCP::iRangeZoom, true);
	ui->customPlotTime->setInteraction(QCP::iRangeDrag, true);

	ui->progressBar->setVisible(false);

	connect(ui->backButton, SIGNAL(clicked()), this, SLOT(onBackButtonClicked()));
	connect(ui->plotButton, SIGNAL(clicked()), this, SLOT(onPlotButtonClicked()));
	connect(ui->fileLoadButton, SIGNAL(clicked()), this, SLOT(onloadFileButtonclicked()));
	connect(ui->variableExtractionCheckBox, SIGNAL(clicked(bool)), this, SLOT(setVariableExtractionBox()));
	connect(ui->convertButton, SIGNAL(clicked()), this, SLOT(onConvertButtonClicked()));
	connect(this, SIGNAL(fileWasLoaded()), this, SLOT(enableSettingGrupBox()));
	connect(this, SIGNAL(midiWasGenerated()), this, SLOT(showSuccessMessageBox()));
	connect(this, SIGNAL(midiWasGenerated()), this, SLOT(enableConverterButton()));
	connect(this, SIGNAL(processingIsDone()), this, SLOT(setProcessBarInvisible()));
	connect(this, SIGNAL(loadButtonWasClicked()), this, SLOT(disableSettingGrupBox()));

	
	setBackgroundWhite();
}

void ConverterDialog::setBackgroundWhite()
{
	QPalette pal = palette();
	pal.setColor(QPalette::Background, Qt::white);

	ui->dataGroupBox->setAutoFillBackground(true);
	ui->dataGroupBox->setPalette(pal);

	ui->settingsGroupBox->setAutoFillBackground(true);
	ui->settingsGroupBox->setPalette(pal);
}

void ConverterDialog::disableConverterButton()
{
	ui->convertButton->setEnabled(false);
}

void ConverterDialog::onloadFileButtonclicked()
{
	emit loadButtonWasClicked();
	const auto filePath = QFileDialog::getOpenFileName(this, "Opening audio file..", QString::fromUtf8(directories::getAudioSamplesDir()), "*.wav").toStdString();
	m_loadedFileName = fs::path(filePath).stem().string();
	try
	{
		clearGraph();
		m_loadedWaveFile = std::make_unique<WaveFile>(filePath);
	}
	catch (const std::invalid_argument& e)
	{
	
		ui->selectedSongLabel->setText("No audio file loaded..");
		emit fileWasLoaded();
		ui->convertButton->setEnabled(false);
		ui->plotButton->setEnabled(false);
		return;
	}

	ui->selectedSongLabel->setText("Current loaded file: " + QString::fromStdString(m_loadedFileName + ".wav"));
	ui->convertButton->setEnabled(true);
	ui->plotButton->setEnabled(true);
	emit fileWasLoaded();
}

void ConverterDialog::generateMidi()
{
	m_midiExporter = std::make_unique<MidiExporter>(*m_loadedWaveFile);
	FourierType fourierType = utilities::stringToFourierType.at(ui->fourierSelectComboBox->currentText().toStdString());

	static constexpr std::string_view targetDirectoryString("./midi_generated/");
	if (!fs::exists(targetDirectoryString))
		fs::create_directory(targetDirectoryString);

	m_midiOutPath = std::string(fs::absolute(targetDirectoryString).string() + m_loadedFileName + ".mid");

	if(ui->variableExtractionCheckBox->isChecked())
		m_midiExporter->writeToFile(m_midiOutPath, fourierType, Extractor::ExtractionType::Variable);
	else	 
		m_midiExporter->writeToFile(m_midiOutPath, fourierType, Extractor::ExtractionType::Fixed, ui->extractNumberSpinBox->value());

	m_midiExporter->endWrite();
	emit midiWasGenerated();
}

bool ConverterDialog::isExtractionNumberValid()
{
	if (!ui->extractNumberSpinBox->value() && !ui->variableExtractionCheckBox->isChecked())
	{
		QMessageBox::warning(this, "Invalid extraction number", "You have selected 0 as the extraction number. The Midi converter requires a number different than 0.", QMessageBox::Ok);
		return false;
	}
	return true;
}

void ConverterDialog::clearGraph()
{
	if (ui->customPlotTime->graph())
	{
		ui->customPlotTime->graph(0)->data()->clear();
		ui->customPlotTime->replot();
	}
}

void ConverterDialog::onConvertButtonClicked()
{
	if (isExtractionNumberValid())
	{
		startProgressBar();
		disableConverterButton();
		std::thread converterThread([this] {generateMidi(); });
		converterThread.detach();
	}
}

void ConverterDialog::onBackButtonClicked()
{
	this->close();
}


ConverterDialog::~ConverterDialog()
{
	delete ui;
}

void ConverterDialog::startProgressBar()
{
	ui->progressBar->setVisible(true);
	ui->progressBar->setMinimum(0);
	ui->progressBar->setMaximum(0);
	ui->progressBar->setValue(0);
}

void ConverterDialog::setProcessBarInvisible()
{
	ui->progressBar->setVisible(false);
}

void ConverterDialog::enableConverterButton()
{
	ui->convertButton->setEnabled(true);
}

void ConverterDialog::onPlotButtonClicked()
{
	disableSettingGrupBox();
	makePlot();
	enableSettingGrupBox();
	ui->plotButton->setEnabled(false);
  
	
	//ProgressBarDialog* prog = new ProgressBarDialog(this);
	//prog->show();
	
	//startProgressBar();
	//std::thread thread([this] {makePlot(); });
	//thread.detach();

}

void ConverterDialog::setVariableExtractionBox()
{
	ui->extractNumberSpinBox->setEnabled(!ui->variableExtractionCheckBox->isChecked());
}

void ConverterDialog::enableSettingGrupBox()
{
	ui->settingsGroupBox->setEnabled(true);
}

void ConverterDialog::disableSettingGrupBox()
{
	ui->settingsGroupBox->setEnabled(false);
	ui->settingsGroupBox->repaint();   //fixes buttons not being disabled before plot
}

std::pair<QVector<double>, QVector<double>> ConverterDialog::generateAxes()
{
	const auto sampleSize = m_loadedWaveFile->getTotalNumberOfSamples();
	const auto timeIncrement = 1 / m_loadedWaveFile->getSampleRate();
	QVector<double> xTimeStamps(sampleSize);
	QVector<double> yAmplitude(sampleSize);

	for (int i = 0; i < sampleSize; i++)
	{
		xTimeStamps[i] = i * timeIncrement;
		yAmplitude[i] = m_loadedWaveFile->getSample(i);
	}

	return { xTimeStamps, yAmplitude };
}

void ConverterDialog::makePlot()
{

	auto[xTimeStamps, yAmplitude] = generateAxes();
	ui->customPlotTime->addGraph();
	ui->customPlotTime->graph(0)->setData(xTimeStamps, yAmplitude);
	ui->customPlotTime->xAxis->setLabel("Time");
	ui->customPlotTime->yAxis->setLabel("Amplitude");
	ui->customPlotTime->rescaleAxes();
	ui->customPlotTime->replot();
}

void ConverterDialog::showSuccessMessageBox()
{
	emit processingIsDone();
	QMessageBox::information(this, "Conversion results", QString::fromStdString("The Midi conversion has succedeed!\nThe file was written to: " +  m_midiOutPath), QMessageBox::Ok);
}



#pragma once

#include <QDialog>

#include <memory>

#include <MidiExporter.h>
#include <WaveFile.h>
#include <qvector.h>

namespace Ui {
class ConverterDialog;
}

class ConverterDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ConverterDialog(QWidget *parent = nullptr);
	~ConverterDialog();

private slots:
	void onBackButtonClicked();
	void onPlotButtonClicked();
	void onloadFileButtonclicked();
	void onConvertButtonClicked();

	void setVariableExtractionBox();
	void enableSettingGrupBox();
	void disableSettingGrupBox();
	void makePlot();
	void showSuccessMessageBox();
	void startProgressBar();
	void setProcessBarInvisible();
	void enableConverterButton();

signals:
	void fileWasLoaded();
	void midiWasGenerated();
	void processingIsDone();
	void loadButtonWasClicked();

private:
	void generateMidi();
	bool isExtractionNumberValid();
	void clearGraph();
	void setBackgroundWhite();
	void disableConverterButton();
	std::pair<QVector<double>, QVector<double>> generateAxes();
	


private:
	Ui::ConverterDialog *ui;
	std::string m_loadedFileName;
	std::string m_midiOutPath;
	std::unique_ptr<WaveFile> m_loadedWaveFile;
	std::unique_ptr<MidiExporter> m_midiExporter;
};
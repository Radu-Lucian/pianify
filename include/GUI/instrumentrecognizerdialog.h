#pragma once

#include <QDialog>

class WaveFile;

namespace Ui {
class InstrumentRecognizerDialog;
}

class InstrumentRecognizerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InstrumentRecognizerDialog(QWidget *parent = nullptr);
    ~InstrumentRecognizerDialog();

private slots:
	void onInstrumentRecognizerButtonClicked();
	void onLoadButtonClicked();
	void onBackButtonClicked();
	void enableGroupBox();
	void disableGroupBox();
	void startProcessingBar();
	void setProcessBarInvisible();
	void showMessageBox();
	void setLabelsEmpty();
	void disableRecognizeButton();
	void enableRecognizeButton();

signals:
	void loadButtonWasClicked();
	void loadingIsDone();
	void InstrumentRecognizerButtonWasClicked();
	void processingIsDone();
	void recognitionIsDone();

private:
	void recognizeInstuments();

private:
    Ui::InstrumentRecognizerDialog *ui;
	std::string m_loadedFileName;
	std::unique_ptr<WaveFile> m_loadedWaveFile;
	std::string m_midiOutPath;
};


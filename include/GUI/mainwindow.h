#pragma once
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void onConverterWindowButtonClicked();
	void onInstrumentRecognizerWindowButtonClicked();

private:
	Ui::MainWindow *ui;
};
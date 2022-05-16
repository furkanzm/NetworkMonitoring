#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	net = new networkMonitoring("10.5.5.0:30561");
}

MainWindow::~MainWindow()
{
	delete ui;
}

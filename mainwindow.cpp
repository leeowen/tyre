#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList stretchLists;
    stretchLists<< "5%" << "10%";
    ui->comboStretchPercentageBox->addItems(stretchLists);
    ui->stepSpinBox->setValue(ui->renderArea->getStep());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_originBtn_clicked()
{
    this->ui->renderArea->setShape(RenderArea::Origin);
    this->ui->renderArea->repaint();
}

void MainWindow::on_stretchBtn_clicked()
{
    this->ui->renderArea->setShape(RenderArea::Stretch);
    this->ui->renderArea->repaint();
}

void MainWindow::on_comboStretchPercentageBox_currentTextChanged(const QString &stretch)
{
    int percentage=stretch.toInt();
    this->ui->renderArea->setStretch(percentage);
}



void MainWindow::on_stepSpinBox_valueChanged(int step)
{
    this->ui->renderArea->setStep(step);
}

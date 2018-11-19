#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QColorDialog>

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
    QStringList pieces = stretch.split( "%" );
    int percentage=pieces[0].toInt();
    this->ui->renderArea->setStretchPercentage(percentage);
}



void MainWindow::on_stepSpinBox_valueChanged(int step)
{
    if(step%2==1)
    {
        QMessageBox msgBox;
        msgBox.setText("Step count has to be even number, find the nearest even number for you.");
        msgBox.exec();
        this->ui->stepSpinBox->setValue(step+1);
    }
    this->ui->renderArea->setStep(step);
}

void MainWindow::on_LineColorBtn_clicked()
{
    //open a color picker dialogue
    QColor color=QColorDialog::getColor(Qt::red,this,"Select Color");
    ui->renderArea->setShapeColor(color);
}

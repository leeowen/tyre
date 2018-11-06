#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

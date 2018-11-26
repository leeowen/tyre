#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QColorDialog>
#include<QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList stretchLists;
    stretchLists<< "perimeter" << "area"<<"fixed length";
    ui->comboStretchTypeBox->addItems(stretchLists);
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

void MainWindow::on_comboStretchTypeBox_currentTextChanged(const QString &str)
{
//    QStringList pieces = stretch.split( "%" );
//    int percentage=pieces[0].toInt();
//    this->ui->renderArea->setStretchPercentage(percentage);
    this->ui->renderArea->setStretchType(str);
    if(str=="fixed length")
    {

    }
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

void MainWindow::on_actionSave_triggered()
{
    QString fileName=QFileDialog::getSaveFileName(
                this,
                "save",
                "/Users");
    if(!fileName.isEmpty()){
        saveFile(fileName);
    }
}

void MainWindow::saveFile(QString fileName)
{
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly)){
        QPixmap pixmap(this->ui->renderArea->size());
        this->ui->renderArea->render(&pixmap);

        pixmap.save(&file, "PNG");
        file.close();
    }
    else{
        QMessageBox::warning(
                    this,
                    "tyre",
                    tr("Cannot write file %1.\nEroor:%2").arg(fileName).arg(file.errorString()));
    }
}

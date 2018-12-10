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
    stretchLists<< "ellipse" << "perimeter" << "area" << "fixed length";
    ui->comboStretchTypeBox->addItems(stretchLists);
    ui->renderArea->setRadiusA(ui->a_axisSpinBox->value());
    ui->renderArea->setRadiusB(ui->b_axisSpinBox->value());
    ui->renderArea->setStep(ui->stepSpinBox->value());
    ui->renderArea->setCursor(Qt::CrossCursor);
    ui->renderArea->setStretchFixedLength(ui->doubleSpinBox->value());
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
    this->ui->renderArea->setStretchType(str);
    if(str=="fixed length")
    {
        this->ui->lengthLabel->setEnabled(true);
        this->ui->doubleSpinBox->setEnabled(true);
    }
    else
    {
        this->ui->lengthLabel->setEnabled(false);
        this->ui->doubleSpinBox->setEnabled(false);
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
                "../../",
                "(*.png);;All Files (*)");
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

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    this->ui->renderArea->setStretchFixedLength(arg1);
    int percentage=(arg1-1)*100;
    this->ui->renderArea->setMelName("StretchBy"+QString::number(percentage)+"Percent");
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;
    this->ui->renderArea->setShapeColor(QColor(r,g,b));
}

void MainWindow::on_save_to_maya_clicked()
{
    QString fileName=QFileDialog::getSaveFileName(
                this,
                "save",
                "",
                "(*.mel);;All Files (*)");
    if(!fileName.isEmpty()){
            this->ui->renderArea->saveToMaya(fileName);
    }
}

void MainWindow::on_a_axisSpinBox_valueChanged(int arg1)
{
    this->ui->renderArea->setRadiusA(arg1);
}

void MainWindow::on_b_axisSpinBox_valueChanged(int arg1)
{
    this->ui->renderArea->setRadiusB(arg1);
}

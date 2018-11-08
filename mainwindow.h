#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void on_originBtn_clicked();

    void on_stretchBtn_clicked();

    void on_comboStretchPercentageBox_currentTextChanged(const QString &percentage);


    void on_stepSpinBox_valueChanged(int step);

    void on_LineColorBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

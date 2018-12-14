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

    void on_comboStretchTypeBox_currentTextChanged(const QString &str);

    void on_stepSpinBox_valueChanged(int step);

    void on_LineColorBtn_clicked();

    void on_actionSave_triggered();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_save_to_maya_clicked();

    void on_a_axisSpinBox_valueChanged(int arg1);

    void on_b_axisSpinBox_valueChanged(int arg1);

    void on_ksSlider_valueChanged(int value);

    void on_kbSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    void saveFile(QString fileName);
};

#endif // MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;
    void saveFile(QString fileName);
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void numberGroup_clicked(QAbstractButton*);
    void actionGroup_clicked(QAbstractButton*);

    void on_actionDel_clicked();
    void on_actionCalc_clicked();
    void on_comma_clicked();
    void on_actionClear_clicked();
    void on_actionPercent_clicked();
    void on_actionSign_clicked();

private:
    Ui::MainWindow *ui;
    //Предел цифр
    const int DIGIT_LIMIT = 16;
    //Flag to check whether the previous button that was clicked was an operator
    bool operatorClicked;
    //Последний запрошенный оператор
    QChar storedOperator;
    //Флаг, чтобы проверить, сохранено ли число в памяти
    bool hasStoredNumber;
    //Сохраненный номер
    double storedNumber;
    //Рассчитывает результат на основе сохраненного числа и отображаемого числа
    void calculate_result();

protected:
    void keyPressEvent(QKeyEvent *e);
};

#endif // MAINWINDOW_H

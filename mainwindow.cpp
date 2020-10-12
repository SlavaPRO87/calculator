#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   //создает фактические экземпляры виджетов
    ui->setupUi(this);

    //Очищаем окно вывода
    ui->displayPanel->clear();

    //Установить флаг оператора в false
    operatorClicked = false;
    hasStoredNumber = false;

    //Установить номер слушателя группы кнопок
    ui->numberGroup->connect(ui->numberGroup,SIGNAL(buttonClicked(QAbstractButton*)),
                             this, SLOT(numberGroup_clicked(QAbstractButton*)));
    //Установить прослушиватель группы кнопок оператора
    ui->actionGroup->connect(ui->actionGroup,SIGNAL(buttonClicked(QAbstractButton*)),
                             this, SLOT(actionGroup_clicked(QAbstractButton*)));

    //Установить ширину и высоту окна
    this->setFixedSize(QSize(306, 319));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//==================================================================================
//Функции слота
//==================================================================================
// Вызывается всякий раз, когда нажимается цифровая кнопка
void MainWindow::numberGroup_clicked(QAbstractButton* button)
{
    //Получаем строку с дисплея
    QString displayLabel = ui->displayPanel->text();

    /* Проверка, была ли предыдущая нажатая кнопка кнопкой оператора
     * Если это так, очистите дисплей и установите флаг в значение false. */
    if (operatorClicked) {
        displayLabel.clear();
        operatorClicked = false;
    }

    //Если длинна введенной строки больше лимита, больше не добавляем цифры
    if (displayLabel.length() >= DIGIT_LIMIT) {
        return;
    }

    //Добавляем нажатую цифру к строке
    displayLabel.append(button->text());

    //Выводим получившуюся строку
    ui->displayPanel->setText(displayLabel);
}

// CВызывается всякий раз, когда нажимается кнопка действия
void MainWindow::actionGroup_clicked(QAbstractButton* button)
{
    /* Если предыдущая нажата кнопка не была оператором, то нам просто нужно сохранить оператор.
     * что было запрошено и выйти.
     * Если бы это было так, нам нужно посмотреть, нужно ли нам просто сохранить число, которое
     * отображается или, если в памяти уже хранится число, выполните расчет и
     * сохранить результат.
     * Пример для этого случая: 5 + 7 + -> Нам нужно сохранить 12 в памяти, а затем сохранить оператор*/

    if (operatorClicked) {
        storedOperator = button->text().at(0);
    }
    else {
        if (hasStoredNumber) {
            calculate_result();
        }
        else {
            //Устанавливаем флаг, чтобы указать, что теперь у нас есть число, хранящееся в памяти
            hasStoredNumber = true;
            //Получаем строку с дисплея
            QString displayLabel = ui->displayPanel->text();
            //Преобразуем строку в двойную и сохраняем
            storedNumber = displayLabel.toDouble();
        }
        //Установите флаг, что последней нажатой кнопкой был оператор
        operatorClicked = true;
        //Сохранить оператор в памяти
        storedOperator = button->text().at(0);
    }
}

void MainWindow::on_actionDel_clicked()
{
    //Получить строку с дисплея
    QString displayLabel = ui->displayPanel->text();

    //Если строка пуста перезапустим
    if (displayLabel.length() == 0) {
        return;
    }

    //Удалить последнюю цифру из строки
    displayLabel.QString::chop(1);
    //Отобразить получившиюся строку
    ui->displayPanel->setText(displayLabel);
}

void MainWindow::on_actionCalc_clicked()
{
    //Получаем строку с дисплея
    QString displayLabel = ui->displayPanel->text();
    /* Число необходимо сохранить в памяти, чтобы можно было вычислить результат.
     * Кроме того, на дисплее должен отображаться номер как минимум из одной цифры и
     * Последняя нажатая кнопка не должна быть оператором */

    if (!hasStoredNumber || displayLabel.length() < 1 || operatorClicked) {
        return;
    }

    //Рассчитать результат и отобразить на дисплее
    calculate_result();

    //Установите флаг сохраненного номера на false (теперь он у нас на экране)
    hasStoredNumber = false;

}

void MainWindow::on_comma_clicked()
{
    //Получаем строку с дисплея
    QString displayLabel = ui->displayPanel->text();

    /* Добавляйте цифру, только если мы не превышаем лимит цифр.
     * В частности, в этом случае нам нужны 2 цифры.
     * Один для запятой и как минимум еще один для оставшейся цифры.
     * Также проверьте, есть ли уже другая запятая. */
    if (displayLabel.length() >= (DIGIT_LIMIT - 1) ||
        displayLabel.contains('.', Qt::CaseSensitive)) {
        return;
    }

    //Если метка пуста, добавьте ноль, а затем запятую
    if (displayLabel.length() == 0) {
        displayLabel = "0";
    }

    //Добавить запятую
    displayLabel.append('.');
    //Вывести полученную строку
    ui->displayPanel->setText(displayLabel);
}

void MainWindow::on_actionClear_clicked()
{
    //Очистить дисплей
    ui->displayPanel->clear();
    //Установить флаг оператора и сохранить номер в false
    operatorClicked = false;
    hasStoredNumber = false;
}

void MainWindow::on_actionPercent_clicked()
{
    //Получить строку с дисплея
    QString displayLabel = ui->displayPanel->text();
    //Преобразовать в двойной
    double percentage = displayLabel.toDouble();
    //Умножаем на на 0,01, чтобы получить процент
    percentage *= 0.01;
    //Так как может произойти переполнение, лучше внимательно преобразовать число
    displayLabel = QString::number(percentage,'g', DIGIT_LIMIT);
    //Вывести полученную строку
    ui->displayPanel->setText(displayLabel);
}

void MainWindow::on_actionSign_clicked()
{
    //Получить строку с дисплея
    QString displayLabel = ui->displayPanel->text();
    //Преобразовать в двойной
    double percentage = displayLabel.toDouble();
    //Просто умножьте на -1, чтобы изменить знак
    percentage *= -1;
    //конвертируйте число
    displayLabel = QString::number(percentage,'g', DIGIT_LIMIT);
    //Вывести полученную строку
    ui->displayPanel->setText(displayLabel);
}

//==================================================================================
//Вспомогательные функции
//==================================================================================
void MainWindow::calculate_result() {
    //Получаем строку с дисплея
    QString displayLabel = ui->displayPanel->text();

    //Если отображаемое число заканчивается запятой, убираем запятую.
     if (displayLabel.endsWith('.',Qt::CaseSensitive)) {
         displayLabel.QString::chop(1);
     }

     //Решаем, что делать в зависимости от операции
     if (storedOperator == '+') {
         storedNumber += displayLabel.toDouble();
     }
     else if (storedOperator == '-') {
         storedNumber -= displayLabel.toDouble();
     }
     else if (storedOperator == 'x') {
         storedNumber *= displayLabel.toDouble();
     }
     else if (storedOperator == '/') {
         storedNumber /= displayLabel.toDouble();
     }

     //Так как может произойти переполнение, лучше внимательно преобразовать число
     displayLabel = QString::number(storedNumber,'g', DIGIT_LIMIT);

     //Выводим полученную строку
     ui->displayPanel->setText(displayLabel);
}

//Кнопки клавиатуры должны вызывать соответствующие функции
void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        //Numbers
        case Qt::Key_1:
            numberGroup_clicked(ui->num1);
            break;
        case Qt::Key_2:
            numberGroup_clicked(ui->num2);
            break;
        case Qt::Key_3:
            numberGroup_clicked(ui->num3);
            break;
        case Qt::Key_4:
            numberGroup_clicked(ui->num4);
            break;
        case Qt::Key_5:
            numberGroup_clicked(ui->num5);
            break;
        case Qt::Key_6:
            numberGroup_clicked(ui->num6);
            break;
        case Qt::Key_7:
            numberGroup_clicked(ui->num7);
            break;
        case Qt::Key_8:
            numberGroup_clicked(ui->num8);
            break;
        case Qt::Key_9:
            numberGroup_clicked(ui->num9);
            break;
        case Qt::Key_0:
            numberGroup_clicked(ui->num0);
            break;
        //Operators
        case Qt::Key_Plus:
            actionGroup_clicked(ui->actionPlus);
            break;
        case Qt::Key_Minus:
            actionGroup_clicked(ui->actionMinus);
            break;
        case Qt::Key_Asterisk:
            actionGroup_clicked(ui->actionMul);
            break;
        case Qt::Key_Slash:
            actionGroup_clicked(ui->actionDiv);
            break;
        //Comma
        case Qt::Key_Period:
            on_comma_clicked();
            break;
        //Return (enter)
        case Qt::Key_Enter:
        case Qt::Key_Return:
            on_actionCalc_clicked();
            break;
        //Backspace and delete
        case Qt::Key_Backspace:
            on_actionDel_clicked();
            break;
        case Qt::Key_Delete:
            on_actionClear_clicked();
            break;
        //Percentage
        case Qt::Key_Percent:
            on_actionPercent_clicked();
            break;
    }
}

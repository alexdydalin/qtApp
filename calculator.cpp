#include "calculator.h"

#include <QGridLayout>
#include <QToolButton>

#include <QtMath>

#include <QDebug>

Calculator::Calculator()
{
    m_display_up   = new QLineEdit();
    m_display_down = new QLineEdit();
    m_sign         = new QLabel();

    QGridLayout* mainLayout = new QGridLayout;

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    m_display_down->setText("0");

    m_sign->setAlignment(Qt::AlignRight);

    /// неизменяемое поле
    m_display_up->setReadOnly(true);
    m_display_down->setReadOnly(true);

    /// выравнивание по правой части поля
    m_display_up->setAlignment(Qt::AlignRight);
    m_display_down->setAlignment(Qt::AlignRight);

    /// ограниение ввода по длинне
    m_display_up->setMaxLength(15);
    m_display_down->setMaxLength(15);

    /// измениение размера шрифта
    QFont font = m_display_up->font();
    font.setPointSize(font.pointSize() + 8);
    m_display_up->setFont(font);
    m_display_down->setFont(font);
    m_sign->setFont(font);


    // создание кнопок цифр
    /// в slot пишутся slots из файла calculator.h
    for (int i = 0; i < 10; ++i)
        m_digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));

    // создание остальных кнопок
    Button *pointButton       = createButton(".",          SLOT(pointClicked()));
    Button *changeSignButton  = createButton(m_change_sign, SLOT(changeSignClicked()));

    Button *backspaceButton   = createButton("Удалить символ", SLOT(backspaceClicked()));
    Button *clearButton       = createButton("Отчистить строку",     SLOT(clear()));
    Button *clearAllButton    = createButton("Отчистить всё", SLOT(clearAll()));

    Button *clearMemoryButton = createButton("MC", SLOT(clearMemory()));
    Button *readMemoryButton  = createButton("MR", SLOT(readMemory()));
    Button *setMemoryButton   = createButton("M+", SLOT(addToMemory()));
    Button *addToMemoryButton = createButton("M-", SLOT(minToMemory()));

    Button *divisionButton    = createButton(m_division_sign,   SLOT(doubleOperandClicked()));
    Button *timesButton       = createButton(m_times_sign,      SLOT(doubleOperandClicked()));
    Button *minusButton       = createButton(m_minus_sign,      SLOT(doubleOperandClicked()));
    Button *plusButton        = createButton(m_plus_sign,       SLOT(doubleOperandClicked()));

    Button *squareRootButton  = createButton(m_squareRoot_sign,  SLOT(unaryOperatorClicked()));
    Button *powerButton       = createButton(m_power_sign,       SLOT(unaryOperatorClicked()));
    Button *reciprocalButton  = createButton(m_reciprocal_sign,  SLOT(unaryOperatorClicked()));

    Button *equalButton       = createButton("=",                SLOT(equalClicked()));


    // параметры добавления(строки, колонки, длинна элемента по строке, длинна элемента по колонке)
    mainLayout->addWidget(m_display_up,   0, 0, 1, 6);
    mainLayout->addWidget(m_sign,     1, 5, 1, 1);
    mainLayout->addWidget(m_display_down,  2, 0, 1, 6);
    mainLayout->addWidget(backspaceButton, 3, 0, 1, 2);
    mainLayout->addWidget(clearButton,     3, 2, 1, 2);
    mainLayout->addWidget(clearAllButton,  3, 4, 1, 2);

    mainLayout->addWidget(clearMemoryButton, 4, 0);
    mainLayout->addWidget(readMemoryButton,  5, 0);
    mainLayout->addWidget(setMemoryButton,   6, 0);
    mainLayout->addWidget(addToMemoryButton, 7, 0);

    for (int i = 1; i < 10; ++i) {
        int row = ((9 - i) / 3) + 4; /// место в строке
        int column = ((i - 1) % 3) + 1; /// место в колонке
        mainLayout->addWidget(m_digitButtons[i], row, column);
    }

    mainLayout->addWidget(m_digitButtons[0],  7, 1);
    mainLayout->addWidget(pointButton,      7, 2);
    mainLayout->addWidget(changeSignButton, 7, 3);

    mainLayout->addWidget(divisionButton, 4, 4);
    mainLayout->addWidget(timesButton,    5, 4);
    mainLayout->addWidget(minusButton,    6, 4);
    mainLayout->addWidget(plusButton,     7, 4);

    mainLayout->addWidget(squareRootButton, 4, 5);
    mainLayout->addWidget(powerButton,      5, 5);
    mainLayout->addWidget(reciprocalButton, 6, 5);
    mainLayout->addWidget(equalButton,      7, 5);

    setLayout(mainLayout);

    setWindowTitle("Калькулятор");
}

/// Код ниже создаёт функцию которая принимает два аргумента:
/// text (строка с текстом кнопки) и member (slot).
/// Затем он создаёт новую кнопку btn с текстом, сигнал clicked к слотом member
Button* Calculator::createButton(const QString& text, const char* member){
    Button* btn = new Button(text);
    connect(btn, SIGNAL(clicked()), this, member);
    return btn;
}


// при нажатии на цифры
void Calculator::digitClicked() {
    Button* btn = (Button*) sender(); // показывает имя отправителя (имя нажатой кнопки, т е цифру)
    int digit = btn->text().toUInt(); //чтение кнопки из текста и преобразование в число

    // при пустом поле висит 0, при нажатии поле отчищается и отображается введенное значение
    if (m_display_down->text() == "0") {
        m_display_down->clear();
        m_display_up->clear();
    }

    // добавление цифры к текущему тексту на дисплее если он не равен нулю
    m_display_down->setText(m_display_down->text() + QString::number(digit));
}


// обработка нажатых унарных операторов
void Calculator::unaryOperatorClicked() {
    Button* btn = (Button*) sender();
    QString operation = btn->text();

    double operand = m_display_down->text().toDouble();
    double result = 0.0;

    if (operation == m_squareRoot_sign) { // получение корня из числа
        if (operand < 0.0) {
            abortOperation();
            return;
        }

        result = std::sqrt(operand);
    } else if (operation == m_power_sign) { // возведение в квадрат
        result = std::pow(operand, 2.0);
    } else if (operation == m_reciprocal_sign) { // обратное число
        if (operand == 0.0) {
            abortOperation();
            return;
        }

        result = 1.0 / operand;
    }

    m_display_down->setText(QString::number(result));
}

void Calculator::doubleOperandClicked() {
    Button* btn = (Button*) sender();
    QString operation = btn->text();

    double operand = m_display_down->text().toDouble();

    if (m_display_down->text() == "0")
        return;

    // отображение знака текущей операции
    m_sign->setText(operation);

    if (m_display_down->text() == "")
        return;

    m_display_down->clear();


    if (!m_pending_operation.isEmpty()) {
        if(!calculate(operand, m_pending_operation)) {
            abortOperation();
            return;
        }
        m_pending_operation.clear();
    } else {
        m_display_up->setText(QString::number(operand));
    }

    m_pending_operation = operation;

}


// переносит результат операции в нижнее поле
void Calculator::equalClicked() {
    double operand = m_display_down->text().toDouble();

    if (!m_pending_operation.isEmpty()) {
        if(!calculate(operand, m_pending_operation)) {
            abortOperation();
            return;
        }
        m_pending_operation.clear();
    }

    m_display_down->setText(m_display_up->text());
    m_display_up->clear();
    m_sign->clear();
}


// функция для добавления точки к числу
void Calculator::pointClicked() {
    if (!m_display_down->text().contains('.')){
        m_display_down->setText(m_display_down->text() + '.');
    }
}


// функция измениение знака перед числом
void Calculator::changeSignClicked() {
    QString text = m_display_down->text();
    double val = text.toDouble();

    if (val > 0.0) {
        text.prepend("-");
    } else if (val < 0.0) {
        text.remove(0, 1);
    }

    m_display_down->setText(text);
}


// удаление символа
void Calculator::backspaceClicked() {
    QString text = m_display_down->text();
    text.chop(1); // отрезает последний символ из текста

    if (text.isEmpty()) { // если срока пустая то ее начение становится 0
        text = "0";
    }

    m_display_down->setText(text);
}


// отчищение последнего введенного операнда (всего нижнего поля)
void Calculator::clear() {
    m_display_down->setText("0");
}


// отчищение всех ранее введенных опернандов (верхнего и нижнего поля)
void Calculator::clearAll() {
    m_display_down->setText("0");
    m_display_up->clear();
}


// отчищение ранее сохраненной суммы
void Calculator::clearMemory() {
    m_sum_in_memory = 0.0;
}

// просмотр сохраненной суммы
void Calculator::readMemory() {
    m_display_up->clear();
    m_sign->clear();
    m_display_down->setText(QString::number(m_sum_in_memory));
}

// вычитание из сохраненной суммы
void Calculator::minToMemory() {
    equalClicked();
    m_sum_in_memory -= m_display_down->text().toDouble();
}

// прибавление к сохраненной сумме
void Calculator::addToMemory() {
    equalClicked();
    m_sum_in_memory += m_display_down->text().toDouble();
}


// запускается при запрещенных действиях (взять корень из отрицательного числа и тд)
void Calculator::abortOperation() {
    m_display_up->setText("###");
    m_display_down->setText("###");
}



// выполнение ранее сделаных математических операций при добавлении новой без кнопки равно
bool Calculator::calculate(double operand, const QString &operation) {

    double temp_total = m_display_up->text().toDouble();

    if (operation == m_plus_sign) {
        temp_total += operand;
    } else if (operation == m_minus_sign) {
        temp_total -= operand;
    } else if (operation == m_times_sign) {
        temp_total *= operand;
    } else if (operation == m_division_sign) {
        if (operand == 0.0)
            return false;
        temp_total /= operand;
    }

    m_display_up->setText(QString::number(temp_total));
    return true;
}

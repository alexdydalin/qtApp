#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>

#include <mybutton.h>

class Calculator : public QWidget {
    Q_OBJECT

public:
    Calculator();

// слоты для обработки сигналов внутри класса
private slots:
    void digitClicked();
    void unaryOperatorClicked();
    void doubleOperandClicked();
    void equalClicked();
    void pointClicked();
    void changeSignClicked();
    void backspaceClicked();
    void clear();
    void clearAll();
    void clearMemory();
    void readMemory();
    void addToMemory();
    void minToMemory();
private:
    Button *createButton(const QString& text, const char* member);


    void abortOperation();
    double m_sum_in_memory;

    QString m_pending_operation;

    QLineEdit* m_display_up;
    QLineEdit* m_display_down;
    QLabel*    m_sign;

    Button* m_digitButtons[10];

    bool calculate(double operand, const QString& operation);

    // отображение значение кнопок через Unicode
    QString m_squareRoot_sign = QChar(0x0000221A); // квадратный корень
    QString m_power_sign      = 'x' + QChar(0x000000b2); // х в квадрате
    QString m_reciprocal_sign = "1/x";

    QString m_division_sign    = QChar(0x000000F7); // символ деления
    QString m_times_sign       = QChar(0x000000D7); // символ умножения
    QString m_plus_sign        = "+";
    QString m_minus_sign       = "-";

    QString m_change_sign       = QChar(0x000000b1); // символ +/-
};

#endif

#ifndef BUTTON_H
#define BUTTON_H

#include <QToolButton>

/// создание собственного класса для кнопок с переопределением метода
/// чтобы кнопка создавалась вместе с текстом

class Button : public QToolButton
{
    Q_OBJECT

public:
    explicit Button(const QString &text, QWidget *parent = nullptr);

    QSize sizeHint() const override;
};

#endif

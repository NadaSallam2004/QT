#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayText READ displayText NOTIFY displayTextChanged)

public:
    explicit Backend(QObject *parent = nullptr);

    QString displayText() const;

    Q_INVOKABLE void inputDigit(const QString &digit);
    Q_INVOKABLE void inputOperator(const QString &op);
    Q_INVOKABLE void calculateResult();
    Q_INVOKABLE void clearAll();
    Q_INVOKABLE void backspace();

signals:
    void displayTextChanged();

private:
    QString formatNumber(double value) const;
    double applyOperator(double a, const QString &op, double b, bool &ok) const;
    void setDisplayText(const QString &text);
    void refreshExpressionDisplay();

    double  m_storedValue      = 0.0;
    QString m_currentInput;
    QString m_pendingOperator;
    QString m_displayText      = "0";
    bool    m_awaitingNewInput = true;
    bool    m_justCalculated   = false;
};

#endif

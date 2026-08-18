#include "backend.h"
#include <QtGlobal>
#include <cmath>

Backend::Backend(QObject *parent)
    : QObject(parent)
{
}

QString Backend::displayText() const
{
    return m_displayText;
}

void Backend::setDisplayText(const QString &text)
{
    if (m_displayText == text)
        return;
    m_displayText = text;
    emit displayTextChanged();
}

QString Backend::formatNumber(double value) const
{
    if (qFuzzyCompare(value, std::round(value)) && std::abs(value) < 1e15) {
        return QString::number(static_cast<qint64>(std::round(value)));
    }
    return QString::number(value, 'g', 12);
}

double Backend::applyOperator(double a, const QString &op, double b, bool &ok) const
{
    ok = true;
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b == 0.0) { ok = false; return 0.0; }
        return a / b;
    }
    if (op == "%") {
        if (b == 0.0) { ok = false; return 0.0; }
        return std::fmod(a, b);
    }
    ok = false;
    return b;
}

void Backend::refreshExpressionDisplay()
{
    QString left = m_pendingOperator.isEmpty()
                       ? QString()
                       : formatNumber(m_storedValue) + m_pendingOperator;
    QString shown = left + m_currentInput;
    setDisplayText(shown.isEmpty() ? "0" : shown);
}

void Backend::inputDigit(const QString &digit)
{
    if (m_justCalculated) {
        m_storedValue = 0.0;
        m_pendingOperator.clear();
        m_currentInput.clear();
        m_justCalculated = false;
    }

    if (digit == ".") {
        if (m_currentInput.contains('.'))
            return;
        if (m_currentInput.isEmpty())
            m_currentInput = "0";
        m_currentInput += ".";
    } else {
        if (m_awaitingNewInput)
            m_currentInput.clear();
        if (m_currentInput == "0")
            m_currentInput.clear();
        m_currentInput += digit;
    }

    m_awaitingNewInput = false;
    refreshExpressionDisplay();
}

void Backend::inputOperator(const QString &op)
{
    m_justCalculated = false;

    if (m_currentInput.isEmpty() && m_pendingOperator.isEmpty()) {
        m_storedValue = 0.0;
    } else if (!m_currentInput.isEmpty()) {
        if (m_pendingOperator.isEmpty()) {
            m_storedValue = m_currentInput.toDouble();
        } else {
            bool ok = true;
            m_storedValue = applyOperator(m_storedValue, m_pendingOperator,
                                           m_currentInput.toDouble(), ok);
            if (!ok) {
                setDisplayText("Error");
                clearAll();
                return;
            }
        }
    }

    m_pendingOperator = op;
    m_currentInput.clear();
    m_awaitingNewInput = true;
    refreshExpressionDisplay();
}

void Backend::calculateResult()
{
    if (m_pendingOperator.isEmpty()) {
        if (!m_currentInput.isEmpty()) {
            setDisplayText(m_currentInput + "=" + m_currentInput);
            m_storedValue = m_currentInput.toDouble();
            m_awaitingNewInput = true;
            m_justCalculated = true;
        }
        return;
    }

    double rightOperand = m_currentInput.isEmpty() ? m_storedValue
                                                    : m_currentInput.toDouble();

    QString rightText = m_currentInput.isEmpty() ? formatNumber(m_storedValue) : m_currentInput;
    QString expressionBefore = formatNumber(m_storedValue) + m_pendingOperator + rightText;

    bool ok = true;
    double result = applyOperator(m_storedValue, m_pendingOperator, rightOperand, ok);
    if (!ok) {
        setDisplayText("Error");
        clearAll();
        return;
    }

    setDisplayText(expressionBefore + "=" + formatNumber(result));

    m_storedValue = result;
    m_pendingOperator.clear();
    m_currentInput = formatNumber(result);
    m_awaitingNewInput = true;
    m_justCalculated = true;
}

void Backend::clearAll()
{
    m_storedValue = 0.0;
    m_pendingOperator.clear();
    m_currentInput.clear();
    m_awaitingNewInput = true;
    m_justCalculated = false;
    setDisplayText("0");
}

void Backend::backspace()
{
    if (m_justCalculated) {
        clearAll();
        return;
    }
    if (!m_currentInput.isEmpty()) {
        m_currentInput.chop(1);
        refreshExpressionDisplay();
    }
}

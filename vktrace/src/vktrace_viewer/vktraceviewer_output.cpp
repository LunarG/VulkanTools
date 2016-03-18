#include "vktraceviewer_output.h"
#include <QTextEdit>

vktraceviewer_output gs_OUTPUT;

vktraceviewer_output::vktraceviewer_output()
{
}

vktraceviewer_output::~vktraceviewer_output()
{
}

void vktraceviewer_output::init(QTextBrowser *pTextEdit)
{
    m_pTextEdit = pTextEdit;
}

QString vktraceviewer_output::convertToHtml(QString message)
{
    QString result;
    if (message.endsWith("\n"))
    {
        message.chop(1);
    }
    result = message.replace("\n", "<br>");
    return result;
}

void vktraceviewer_output::moveCursorToEnd()
{
    QTextCursor cursor = m_pTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    m_pTextEdit->setTextCursor(cursor);
}

void vktraceviewer_output::message(uint64_t packetIndex, const QString& message)
{
    if (m_pTextEdit != NULL)
    {
        QString msg;
        if (packetIndex == (uint64_t)-1)
        {
            msg = message;
        }
        else
        {
            msg = QString("(<a href='packet#%1'>%1</a>): %2 ").arg(packetIndex).arg(message);
        }
        moveCursorToEnd();
        m_pTextEdit->append(msg);
    }
}

void vktraceviewer_output::warning(uint64_t packetIndex, const QString& warning)
{
    if (m_pTextEdit != NULL)
    {
        QString msg;
        if (packetIndex == (uint64_t)-1)
        {
            msg = QString("<font color='red'>Warning: %1</font> ").arg(warning);
        }
        else
        {
            msg = QString("<font color='red'>(<a href='packet#%1'>%1</a>) Warning: %2</font> ").arg(packetIndex).arg(warning);
        }
        moveCursorToEnd();
        m_pTextEdit->append(msg);
    }
}

void vktraceviewer_output::error(uint64_t packetIndex, const QString& error)
{
    if (m_pTextEdit != NULL)
    {
        QString msg;
        if (packetIndex == (uint64_t)-1)
        {
            msg = QString("<font color='red'><b>Error: %1</b></font> ").arg(convertToHtml(error));
        }
        else
        {
            msg = QString("<font color='red'><b>(<a href='packet#%1'>%1</a>) Error: %2</b></font> ").arg(packetIndex).arg(convertToHtml(error));
        }
        moveCursorToEnd();
        m_pTextEdit->append(msg);
    }
}

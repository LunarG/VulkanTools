#ifndef VKTRACEVIEWER_OUTPUT_H
#define VKTRACEVIEWER_OUTPUT_H

#include <QString>
#include <QTextBrowser>
extern "C"
{
#include "vktrace_platform.h"
#include "vktrace_tracelog.h"
}

class QTextEdit;

class vktraceviewer_output
{
public:
    vktraceviewer_output();
    ~vktraceviewer_output();

    void init(QTextBrowser* pTextEdit);

    void message(uint64_t packetIndex, const QString& message);
    void warning(uint64_t packetIndex, const QString& warning);
    void error(uint64_t packetIndex, const QString& error);

private:
    QString convertToHtml(QString message);
    void moveCursorToEnd();
    QTextBrowser* m_pTextEdit;
};

extern vktraceviewer_output gs_OUTPUT;

inline void vktraceviewer_output_init(QTextBrowser* pTextEdit) { gs_OUTPUT.init(pTextEdit); }

inline void vktraceviewer_output_message(uint64_t packetIndex, const QString& message) { gs_OUTPUT.message(packetIndex, message); }
inline void vktraceviewer_output_message(const QString& message) { gs_OUTPUT.message(-1, message); }

inline void vktraceviewer_output_warning(uint64_t packetIndex, const QString& warning) { gs_OUTPUT.warning(packetIndex, warning); }
inline void vktraceviewer_output_warning(const QString& warning) { gs_OUTPUT.warning(-1, warning); }

inline void vktraceviewer_output_error(uint64_t packetIndex, const QString& error) { gs_OUTPUT.error(packetIndex, error); }
inline void vktraceviewer_output_error(const QString& error) { gs_OUTPUT.error(-1, error); }
inline void vktraceviewer_output_deinit() { gs_OUTPUT.init(0); }

#endif // VKTRACEVIEWER_OUTPUT_H

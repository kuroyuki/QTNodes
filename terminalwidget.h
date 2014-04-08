#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include <QDockWidget>
#include "QtWidgets"

class TerminalWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit TerminalWidget(QDockWidget *parent = 0);

signals:

public slots:
    void AddText(QString text);
    void ParseJson(QJsonObject text);

private:
    QTextEdit* TextOutput;

};

#endif // TERMINALWIDGET_H

#include "terminalwidget.h"
#include <QTextCursor>

TerminalWidget::TerminalWidget(QDockWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle("Terminal");
    TextOutput = new QTextEdit();

    QPalette palette;
    palette.setColor(QPalette::Base, QColor(0, 0, 0, 255));
    TextOutput->setPalette(palette);

    TextOutput->setTextColor(QColor(50, 200, 50, 255));

    TextOutput->setFont(QFont("Courier", 10));

    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setWidget(TextOutput);
}

void TerminalWidget::AddText(QString text){
    TextOutput->insertPlainText(text+'\n');
    TextOutput->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}
void TerminalWidget::ParseEvent(QString event){
    QStringList list = event.split(",");
    switch (list.at(0).toInt()){
        case 1 :
            AddText("create node "+list.at(1)+","+list.at(2));
            break;
        case 2 :
            AddText("create sensor "+list.at(1)+","+list.at(2));
            break;
        case 3 :
            AddText("create act "+list.at(1)+","+list.at(2));
            break;
        case 4 :
            AddText("bind nodes "+list.at(1)+","+list.at(2)+" and "+list.at(3)+","+list.at(4));
            break;
        default :
            AddText(event);
            break;
    }
}

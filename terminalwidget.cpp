#include "terminalwidget.h"
#include <QTextCursor>

TerminalWidget::TerminalWidget(QDockWidget *parent) :
    QDockWidget(parent)
{    
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
    case list[0] :
        "1" ->
            AddText("create node "+list[1]+","+list[2]);
            break;
        "2" ->
            AddText("create sensor "+list[1]+","+list[2]);
            break;
        "3" ->
            AddText("create act "+list[1]+","+list[2]);
            break;
        "4" ->
            AddText("bind nodes "+list[1]+","+list[2]+" and "+list[3]+","+list[4]);
            break;
    default :
        AddText("unknown message");
        break;
}

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
void TerminalWidget::ParseJson(QJsonObject json){
   QStringList list = json.keys();
   for (int i =0; i<list.length(); i++)
       AddText(list[i]);
}

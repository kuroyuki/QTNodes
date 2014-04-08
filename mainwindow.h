#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "terminalwidget.h"
#include "dojonetwork.h"
#include "clientarea.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void ConsoleOutput(QString text);
private slots:
    void about();

private:
    TerminalWidget *terminalWidget;

    dojoNetwork* Dojo;

    void createActions();
    void createMenus();

    ClientArea *clientArea;

    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *terminalAct;
    QAction *exitAct;

    QAction *aboutAct;
    QAction *aboutQtAct;

};

#endif // MAINWINDOW_H

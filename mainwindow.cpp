#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtWidgets"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    terminalWidget = new TerminalWidget();
    addDockWidget(Qt::RightDockWidgetArea, terminalWidget);
    ConsoleOutput("terminalWidget created...");    

    Dojo = new dojoNetwork();
    connect(Dojo, SIGNAL(dojoEvent(QJsonObject)), terminalWidget, SLOT(ParseJson(QJsonObject)));
    ConsoleOutput("dojoNetwork created...");

    clientArea = new ClientArea(this, Dojo);
    setCentralWidget(clientArea);
    ConsoleOutput("client area created...");
    connect(Dojo, SIGNAL(dojoEvent(QString)), clientArea, SLOT(ClientUpdate(QJsonObject)));

    clientArea->InitializeNetwork();

    createActions();
    createMenus();

    setWindowTitle(tr("dojoNodes"));
    resize(700, 500);
}

MainWindow::~MainWindow()
{

}
void MainWindow::ConsoleOutput(QString text){
    terminalWidget->AddText(text);
}
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Scribble"),
            tr("<p> This is Dojo application to create, edit and test artifical neural networks.</p>"));
}

void MainWindow::createActions()
{
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    terminalAct = new QAction(tr("&Terminal"), this);
    connect(terminalAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{

    fileMenu = new QMenu(tr("&File"), this);

    fileMenu->addAction(terminalAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);
}


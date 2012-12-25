#include <QtGui/QApplication>
#include <QtGui/QPushButton>
#include <QtGui/QFont>
#include <qlabel.h>
#include "maze.h"
#include <QGridLayout>
Researcher* InitResearcher (Maze* maze, QWidget* parent, Console* console)
{
    WayLight* journey [30][25];
    for (int cx = 0; cx < 30; cx++)
         for (int cy = 0; cy < 25; cy++)
              journey [cx][cy] = new WayLight (maze -> getPositionCell(cx, cy) -> x + maze -> mapTo (parent, QPoint(0, 0)).x(),
                                               maze -> getPositionCell(cx, cy) -> y + maze -> mapTo (parent, QPoint(0, 0)).y(),
                                               parent);   
    Researcher* researcher = new Researcher  (maze, console, parent);
    
    for (int cx = 0; cx < 30; cx++)
            for (int cy = 0; cy < 25; cy++)
                 researcher -> journey [cx][cy] = journey [cx][cy];
    
    return researcher;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    QWidget window;
    window.setMinimumSize (1300, 750);
    window.setWindowState (Qt::WindowMaximized);
   // window.resize    (1000, 700);
    Console* console = NULL;
    console = new Console (&window);
    console -> setGeometry (10, 520, 600, 170);
    
    console -> printText ("User, welcome to maze...\n"
                          "Print \"script tutorial.txt\" to see how it works...");

    QWidget buttonsLayer;
    
    QPushButton quit ("Quit", &buttonsLayer);
    quit.setFont     (QFont ("Times", 19, QFont::Bold));
    
    QPushButton step ("One step", &buttonsLayer);
    step.setFont     (QFont ("Times", 19, QFont::Bold));
    
    QPushButton autopass ("Autopass", &buttonsLayer);
    autopass.setFont     (QFont ("Times", 19, QFont::Bold));
    
    QPushButton about ("About", &buttonsLayer);
    about.setFont     (QFont ("Times", 19, QFont::Bold));
    
    QPushButton stopPassing ("Stop", &buttonsLayer);
    stopPassing.setFont     (QFont ("Times", 19, QFont::Bold));
    stopPassing.setGeometry (620, 190, 170, 50);    
    
    QPushButton generate ("Generate", &buttonsLayer);
    generate.setFont     (QFont ("Times", 19, QFont::Bold));   
    
    Maze* maze = new Maze (30, 25, &window);
    //maze.loadMaze       ();
    QObject::connect      (&quit, SIGNAL (clicked ()), &app, SLOT (quit ()));
    
    Researcher* researcher = InitResearcher (maze, maze, console);
    
    myTimer* passTimer     = new myTimer     (researcher); 
    
    Scheme researchScheme (&window);
    
    QGridLayout* lay    = new QGridLayout;
    
    
    lay -> addWidget (&buttonsLayer, 9, 0, 1, 17);
    QGridLayout* layBut = new QGridLayout (&buttonsLayer);
    
    lay -> addWidget (maze, 0, 0, 5, 6);
    layBut -> addWidget (maze, 0, 0, 5, 6);
    
    lay -> setColumnStretch (7, 1);
    lay -> setColumnStretch (8, 1);
    lay -> setColumnStretch (9, 1);
    
    
    layBut -> addWidget (&generate, 0, 0, 1, 4);
    layBut -> addWidget (&step,     0, 4, 1, 4);
    
    layBut -> addWidget (&autopass, 0, 8, 1, 4);
    
    layBut -> addWidget (&stopPassing, 0, 12, 1, 4);
    layBut -> addWidget (&about, 0, 16, 1, 4);
    lay -> addWidget (console, 5, 0, 4, 6);
    
    layBut -> addWidget (&quit, 0, 20, 1, 4);
    
    lay -> addWidget (&researchScheme, 0, 7, 9, 10);
    //lay -> setColumnMinimumWidth (60);
   
    window.setLayout (lay);
    buttonsLayer.setLayout (layBut);
    
    researcher -> generateMaze ();
    myTimer* scriptTimer = new myTimer (console);
    
    QObject::connect (&step,        SIGNAL (clicked ()),               researcher,       SLOT (processStep   ()));
    
    QObject::connect (passTimer,    SIGNAL (timeout ()),               researcher,       SLOT (processStep   ()));
    QObject::connect (scriptTimer,  SIGNAL (timeout ()),               console,          SLOT (processScript ()));
    
    QObject::connect (researcher,   SIGNAL (researcherFoundFinish ()), passTimer,        SLOT (stop ()));  
    QObject::connect (researcher,   SIGNAL (stateChanged(int)),        &researchScheme,  SLOT (researcherStateChanged(int)));  
    
    QObject::connect (&stopPassing, SIGNAL (clicked ()),               passTimer,        SLOT (stop ())); 
    
    QObject::connect (&autopass,    SIGNAL (clicked ()),               passTimer,        SLOT (startMyTimer ())); 
    QObject::connect (&generate,    SIGNAL (clicked ()),               researcher,       SLOT (generateMaze ()));
    
    QObject::connect (&about,       SIGNAL (clicked ()),               console,          SLOT (insertAbout  ()));
    
    QObject::connect (console,      SIGNAL (step ()),                  researcher,       SLOT (processStep   ()));
    QObject::connect (console,      SIGNAL (autopass ()),              passTimer,        SLOT (startMyTimer  ()));
    QObject::connect (console,      SIGNAL (stopAutopassing ()),       passTimer,        SLOT (stop          ()));
    QObject::connect (console,      SIGNAL (generate ()),              researcher,       SLOT (generateMaze  ()));
    QObject::connect (console,      SIGNAL (returnToStart()),          researcher,       SLOT (returnToStart ()));
    QObject::connect (console,      SIGNAL (fullDump()),               researcher,       SLOT (notSimpleDump ()));
    QObject::connect (console,      SIGNAL (statesDump()),             researcher,       SLOT (simpleDump    ()));
    QObject::connect (console,      SIGNAL (script()),                 scriptTimer,      SLOT (startMyTimer  ()));
    
    window.show();
    
    return app.exec();
}

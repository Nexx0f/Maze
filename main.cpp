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
    window.resize    (900, 700);
    Console* console = NULL;
    console = new Console (&window);
    console -> setGeometry (10, 520, 600, 170);
    
    console -> printText ("User, welcome to maze...\n"
                          "Press \"generate button\"...");

    QPushButton quit ("Quit", &window);
    quit.setFont     (QFont ("Times", 30, QFont::Bold));
    quit.setGeometry (620, 540, 170, 50);
    
    QPushButton step ("One step", &window);
    step.setFont     (QFont ("Times", 19, QFont::Bold));
    step.setGeometry (620, 70, 170, 50);  
    
    QPushButton autopass ("Autopass", &window);
    autopass.setFont     (QFont ("Times", 19, QFont::Bold));
    autopass.setGeometry (620, 130, 170, 50);
   
    QPushButton stopPassing ("Stop passing", &window);
    stopPassing.setFont     (QFont ("Times", 19, QFont::Bold));
    stopPassing.setGeometry (620, 190, 170, 50);    
    
    QPushButton generate ("Generate", &window);
    generate.setFont     (QFont ("Times", 19, QFont::Bold));
   // generate.setGeometry (620, 10, 170, 50);    
    
    Maze* maze = new Maze (30, 25, &window);
    //maze.loadMaze       ();
    QObject::connect      (&quit, SIGNAL (clicked ()), &app, SLOT (quit ()));
    
    Researcher* researcher = InitResearcher (maze, &window, console);
    
    myTimer* passTimer     = new myTimer     (researcher); 
    
    Scheme researchScheme (&window);
    
    QGridLayout* lay = new QGridLayout;
    
    lay -> addWidget (maze, 0, 0, 5, 6);
    
    lay -> addWidget (&generate, 0, 7);
    lay -> addWidget (&step,     1, 7);
    
    
    
    lay -> addWidget (&autopass, 0, 8);
    lay -> addWidget (&stopPassing, 1, 8);
    lay -> addWidget (console, 6, 0);
    
    lay -> addWidget (&quit, 2, 7);
    
    lay -> addWidget (&researchScheme, 3, 7, 4, 3);
    //lay -> setColumnMinimumWidth (60);
   
    window.setLayout (lay);
    
    researcher -> generateMaze ();
    
    QObject::connect (&step,        SIGNAL (clicked ()),               researcher, SLOT (processStep  ()));
    QObject::connect (passTimer,    SIGNAL (timeout ()),               researcher, SLOT (processStep  ()));
    
    QObject::connect (researcher,   SIGNAL (researcherFoundFinish ()), passTimer,  SLOT (stop ()));  
    QObject::connect (&stopPassing, SIGNAL (clicked ()),               passTimer,  SLOT (stop ())); 
    
    QObject::connect (&autopass,    SIGNAL (clicked ()),               passTimer,  SLOT (startMyTimer ())); 
    QObject::connect (&generate,    SIGNAL (clicked ()),               researcher, SLOT (generateMaze ()));
    
    QObject::connect (console,      SIGNAL (step ()),                  researcher, SLOT (processStep   ()));
    QObject::connect (console,      SIGNAL (autopass ()),              passTimer,  SLOT (startMyTimer  ()));
    QObject::connect (console,      SIGNAL (stopAutopassing ()),       passTimer,  SLOT (stop          ()));
    QObject::connect (console,      SIGNAL (generate ()),              researcher, SLOT (generateMaze  ()));
    QObject::connect (console,      SIGNAL (returnToStart()),          researcher, SLOT (returnToStart ()));
    QObject::connect (console,      SIGNAL (fullDump()),               researcher, SLOT (notSimpleDump ()));
    QObject::connect (console,      SIGNAL (statesDump()),             researcher, SLOT (simpleDump    ()));
    
    window.show();
    
    return app.exec();
}

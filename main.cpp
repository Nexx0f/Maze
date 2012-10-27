#include <QtGui/QApplication>
#include <QtGui/QPushButton>
#include <QtGui/QFont>
#include "maze.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    QWidget window;
    window.resize    (800, 600);
    Console* console = NULL;
    console = new Console (&window);
    console -> setGeometry (10, 520, 600, 70);
    
    console -> printText ("User, welcome to maze...\n");

    QPushButton quit ("Quit", &window);
    quit.setFont     (QFont ("Times", 30, QFont::Bold));
    quit.setGeometry (620, 540, 170, 50);
    
    QPushButton step ("One step", &window);
    step.setFont     (QFont ("Times", 20, QFont::Bold));
    step.setGeometry (620, 40, 170, 50);
    
    QPushButton autopass ("Autopass", &window);
    autopass.setFont     (QFont ("Times", 20, QFont::Bold));
    autopass.setGeometry (620, 100, 170, 50);
    
    QPushButton generate ("Generate", &window);
    generate.setFont     (QFont ("Times", 20, QFont::Bold));
    generate.setGeometry (620, 160, 170, 50);    
    
    Maze* maze = new Maze (30, 25, &window);
    //maze.loadMaze       ();
    QObject::connect      (&quit, SIGNAL (clicked ()), &app, SLOT (quit ()));
    
    
    Researcher* researcher = new Researcher  (maze, console, &window);
    myTimer* passTimer     = new myTimer     (researcher); 
    
    QObject::connect (&step,      SIGNAL (clicked ()),               researcher, SLOT (processStep  ()));
    QObject::connect (passTimer,  SIGNAL (timeout ()),               researcher, SLOT (processStep  ()));
    QObject::connect (researcher, SIGNAL (researcherFoundFinish ()), passTimer,  SLOT (stop ()));  
    QObject::connect (&autopass,  SIGNAL (clicked ()),               passTimer,  SLOT (startMyTimer ())); 
    QObject::connect (&generate,  SIGNAL (clicked ()),               researcher, SLOT (generateMaze ())); 
    
    window.show();
    
    return app.exec();
}

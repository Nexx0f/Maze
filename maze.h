#ifndef maze_H
#define maze_H

#include <QtGui/QWidget>
#include <iostream>
#include <QTimer>
#include <QPlainTextEdit>

const int CellWidth  = 20;
const int CellHeight = 20;

namespace CellStates
{
    const int empty  = 0;
    const int wall   = 1;
    const int start  = 2;
    const int finish = 3;    
}

namespace ResearcherStates
{
    const int searchingWay = 0;
    const int finishFound  = 1;
    const int initializing = 2;
    const int waiting      = 3;
}

struct Vector
{
    int x;
    int y;
};

class Console : public QPlainTextEdit
{
    Q_OBJECT
    public:
      void printText (char* text, ...);
    
    public:
      Console (QWidget* parent);
           
};

class MazeCell : public QWidget
{
    Q_OBJECT
    public:
      int          x; 
      int          y;
      int          state;
     
                   MazeCell   (int newX,  int newY, 
			       int width, int height,
			       QWidget* parent);
      virtual      ~MazeCell  ();
   
      virtual void paintEvent (QPaintEvent*);
  
};

class Maze : public QWidget
{
    Q_OBJECT
    private:  
      int          cellDataX;
      int          cellDataY;
      MazeCell*    cellData [30][25];
    public:  
                   Maze            (int xMax, int yMax, QWidget* parent);
      virtual      ~Maze           ();
    
      virtual void paintEvent      (QPaintEvent*);
      QPoint       getStartPoint   ();
      MazeCell*    getLeftCell     (int x, int y, Vector vector);
      MazeCell*    getRightCell    (int x, int y, Vector vector);
      MazeCell*    getForwardCell  (int x, int y, Vector vector);
      MazeCell*    getPositionCell (int x, int y);
      void         generate        ();
      void         generateCell    (int x, int y);
      
    public slots:
      void         loadMaze       ();
};

class Researcher : public QWidget
{
    Q_OBJECT
    public:
      int          x;
      int          y;
      Maze*        maze;
      int          state;
      Vector       vector;   
      Console*     console;
      
                   Researcher     (Maze* newMaze, Console* newConsole, QWidget* parent);
      virtual      ~Researcher    ();
      
      void         updatePosition (int newX, int newY);
      
      virtual void paintEvent     (QPaintEvent*);
      
    public slots:
      void         processStep    ();  
      void         generateMaze   ();
      
    signals:
      void         researcherFoundFinish ();
    
};

class myTimer : public QTimer
{
    Q_OBJECT
    public:
      myTimer (QWidget* parent): QTimer (parent) {} 
      
    public slots:
      void startMyTimer ()
      {
          start (100);
      }
};

#endif // maze_H

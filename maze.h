#ifndef maze_H
#define maze_H

#include <QtGui/QWidget>
#include <iostream>
#include <QTimer>
#include <QPlainTextEdit>
#include <QPalette>
#include <QStringList>

const int CellWidth  = 20;
const int CellHeight = 20;

const int MaxString = 256;

namespace CellStates
{
    const int empty  = 0;
    const int wall   = 1;
    const int start  = 2;
    const int finish = 3;    
}

namespace ResearcherStates
{
    const int initialising = 0;
    const int findLeft     = 1;
    const int findForward  = 2;
    const int findRight    = 3;
    const int findBackward = 4;
    const int step         = 5;
    const int finishFound  = 6;
}

namespace Ways
{
    const int leftUp     = 0;
    const int leftRight  = 1;
    const int leftDown   = 2;
    const int leftLeft   = 3;
    const int upRight    = 4;
    const int upDown     = 5;
    const int upLeft     = 6;
    const int upUp       = 7;
    const int rightDown  = 8;
    const int rightLeft  = 9;
    const int rightUp    = 10;
    const int rightRight = 11;
    const int downLeft   = 12;
    const int downUp     = 13;
    const int downRight  = 14;
    const int downDown   = 15;
}

struct Vector
{
    int x;
    int y;
    
    Vector (int nx, int ny): x (nx), y (ny) {}
    Vector () {}
};

class Console : public QPlainTextEdit
{
    Q_OBJECT
    public:
      void printText             (char* text, ...);
      void keyPressEvent         (QKeyEvent *);
      void mousePressEvent       (QMouseEvent *);
      void mouseDoubleClickEvent (QMouseEvent *);
      void contextMenuEvent      (QContextMenuEvent *);
      void readOnly              (bool read);
      void onEnter               ();
      QStringList* history;
      int historyPos;
      
      int ticksBefore;
      QString commandsToDo;
      int  currentCmd;
      
      void addHistory            (QString cmd);
      void backHistory           ();
      void forwardHistory        ();
      
      QString text;
      QString prompt;
      QPalette palette;
      QTextCharFormat format;
      
      bool readMode;
    
    public:
      Console (QWidget* parent);
    
    public slots:
      void insertAbout     (bool setColor = true);
      void processScript   ();
      
    signals:
      void step            ();
      void autopass        ();
      void stopAutopassing ();
      void generate        ();
      void returnToStart   ();
      void fullDump        ();
      void statesDump      ();
      void script          ();   
      void scriptDone      ();
};

class WayLight : public QWidget
{
    Q_OBJECT
    public:
      bool         wayData [16];
      
      int          x;
      int          y;
      
                   WayLight     (int newX,  int newY,
                                 QWidget* parent);
      
      virtual void paintEvent   (QPaintEvent*);
      void         clearWayCell ();
};

class MazeCell : public QWidget
{
    Q_OBJECT
    public:
      int          x; 
      int          y;
      int          state;
     
                   MazeCell   (int newX,  int newY,
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
};

class Scheme : public QWidget
{
    Q_OBJECT
    public:
      QPixmap*     schemeImage; 
        
                   Scheme      (QWidget* parent);
      
      virtual void paintEvent  (QPaintEvent*);
      
    public slots:
      void researcherStateChanged (int newState);
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
      Vector       oldVector;
      Console*     console;
      WayLight*    journey [30][25];
      bool         detailedDump;
      QWidget*     myParent;
      
      bool         FindLeft       ();
      bool         FindForward    ();
      bool         FindRight      ();
      bool         FindBackward   ();
      bool         Step           ();
      
      void         changeState    (const int newState);
      
                   Researcher     (Maze* newMaze, Console* newConsole, QWidget* parent);
      virtual      ~Researcher    ();
      
      void         updatePosition (int newX, int newY);
      
      virtual void paintEvent     (QPaintEvent*);
      void         clearJourney   ();
      
    public slots:
      void         processStep   ();  
      void         generateMaze  ();
      void         returnToStart ();
      void         simpleDump    ();
      void         notSimpleDump ();
      
    signals:
      void         researcherFoundFinish ();
      void         stateChanged (int newState);
    
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

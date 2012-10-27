#include "maze.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QPainter>
#include <stdio.h>
#include <assert.h>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <math.h>

const int mazeHard = 3;

const int maxPrintTextSize = 256;

//Maze functions--------------------
Maze::Maze(int xMax, int yMax, QWidget* parent): 
    QWidget (parent),
    cellDataX  (xMax),
    cellDataY  (yMax)
{ 
    for (int x = 0; x <= cellDataX - 1; x++)
         for (int y = 0; y <= cellDataY - 1; y++)
	      cellData [x][y] = new MazeCell (x * CellWidth + 1, y * CellHeight + 1, 
				              CellWidth,     CellHeight,
			                      this);
              
    cellData [0][0] -> state                         = CellStates::start;
    cellData [cellDataX - 1][cellDataY - 1] -> state = CellStates::finish;
              
    setGeometry           (9, 9, CellWidth * cellDataX + 1, CellWidth * cellDataY + 1);
    setPalette            (QPalette(QColor(230, 230, 230)));
    setAutoFillBackground (true);
}

void Maze::paintEvent  (QPaintEvent*)
{
    QPainter painter   (this);
    QPen pen           (Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    painter.setPen     (pen);
    painter.setBrush   (Qt::NoBrush);
    painter.translate  (0, 0);
    painter.drawRect   (QRect(0, 0, 600, 500));
}

void Maze::loadMaze ()
{
    QFile f        ("/home/nexx0f/projects/win32/maze/build/maze.txt");
    f.open         (QIODevice::ReadOnly);
    QTextStream in (&f);
    
    for (int y = 0; y < cellDataY; y++)
         for (int x = 0; x < cellDataX; x++)
              in >> cellData [x][y] -> state;
    
    f.close();     
    
}  

QPoint Maze::getStartPoint ()
{   
    for (int x = 0; x < cellDataX; x++)
         for (int y = 0; y < cellDataY; y++)
              if (cellData[x][y] -> state == CellStates::start)
              {
                  QPoint startPoint;
                  startPoint.setX (x);
                  startPoint.setY (y);
                  
                  return startPoint;
              }
    QMessageBox::critical(0, "Maze crashed", "I can't find start on the level");
    assert (0);
}

MazeCell* Maze::getLeftCell (int x, int y, Vector vector)
{
    QTextStream out (stdout);    
    out << "Checking left direction\n";
    out << "x = " << x << ", y = " << y << "\n";
    out << "Vector.x = " << vector.x << "vector.y = " << vector.y << "\n\n";
    
    if (vector.x == 0 && vector.y == -1)  //up vector
    {
        return cellData [x - 1][y];
    }
    else
    if (vector.x == 1 && vector.y == 0)  //right vector
    {
        return cellData [x][y - 1];
    }
    else
    if (vector.x == 0 && vector.y == 1)  //down up vector
    {
        return cellData [x + 1][y];
    }    
    else
    if (vector.x == -1 && vector.y == 0)  //left vector
    {
        return cellData [x][y + 1];
    }     
    else
    {
        QMessageBox::critical(0, "Maze crashed", "Researcher has incorrect vector");
        assert (0);
    }
}

MazeCell* Maze::getForwardCell (int x, int y, Vector vector)
{
    QTextStream out (stdout);    
    out << "Checking forward direction\n";
    out << "x = " << x << ", y = " << y << "\n";
    out << "Vector.x = " << vector.x << "vector.y = " << vector.y << "\n\n";
    
    return cellData [x + vector.x][y + vector.y];
}

MazeCell* Maze::getRightCell (int x, int y, Vector vector)
{
    QTextStream out (stdout);
    out << "Checking right direction\n";
    out << "x = " << x << ", y = " << y << "\n";
    out << "Vector.x = " << vector.x << "vector.y = " << vector.y << "\n\n";    
    
    if (vector.x == 0 && vector.y == -1)  //up vector
    {
        return cellData [x + 1][y];
    }
    else
    if (vector.x == 1 && vector.y == 0)  //right vector
    {
        return cellData [x][y + 1];
    }
    else
    if (vector.x == 0 && vector.y == 1)  //down up vector
    {
        return cellData [x - 1][y];
    }    
    else
    if (vector.x == -1 && vector.y == 0)  //left vector
    {
        return cellData [x][y - 1];
    }     
    else
    {
        QMessageBox::critical(0, "Maze crashed", "Researcher has incorrect vector");
        assert (0);
    }
}

MazeCell* Maze::getPositionCell (int x, int y)
{
    return cellData [x][y];
}

void Maze::generateCell (int x, int y)
{
    if ((x - 1 != -1 && x + 1 != cellDataX) &&
        (y - 1 != -1 && y + 1 != cellDataY) &&
        cellData [x][y] -> state != CellStates::empty)
    {
        int emptyAmount = 0;;
        
        #define CHECK_CELL(cellX, cellY) \
            if (cellData [cellX][cellY] -> state == CellStates::empty) \
                emptyAmount++; 
                
        
        CHECK_CELL (x - 1, y);
        CHECK_CELL (x, y - 1);
        CHECK_CELL (x + 1, y);
        CHECK_CELL (x, y + 1);
        

        
        if (emptyAmount >= 2)
        {
            cellData [x][y] -> state = CellStates::wall;

            return;
        }
        else
        {   
            cellData [x][y] -> state = CellStates::empty;
            cellData [x][y] -> update (); 
            
            int definetlyOneWay = rand () % 4;
            
            if ((rand () % mazeHard != mazeHard - 1) || (definetlyOneWay == 0)) generateCell (x - 1 , y);
            if ((rand () % mazeHard != mazeHard - 1) || (definetlyOneWay == 1)) generateCell (x, y - 1);
            if ((rand () % mazeHard != mazeHard - 1) || (definetlyOneWay == 2)) generateCell (x + 1, y);           
            if ((rand () % mazeHard != mazeHard - 1) || (definetlyOneWay == 3)) generateCell (x, y + 1);
        }
    }
    else return;
}

void Maze::generate()
{
    QTextStream out (stdout);
    
    out << "Maze::Generate function called...\n";
    
    for (int x = 0; x < cellDataX; x++)
         for (int y = 0; y < cellDataY; y++)
         {
              cellData [x][y] -> state = CellStates::wall;
              cellData [x][y] -> update ();
         }
    
    out << "Starting to generate maze...\n";
    
    int genStartPointX = rand () % (cellDataX - 3) + 2;
    int genStartPointY = rand () % (cellDataY - 3) + 2;
    
    out << "genStartPointX = " << genStartPointX << "\ngenStartPointY = " << genStartPointY << "\n";
    
    generateCell (genStartPointX, genStartPointY);  
   
    MazeCell* emptyData [cellDataX*cellDataY];
    int emptyDataSize = 0;
    
    out << "Maze generated...\n";
    
    for (int x = 0; x < cellDataX; x++)
         for (int y = 0; y < cellDataY; y++)
         {
             if (cellData [x][y] -> state == CellStates::empty)
             {
                 emptyData [emptyDataSize] = cellData [x][y];
                 emptyDataSize++;
             }
         } 
    
    out << "emptyDataSize = " << emptyDataSize << "\n\n\n\n";       
    emptyData [rand () % (emptyDataSize - 1) + 1] -> state = CellStates::start;
   
    while (true)
    {       
        int finishCell = rand () % (emptyDataSize - 1) + 1;
        if (emptyData [finishCell] -> state != CellStates::start)
        {
            emptyData [finishCell] -> state = CellStates::finish;
            break;
        }
    }    
}

Maze::~Maze()
{}
//----------------------------------
//MazeCell functions----------------
MazeCell::MazeCell(int newX,     int newY, 
		   int width, int height,
		   QWidget* parent): 
    QWidget (parent),
    x       (newX),
    y       (newY),
    state   (CellStates::wall)
                   
{
    setGeometry           (x, y, width, height);
}

void MazeCell::paintEvent ( QPaintEvent* )
{
    QPainter painter   (this);
    
    painter.translate  (0, 0);
    
    if (state == CellStates::empty)
    {
        //We arent drawing. Cell is empty.
    }
    else
    if (state == CellStates::wall)
    {
        painter.setPen   (Qt::NoPen);
        painter.setBrush (QColor (50, 50, 50));
        painter.drawRect (QRect  (0, 0, 20, 20));
    }
    else
    if (state == CellStates::start)
    {
        painter.setPen   (Qt::black);
        painter.drawLine (QLine (5, 20, 5, 5));
        painter.setBrush (Qt::green);
        painter.drawRect (QRect (5, 5, 10, 5));
    }
    else
    if (state == CellStates::finish)
    {
        painter.setPen   (Qt::black);
        painter.drawLine (QLine (5, 20, 5, 5));
        painter.setBrush (Qt::red);
        painter.drawRect (QRect (5, 5, 10, 5));
    }
}

MazeCell::~MazeCell()
{}
//-------------------------------------------
//Researcher functions-----------------------
Researcher::Researcher (Maze* newMaze, Console* newConsole, QWidget* parent):
    maze    (newMaze),
    console (newConsole),
    QWidget (parent)
{
    state = ResearcherStates::initializing;
    console -> printText ("Researcher state - Initialising\n");
    
    x  = maze -> getStartPoint().x();
    y  = maze -> getStartPoint().y();    
    setGeometry (10 + x * 20, 10 + y * 20, CellWidth, CellHeight);
    vector = {0, -1};
    
    state = ResearcherStates::waiting;
    console -> printText ("Researcher initialised...\n\n"
                          "Researcher state - Waiting");
}

void Researcher::updatePosition (int newX, int newY)
{
    x = newX;
    y = newY;
    setGeometry (10 + x * 20, 10 + y * 20, CellWidth, CellHeight); 
    update ();
}

void Researcher::paintEvent (QPaintEvent*)
{
    QPainter painter (this);
    
    painter.translate (0, 0);
    painter.setPen    (Qt::NoPen);
    painter.setBrush  (Qt::blue);
    
    //const QPoint points[4] = 
    //{
      //  QPoint (CellWidth/2, 0),
     //   QPoint (CellWidth,   CellHeight/2),
    //    QPoint (CellWidth/2, CellHeight),
   //     QPoint (0,           CellHeight/2)
  //  };
    const QPoint points [3] =
    {
        QPoint (CellWidth/2,     0),
        QPoint (CellWidth * 5/6, CellHeight),
        QPoint (CellWidth/6,     CellHeight)
    };
    
    const QPoint points2 [3] = 
    {
        QPoint (4/10 * CellWidth, CellHeight/2),
        QPoint (6/10 * CellWidth, CellHeight/2),
        QPoint (6/10 * CellWidth, CellHeight),
      //  QPoint (4/10 * CellWidth, CellHeight)
    };    
    
    int alpha = 0;
    if (vector.x == 1 && vector.y == 0) alpha = 90;
    else
    if (vector.x == 0 && vector.y == 1) alpha = 180;
    else        
    if (vector.x == -1 && vector.y == 0) alpha = 270;
    else     
    if (vector.x == 0 && vector.y == -1) alpha = 0;
           
    painter.translate (CellWidth/2, CellHeight/2);
    painter.rotate (alpha);
    painter.translate (-CellWidth/2, -CellHeight/2);
    
     painter.drawPolygon (points, 3);
   // painter.drawPolygon (points2, 3);
}

void Researcher::processStep ()
{
    if (state == ResearcherStates::searchingWay)
    {  
        console -> printText ("Researcher state - Searching way...");
        
        if (maze -> getLeftCell    (x, y, vector) -> state != CellStates::wall)
        {
            int newVectorX = (maze -> getLeftCell    (x, y, vector) -> x - 1)/CellWidth  - x; 
            int newVectorY = (maze -> getLeftCell    (x, y, vector) -> y - 1)/CellHeight - y;
            vector = {newVectorX, newVectorY};
        }
        else
        if (maze -> getForwardCell (x, y, vector) -> state != CellStates::wall)
        {
        }
        else
        if (maze -> getRightCell   (x, y, vector) -> state != CellStates::wall)
        {
            int newVectorX = (maze -> getRightCell    (x, y, vector) -> x - 1)/CellWidth - x; 
            int newVectorY = (maze -> getRightCell    (x, y, vector) -> y - 1)/CellHeight - y;
            vector = {newVectorX, newVectorY};
        }  
        else
        {
            vector.x *= -1;
            vector.y *= -1;
        }
        
        updatePosition (x + vector.x, y + vector.y);    
        
        if (maze -> getPositionCell(x, y) -> state == CellStates::finish)
        {
            state = ResearcherStates::finishFound;
            QTextStream out (stdout);
            console -> printText ("FINISH FOUND----\n"
                                  "Researcher state - finish found\n"
                                  "Finish x = %d, Finish y = %d\n", x, y);
            
            emit researcherFoundFinish ();
            
            return;
        }
        
        QTextStream out (stdout);
        console -> printText ("Researcher updated\n"
                              "New x = %d, new y = %d. "
                              "Direction vector = {%d, %d}", x, y, vector.x, vector.y);   
        
        console -> printText ("Researcher state - waiting...");
    }
    if (state == ResearcherStates::finishFound)
    {
        //Nothing to do
    }
}

void Researcher::generateMaze()
{
    QTextStream out (stdout);
    
    out << "Researcher::generateMaze function called...\n";
    
    maze -> generate ();
    
    state = ResearcherStates::initializing;
    updatePosition (maze -> getStartPoint().x(), maze -> getStartPoint().y());
    vector = {0, -1};
    state = ResearcherStates::searchingWay;    
}

Researcher::~Researcher ()
{}
//-----------------------------------------------------
//Console----------------------------------------------

Console::Console (QWidget* parent) : QPlainTextEdit (parent)
{
   setReadOnly (true);
}


void Console::printText (char* text, ...)
{
    char    compText [maxPrintTextSize] = "";
    va_list    arg;

    va_start(arg, text);
    vsprintf(compText, text, arg);

    insertPlainText (QString(compText) + "\n");
    moveCursor(QTextCursor::End); 
    
    va_end(arg);
}




#include "maze.moc"

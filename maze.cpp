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
#include <QCoreApplication>
#include <QFont>

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
			                      this);
              
    cellData [0][0] -> state                         = CellStates::start;
    cellData [cellDataX - 1][cellDataY - 1] -> state = CellStates::finish;
              
    setGeometry           (9, 9, CellWidth * cellDataX + 1, CellWidth * cellDataY + 1);
    setPalette            (QPalette(QColor(230, 230, 230)));
    setAutoFillBackground (true);
    setMinimumSize(600, 500);
    setMaximumSize(600, 500);
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
		   QWidget* parent): 
    QWidget (parent),
    x       (newX),
    y       (newY),
    state   (CellStates::wall)
                   
{
    setGeometry           (x, y, CellWidth, CellHeight);
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
    changeState (ResearcherStates::initialising);
    
    detailedDump = false;
    
    console -> printText ("Researcher state - Initialising");
    
    x  = maze -> getStartPoint().x();
    y  = maze -> getStartPoint().y(); 
    
    myParent = parent;
              
    setGeometry (maze -> mapTo (myParent, QPoint (0, 0)).x() + x * 20, maze -> mapTo (myParent, QPoint (0, 0)).y () + y * 20, CellWidth, CellHeight);
    vector = {0, -1};
    oldVector = {0, -1};
    update ();
    
    if (detailedDump) console -> printText ("Start x = %d, start y = %d\n"
                                            "Start direction vector = {0, -1}");
    
    changeState (ResearcherStates::findLeft);
    
    console -> printText ("State - {0:initialising -> 1:findLeft}. Transition - initialised\n");
}

void Researcher::updatePosition (int newX, int newY)
{
    x = newX;
    y = newY;
    setGeometry (maze -> mapTo (myParent, QPoint (0, 0)).x() + x * 20, maze -> mapTo (myParent, QPoint (0, 0)).y()+ y * 20, CellWidth, CellHeight); 
    update ();
}

void Researcher::paintEvent (QPaintEvent*)
{
    QPainter painter (this);
    
    painter.translate (0, 0);
    painter.setPen    (Qt::NoPen);
    painter.setBrush  (Qt::blue);
    
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
        QPoint (6/10 * CellWidth, CellHeight)
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
     for (int cx = 0; cx < 30; cx++)
         for (int cy = 0; cy < 25; cy++)
              journey [cx][cy] -> setGeometry (maze -> getPositionCell(cx, cy) -> x + maze -> mapTo (myParent, QPoint(0, 0)).x(),
                                               maze -> getPositionCell(cx, cy) -> y + maze -> mapTo (myParent, QPoint(0, 0)).y(),
                                               20, 20);   
     updatePosition (x, y);
   // painter.drawPolygon (points2, 3);
}

bool Researcher::FindLeft ()
{
    if (maze -> getLeftCell    (x, y, vector) -> state != CellStates::wall)
    {
        int newVectorX = (maze -> getLeftCell    (x, y, vector) -> x - 1)/CellWidth  - x; 
        int newVectorY = (maze -> getLeftCell    (x, y, vector) -> y - 1)/CellHeight - y;
        oldVector = vector;
        vector = {newVectorX, newVectorY};
            
        console -> printText ("State - {1:findLeft -> 5:step}. Transition - Left way was found");
        changeState (ResearcherStates::step);
        return true;
    }
    else
    {
        console -> printText ("State - {1:findLeft -> 2:findForward}. Transition - Left way was not found");
        changeState (ResearcherStates::findForward);
        return false;
    }
}

bool Researcher::FindForward ()
{
    if (maze -> getForwardCell (x, y, vector) -> state != CellStates::wall)
    {
        oldVector = vector;
            
        console -> printText ("State - {2:findForward -> 5:step}. Transition - forward way was found");
        changeState (ResearcherStates::step);
        return true;
    }
    else
    {
        console -> printText ("State - {2:findForward -> 3:findRight}. Transition - Forward way was not found");
        changeState (ResearcherStates::findRight);
        return false;
    }
}

bool Researcher::FindRight ()
{
    if (maze -> getRightCell   (x, y, vector) -> state != CellStates::wall)
    {
        int newVectorX = (maze -> getRightCell    (x, y, vector) -> x - 1)/CellWidth - x; 
        int newVectorY = (maze -> getRightCell    (x, y, vector) -> y - 1)/CellHeight - y;
        oldVector = vector;
        vector = {newVectorX, newVectorY};
            
        console -> printText ("State - {3:findRight -> 5:step}. Transition - Right way found");
        changeState (ResearcherStates::step);
        return true;
    }
    else
    {
        console -> printText ("State - {3:findRight -> 4:findBackward}. Transition - Right way not found");
        changeState (ResearcherStates::findBackward);
        return false;
    }
}

bool Researcher::FindBackward ()
{
    if (maze -> getForwardCell   (x, y, Vector (vector.x * -1, vector.y * -1)) -> state != CellStates::wall)
    {
        oldVector = vector;
        vector.x *= -1;
        vector.y *= -1;

        console -> printText ("State - {4:findBackward -> 5:step}. Transition - Backward way was found");
        changeState (ResearcherStates::step);
        return true;
    }
}

bool Researcher::Step ()
{
    //A lot of "ifs" to check all variants of walking in the maze
    //I'm really sorry =(
            
    if ((oldVector.x == 1  && oldVector.y == 0)  &&
        (vector.x    == 0  && vector.y    == -1)) {journey [x][y] -> wayData [Ways::leftUp] = true;}
    if ((oldVector.x == 1  && oldVector.y == 0)  &&
        (vector.x    == 1  && vector.y    == 0))  {journey [x][y] -> wayData [Ways::leftRight] = true;}
    if ((oldVector.x == 1  && oldVector.y == 0)  &&
        (vector.x    == 0  && vector.y    == 1))  {journey [x][y] -> wayData [Ways::leftDown] = true;}
    if ((oldVector.x == 1  && oldVector.y == 0)  &&
        (vector.x    == -1 && vector.y    == 0))  {journey [x][y] -> wayData [Ways::leftLeft] = true;}     
                
    if ((oldVector.x == 0  && oldVector.y == 1)  &&
        (vector.x    == 1  && vector.y    == 0))  {journey [x][y] -> wayData [Ways::upRight] = true;}
    if ((oldVector.x == 0  && oldVector.y == 1)  &&
        (vector.x    == 0  && vector.y    == 1))  {journey [x][y] -> wayData [Ways::upDown] = true;}
    if ((oldVector.x == 0  && oldVector.y == 1)  &&
        (vector.x    == -1 && vector.y    == 0))  {journey [x][y] -> wayData [Ways::upLeft] = true;}
    if ((oldVector.x == 0  && oldVector.y == 1)  &&
        (vector.x    == 0  && vector.y    == -1)) {journey [x][y] -> wayData [Ways::upUp] = true;}             

    if ((oldVector.x == -1 && oldVector.y == 0)  &&
        (vector.x    == 0  && vector.y    == 1))  {journey [x][y] -> wayData [Ways::rightDown] = true;}
    if ((oldVector.x == -1 && oldVector.y == 0)  &&
        (vector.x    == -1 && vector.y    == 0))  {journey [x][y] -> wayData [Ways::rightLeft] = true;}
    if ((oldVector.x == -1 && oldVector.y == 0)  &&
        (vector.x    == 0  && vector.y    == -1)) {journey [x][y] -> wayData [Ways::rightUp] = true;}
    if ((oldVector.x == -1 && oldVector.y == 0)  &&
        (vector.x    == 1  && vector.y    == 0))  {journey [x][y] -> wayData [Ways::rightRight] = true;}   
                
    if ((oldVector.x == 0  && oldVector.y == -1) &&
        (vector.x    == -1 && vector.y    == 0))  {journey [x][y] -> wayData [Ways::downLeft] = true;}
    if ((oldVector.x == 0  && oldVector.y == -1) &&
        (vector.x    == 0  && vector.y    == -1)) {journey [x][y] -> wayData [Ways::downUp] = true;}
    if ((oldVector.x == 0  && oldVector.y == -1) &&
        (vector.x    == 1  && vector.y    == 0))  {journey [x][y] -> wayData [Ways::downRight] = true;}
    if ((oldVector.x == 0  && oldVector.y == -1) &&
        (vector.x    == 0  && vector.y    == 1))  {journey [x][y] -> wayData [Ways::downDown] = true;}            
        
    updatePosition (x + vector.x, y + vector.y);    
        
        
    if (maze -> getPositionCell(x, y) -> state == CellStates::finish)
    {
        console -> printText ("State - {5:step -> 6:finishFound}. Transition - Finish was found");
        changeState (ResearcherStates::finishFound);
            
        if (detailedDump) console -> printText ("FINISH FOUND----\n"
                                                "Researcher state - finish found\n"
                                                "Finish x = %d, Finish y = %d\n", x, y);
            
        emit researcherFoundFinish ();
            
        return true;
     }
     else
     {
        if (detailedDump) console -> printText ("Researcher updated\n"
                                                "New x = %d, new y = %d. "
                                                "Direction vector = {%d, %d}", x, y, vector.x, vector.y);   
            
        console -> printText ("State - {5:step -> 1:findLeft}. Transition - Finish was not found");
        changeState (ResearcherStates::findLeft);
        return false;
     }
}


void Researcher::processStep ()
{   
    if (state == ResearcherStates::findLeft)
    {
        FindLeft ();
    }    
    else
    if (state == ResearcherStates::findForward)
    {
        FindForward ();
    }
    else
    if (state == ResearcherStates::findRight)
    {
        FindRight ();
    }
    else
    if (state == ResearcherStates::findBackward) 
    {
        FindBackward ();
    }
    else
    if (state == ResearcherStates::step)
    {
        Step ();
    }
    else
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
    
    changeState (ResearcherStates::initialising);
    
    console -> printText ("\nState - inialised. Transition - Generate button was pushed");
    
    updatePosition (maze -> getStartPoint().x(), maze -> getStartPoint().y());
    clearJourney ();
    vector = {0, -1};
    
    console -> printText ("State - {initialised -> findLeft}. Transition - initialised");
    
    changeState (ResearcherStates::findLeft);    
    
}

void Researcher::returnToStart()
{    
    changeState (ResearcherStates::initialising);
    
    updatePosition (maze -> getStartPoint().x(), maze -> getStartPoint().y());
    clearJourney ();
    vector = {0, -1};
    
    changeState (ResearcherStates::findLeft);   
}

void Researcher::clearJourney ()
{
    for (int cx = 0; cx < 30; cx++)
         for (int cy = 0; cy < 25; cy++)
         {
              journey [cx][cy] -> clearWayCell ();
              journey [cx][cy] -> update ();
         }
}

void Researcher::simpleDump()
{
    detailedDump = false;
}

void Researcher::notSimpleDump()
{
    detailedDump = true;
}

void Researcher::changeState (const int newState)
{
    state = newState;
    emit stateChanged (state);
}


Researcher::~Researcher ()
{}
//-----------------------------------------------------
//Console----------------------------------------------

Console::Console (QWidget* parent) : QPlainTextEdit (parent)
{
    readOnly (false);
    
    prompt = "user> ";
    
    palette.setColor(QPalette::Text, Qt::darkRed);
    setPalette (palette);
    
    format.setForeground (Qt::darkRed);
    
    history = new QStringList ();
    
    insertPlainText (prompt);
    
    text   = "";
    
    ticksBefore = 0;
    currentCmd = 0;
}

void Console::readOnly (bool read)
{
    readMode = read;
    if (read)
    {
        while (textCursor().positionInBlock() > 0)
             QPlainTextEdit::keyPressEvent (new QKeyEvent (QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier));
        setReadOnly (read);
    }
    else
    {
        insertPlainText (prompt + text);
        setReadOnly (read);
    }
}

void Console::keyPressEvent (QKeyEvent* event)
{
    if ((event -> key() >= 0x20 && event -> key() <= 0x7e) &&
        (event -> modifiers() == Qt::NoModifier || event -> modifiers() == Qt::ShiftModifier))
    {
        text = text + event -> text();
        QPlainTextEdit::keyPressEvent(event);
    }
 
    if(event -> key() == Qt::Key_Backspace &&
       event -> modifiers() == Qt::NoModifier && 
       textCursor().positionInBlock() > prompt.length())
    { 
        text.remove (text.length() - 1, 1);
        QPlainTextEdit::keyPressEvent(event);
    }
    if(event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier)
        onEnter();
    
    if(event->key() == Qt::Key_Up && event->modifiers() == Qt::NoModifier)
        backHistory();
    
    if(event->key() == Qt::Key_Down && event->modifiers() == Qt::NoModifier)
        forwardHistory();    
}

void Console::addHistory(QString cmd)
{
    if (!history -> isEmpty ()) history -> pop_back ();
    history -> append(cmd);
    history -> append("");
    historyPos = history->length();
}

void Console::backHistory()
{
    if(!historyPos)
        return;
    QTextCursor cursor = textCursor();
    cursor.movePosition            (QTextCursor::StartOfBlock);
    cursor.movePosition            (QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText      ();
    
    if(historyPos == 0)
    {
        cursor.insertText(prompt);
        return;
    }
    else
        cursor.insertText(prompt + history -> at (historyPos - 1));
    
    if (historyPos == history -> length ()) historyPos--;
    
    text = history -> at (historyPos - 1);
    setTextCursor(cursor);
    historyPos--;
}

void Console::forwardHistory()
{
    if(historyPos == history->length())
        return;
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    if(historyPos == history->length() - 1)
    {
        cursor.insertText(prompt);
        return;
    }
    else
        cursor.insertText(prompt + history->at(historyPos + 1));
    text = history -> at (historyPos + 1);
    setTextCursor(cursor);
    historyPos++;
}

void Console::insertAbout (bool setColor)
{
    while (textCursor().positionInBlock() > 0)
          QPlainTextEdit::keyPressEvent (new QKeyEvent (QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier));
    format.setForeground(Qt::darkMagenta);

    textCursor().setBlockCharFormat(format);
    
    insertPlainText("\n"
                    "      About:\n"
                    "Developer - Pimkin Artem\n"
                    "Developed with help of:\n"
                    " Ilya Dedinsky\n\n\n");
    if (setColor)
    {
        format.setForeground(Qt::darkRed);
        textCursor().setBlockCharFormat(format);
        insertPlainText (prompt);
        moveCursor(QTextCursor::End);
    }
}

void Console::processScript ()
{
    if (ticksBefore > 0) 
    {
        ticksBefore--;
        return;
    }
    
    if (currentCmd == commandsToDo.size() - 1)
    {
        emit scriptDone ();
        return;
    }
    
    QString command;
    QString arg;
    
    while (commandsToDo [currentCmd] != ' ') 
    {
        command += commandsToDo [currentCmd];
        currentCmd++;
    }
    currentCmd += 1;
    
    if (command == "step")
    {
        emit step ();
    }
    else 
    if (command == "autopass")
    {
        emit autopass ();
    }
    else
    if (command == "stop")
    {
        emit stopAutopassing ();
    }    
    else
    if (command == "generate")
    {
        emit generate ();
        return;
    }  
    else
    if (command == "return")
    {
        emit returnToStart ();
    }    
    else
    if (command == "full_dump")
    {
        emit fullDump ();
    }
    else
    if (command == "states_dump")
    {
        emit statesDump ();
    }    
    else
    if (command == "sleep")
    {
        while (commandsToDo [currentCmd] != ' ') 
        {
            arg += commandsToDo [currentCmd];
            currentCmd++;
        } 
        
        int num = 0;
        QByteArray argx = arg.toAscii();
        for (int i = 0; i < arg.size(); i++)
        {
             num = num*10 + argx[i] - '0';
        }
        
        ticksBefore = num;
        currentCmd++;
    }
    else 
    if (command == "echo")
    {
        currentCmd++;
        while (commandsToDo [currentCmd] != '"') 
        {
            arg += commandsToDo [currentCmd];
            currentCmd++;
        }   
        
        currentCmd += 2;
        QByteArray textToPrint1 = arg.toAscii ();
        char* textToPrint = textToPrint1.data();
        printText(textToPrint);
    }
}

void Console::onEnter ()
{
    QString str = text;
    addHistory (text);
    
    while (textCursor().positionInBlock() > 0)
           QPlainTextEdit::keyPressEvent (new QKeyEvent (QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier));
    insertPlainText (prompt + text + "\n");    
    text = "";
    
    format.setForeground(Qt::darkMagenta);

    textCursor().setBlockCharFormat(format);
    
    
    if (str == "help")
    {
        insertPlainText ("Maze console commands help:\n"
                         "    step - make one step of passing maze\n"
                         "    autopass - begin autopassing maze\n"
                         "    stop - stop autopassing maze\n"
                         "    generate - generate a new maze for passing\n"
                         "    scheme - print legend for the state machine scheme\n"
                         "    full_dump - print full dump during passing\n"
                         "    states_dump - (default) print just states and transitions of state machine during passing\n"
                         "    return - returning your character to the maze start point\n"
                         "    about - print message about programm and author\n");
    }
    else 
    if (str == "about")
    {
        insertAbout (false);
    }
    else
    if (str == "scheme")
    {
        insertPlainText ("Scheme transitions:\n"
                         "0) Transitions from \"0:Initialising\" state:\n"
                         "        a) Passing from \"0:Initialising\" to \"1:FindLeft\" state when state machine will be initialised.\n"
                         "1) Transitions from \"1:FindLeft\" state:\n"
                         "        a) Passing from \"1:FindLeft\" to \"2:FindForward\" state"
                                    " when left clear cell will be not found.\n"
                         "        b) Passing from \"1:FindLeft\" to \"5:Step\" state"
                                    " when left clear cell will be found.\n"
                         "2) Transitions from \"2:FindForward\" state:\n"
                         "        a) Passing from \"2:FindForward\" to \"3:FindRight\" state"
                                    " when forward clear cell will be not found.\n"
                         "        b) Passing from \"2:FindLeft\" to \"5:Step\" state"
                                    " when forward clear cell will be found.\n"
                         "3) Transitions from \"3:FindRight\" state:\n"
                         "        a) Passing from \"3:FindRight\" to \"4:FindBackward\" state"
                                    " when right clear cell will be not found.\n"
                         "        b) Passing from \"3:FindRight\" to \"5:Step\" state"
                                    " when right clear cell will be found.\n"
                         "4) Transitions from \"4:FindBackward\" state:\n"
                         "        a) Passing from \"4:FindBackward\" to \"5:Step\" state"
                                     " when backward clear cell will be found.\n" 
                         "5) Transitions from \"5:Step\" state:\n"
                         "        a) Passing from \"5:Step\" to \"1:FindLeft\" state"
                                    " when finish cell will be not found.\n"
                         "        b) Passing from \"5:Step\" to \"6:FinishFound\" state"
                                    " when finish cell will be found.\n");                           
    }
    else 
    if (str == "step")
    {
        emit step ();
        return;
    }
    else
    if (str == "autopass")
    {
        emit autopass ();
    }
    else
    if (str == "stop")
    {
        emit stopAutopassing ();
    }    
    else
    if (str == "generate")
    {
        emit generate ();
        return;
    }  
    else
    if (str == "return")
    {
        emit returnToStart ();
    }    
    else
    if (str == "full_dump")
    {
        emit fullDump ();
    }
    else
    if (str == "states_dump")
    {
        emit statesDump ();
    }
    else
    {
        if (str [0] == 's' && str [1] == 'c' &&
            str [2] == 'r' && str [3] == 'i' &&
            str [4] == 'p' && str [5] == 't' &&
            str [6] == ' ')
        {
            QString fileName;
            for (int i = 7; i < str.size(); i++)
                 fileName += str[i];
            
            QFile file (QCoreApplication::applicationDirPath () + "/" + fileName);
            file.open (QIODevice::ReadOnly);
            
            QTextStream in (&file);
            
            while (!in.atEnd())
            {
                QString helpStr;
                in >> helpStr;
                commandsToDo += helpStr + " ";
            }
            
            file.close ();
            
            ticksBefore = 0;
            currentCmd = 0;
            
            emit script ();
        }
        else
        {
            format.setForeground(Qt::darkRed);
            insertPlainText ("Unknown command - \"" + str + "\"\n");
        }
    }
    format.setForeground(Qt::darkRed);
    textCursor().setBlockCharFormat(format);
    insertPlainText (prompt);
    moveCursor(QTextCursor::End); 
}

void Console::mousePressEvent (QMouseEvent *)
{
    setFocus ();
}

void Console::mouseDoubleClickEvent (QMouseEvent *)
{
    
}

void Console::contextMenuEvent (QContextMenuEvent *)
{
    
}

void Console::printText (char* text, ...)
{
    bool oldRead = readMode;
    
    readOnly (true);

    format.setForeground(Qt::black);
    textCursor().setBlockCharFormat(format);
    
    char    compText [maxPrintTextSize] = "";
    va_list    arg;

    va_start(arg, text);
    vsprintf(compText, text, arg);

    insertPlainText (QString(compText) + "\n");
    moveCursor(QTextCursor::End); 
    
    va_end(arg);
    
    format.setForeground(Qt::darkRed);
    textCursor().setBlockCharFormat(format);
    readOnly (oldRead);
}

//------------------------------------------------
//Way lighting-----------------------------------
WayLight::WayLight (int newX, int newY, QWidget* parent) : QWidget (parent)
{
    setGeometry (newX, newY, CellWidth, CellHeight);
    for (int i = 0; i < 16; i++)
         wayData [i] = false;
}

void WayLight::clearWayCell ()
{
    for (int i = 0; i < 16; i++)
         wayData [i] = false;
}

void WayLight::paintEvent (QPaintEvent*)
{
    QPainter painter (this);
    painter.translate (0, 0);
    painter.setPen    (Qt::green);
    painter.setBrush  (Qt::NoBrush);
    
    if (wayData [Ways::leftUp]) painter.drawEllipse     (-CellWidth*3/4,  -CellHeight*3/4, 
                                                         2*CellWidth*3/4, 2*CellHeight*3/4);
    if (wayData [Ways::leftRight]) painter.drawLine     (0,               CellHeight*3/4,
                                                         CellWidth,       CellHeight*3/4);
    if (wayData [Ways::leftDown]) painter.drawEllipse   (-CellWidth*1/4,  CellHeight*3/4, 
                                                         CellWidth*2/4,   CellHeight*2/4); 
    if (wayData [Ways::leftLeft]) painter.drawEllipse   (-CellWidth,      CellHeight*1/4, 
                                                         2*CellWidth,     CellHeight*2/4);
                                              
    
    if (wayData [Ways::upRight]) painter.drawEllipse    (CellWidth*1/4,   -CellHeight*3/4, 
                                                         2*CellWidth*3/4, 2*CellHeight*3/4);
    if (wayData [Ways::upDown]) painter.drawLine        (CellWidth*1/4,   0,
                                                         CellWidth*1/4,   CellHeight);
    if (wayData [Ways::upLeft]) painter.drawEllipse     (-CellWidth*1/4,  -CellHeight*1/4, 
                                                         CellWidth*2/4,   CellHeight*2/4); 
    if (wayData [Ways::upUp]) painter.drawEllipse       (CellWidth*1/4,   -CellHeight, 
                                                         CellWidth*2/4,   2*CellHeight - 2);   
    
    
    if (wayData [Ways::rightDown]) painter.drawEllipse  (CellWidth*1/4,   CellHeight*1/4, 
                                                         2*CellWidth*3/4, 2*CellHeight*3/4);
    if (wayData [Ways::rightLeft]) painter.drawLine     (0,               CellHeight*1/4,
                                                         CellWidth,       CellHeight*1/4);
    if (wayData [Ways::rightUp]) painter.drawEllipse    (CellWidth*3/4,  -CellHeight*1/4, 
                                                         CellWidth*2/4,   CellHeight*2/4); 
    if (wayData [Ways::rightRight]) painter.drawEllipse (0,               CellHeight*1/4, 
                                                         2*CellWidth,     CellHeight*2/4);
    
   
    if (wayData [Ways::downLeft]) painter.drawEllipse   (-CellWidth*3/4,  CellHeight*1/4, 
                                                         2*CellWidth*3/4, 2*CellHeight*3/4);
    if (wayData [Ways::downUp]) painter.drawLine        (CellWidth*3/4,   0,
                                                         CellWidth*3/4,   CellHeight);
    if (wayData [Ways::downRight]) painter.drawEllipse  (CellWidth*3/4,   CellHeight*3/4, 
                                                         CellWidth*2/4,   CellHeight*2/4); 
    if (wayData [Ways::downDown]) painter.drawEllipse   (CellWidth*1/4,   0, 
                                                         CellWidth*2/4,   2*CellHeight);    
}

Scheme::Scheme(QWidget* parent): QWidget (parent)
{
    setGeometry (620, 250, 170, 280);
    schemeImage = new QPixmap (QCoreApplication::applicationDirPath () + "/Initialising_Scheme.png");
}

void Scheme::researcherStateChanged (int newState)
{
    delete schemeImage;
    
    if (newState == ResearcherStates::initialising) 
        schemeImage = new QPixmap (QCoreApplication::applicationDirPath () + "/Initialising_Scheme.png");
    else
    if (newState == ResearcherStates::findLeft) 
        schemeImage = new QPixmap (QCoreApplication::applicationDirPath () + "/FindLeft_Scheme.png");
    else   
    if (newState == ResearcherStates::findForward) 
        schemeImage = new QPixmap (QCoreApplication::applicationDirPath () + "/FindForward_Scheme.png");
    else        
    if (newState == ResearcherStates::findRight) 
        schemeImage = new QPixmap (QCoreApplication::applicationDirPath () + "/FindRight_Scheme.png");
    else
    if (newState == ResearcherStates::findBackward) 
        schemeImage = new QPixmap (QCoreApplication::applicationDirPath () + "/FindBackward_Scheme.png");
    else
    if (newState == ResearcherStates::step) 
        schemeImage = new QPixmap (QCoreApplication::applicationDirPath () + "/Step_Scheme.png");
    else        
    if (newState == ResearcherStates::finishFound) 
        schemeImage = new QPixmap (QCoreApplication::applicationDirPath () + "/FinishFound_Scheme.png");  
    
    update();    
}


void Scheme::paintEvent (QPaintEvent*)
{
    //QPixmap pic ("Finish_Scheme.png");

    QPainter painter (this);
    
    painter.translate  (0, 0);
    painter.setPen     (Qt::black);
    
    painter.setRenderHint (QPainter::SmoothPixmapTransform);
    
    painter.drawPixmap (0, 0, width(), height(), *schemeImage);
    
  //  painter.drawEllipse(0, 0, 170, 280);

}

#include "maze.moc"


// <--- This file is machine generated, DO NOT EDIT! --->

//--------------------------------------------------
//
//! Automata.cpp
// 
//! Part of Visio2Auto automata compiler (c) Lnd1212, 2008-09
//
//! This file contains automata implementation.
//
//--------------------------------------------------

#include "automata.h"
#include <QTextStream>
#ifdef __cplusplus
namespace automata {
#endif

//-------------------------------------------------------

static struct AutomataData g_AutomataData = {28}; 

//-------------------------------------------------------
//! Automata definitions
//-------------------------------------------------------

/*Maze Researcher*/
void Automaton_Researcher (int event_num, Researcher* researcher)
{
	int old_y = g_AutomataData.y_Researcher;
	QTextStream out (stdout);
        bool found = false;
        
	switch (g_AutomataData.y_Researcher)
	{
		
		case 5:
			/*2: FindForward*/
			found = researcher -> FindForward ();
			if (!found)
			{
				
				g_AutomataData.y_Researcher = 6;
			}
			
			else if (found)
			{
				
				g_AutomataData.y_Researcher = 12;
			}
			
			break;
		
		case 23:
			/*6: FinishFound*/
			
			break;
		
		case 0:
			/*0: Initialising*/
			researcher -> returnToStart ();
			if (true)
			{
				
				g_AutomataData.y_Researcher = 28;
			}
			
			break;
		
		case 7:
			/*4: FindBackward*/
			found = researcher -> FindBackward ();
                        
                        
                        out << "FindBackward was called\n";
                        
			if (found)
			{
				
				g_AutomataData.y_Researcher = 12;
			}
			
			break;
		
		case 12:
			/*5: Step*/
			found = researcher -> Step ();
			if (!found)
			{
				
				g_AutomataData.y_Researcher = 28;
			}
			
			else if (found)
			{
				
				g_AutomataData.y_Researcher = 23;
			}
			
			break;
		
		case 6:
			/*3: FindRight*/
			found = researcher -> FindRight ();
                        out << "FindRight called\n";
			if (!found)
			{
				out << "Calling FindRight\n";
				g_AutomataData.y_Researcher = 7;
			}
			
			else if (found)
			{
				out << "Calling Step\n";
				g_AutomataData.y_Researcher = 12;
			}
			
			break;
		
		case 28:
			/*1: FindLeft*/
			found = researcher -> FindLeft ();
			if (!found)
			{
				
				g_AutomataData.y_Researcher = 5;
			}
			
			else if (found)
			{
				
				g_AutomataData.y_Researcher = 12;
			}
			
			break;	
}

}
//-------------------------------------------------------

#ifdef __cplusplus
}
#endif


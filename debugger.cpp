/*debugger.cpp*/

//
// Debugger for nuPython, C++ edition! Provides a simple gdb-like
// interface, with support for multiple breakpoints and step-by-step
// execution of nuPython, including if stmts. Uses nuPython interpreter 
// as execution engine.
//
// Author:
//   << YOUR NAME>
// 
// Initial template:
//   Ata Guvendi
//   Prof. Hummel
// Northwestern University
// CS 211
//

#include <iostream>
#include <string>

#include "debugger.h"

using namespace std;


//
// constructor:
//
Debugger::Debugger(struct STMT* program)
{
}


//
// destructor:
//
Debugger::~Debugger()
{
}


//
// run:
//
// Run the debugger for one execution run of the input program.
//
void Debugger::run()
{
  string cmd;
  
  //
  // command loop until quit is entered:
  //
  while (true) {
    
    cout << endl;
    cout << "Enter a command, type h for help. Type r to run. > " << endl;
    cin >> cmd;
    
    if (cmd == "q") {

      break;
    }
    else if (cmd == "h") {
      
      cout << "Available commands:"
        << endl << "r -> Run the program / continue from a breakpoint"
        << endl << "s -> Step to next stmt by executing current stmt"
        << endl << "b n -> Breakpoint at line n"
        << endl << "rb n -> Remove breakpoint at line n"
        << endl << "lb -> List all breakpoints"
        << endl << "cb -> Clear all breakpoints"
        << endl << "p varname -> Print variable"
        << endl << "pg -> Program graph"
        << endl << "sm -> Show memory contents"
        << endl << "ss -> Show state of debugger"
        << endl << "w -> What line are we on?"
        << endl << "q -> Quit the debugger"
        << endl;
    }
    else {
      
      cout << "unknown command" << endl;
    }
    
  }//while
  
}//run


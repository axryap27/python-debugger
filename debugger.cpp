/*debugger.cpp*/

//
// Debugger for nuPython, C++ edition! Provides a simple gdb-like
// interface, with support for multiple breakpoints and step-by-step
// execution of nuPython, including if stmts. Uses nuPython interpreter
// as execution engine.
//
// Author:
//   Aarya Patel
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
#include "programgraph.h"
#include "scanner.h"
#include "execute.h"

using namespace std;

//
// constructor:
//
Debugger::Debugger(struct STMT *program)
{
  this->program = program;                   // save the root for "pg", etc.
  this->curr_stmt = program;                // start at the beginning
  if (program != nullptr){
    this->next_stmt = get_next_stmt(program);
  }
  else{
    this->next_stmt = nullptr;
  }
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
  while (true)
  {

    cout << endl;
    cout << "Enter a command, type h for help. Type r to run. > " << endl;
    cin >> cmd;

    if (cmd == "q")
    {

      break;
    }
    else if (cmd == "h")
    {

      cout << "Available commands:"
           << endl
           << "r -> Run the program / continue from a breakpoint"
           << endl
           << "s -> Step to next stmt by executing current stmt"
           << endl
           << "b n -> Breakpoint at line n"
           << endl
           << "rb n -> Remove breakpoint at line n"
           << endl
           << "lb -> List all breakpoints"
           << endl
           << "cb -> Clear all breakpoints"
           << endl
           << "p varname -> Print variable"
           << endl
           << "pg -> Program graph"
           << endl
           << "sm -> Show memory contents"
           << endl
           << "ss -> Show state of debugger"
           << endl
           << "w -> What line are we on?"
           << endl
           << "q -> Quit the debugger"
           << endl;
    }
    else if (cmd == "s")
    {

      // line #
      // "#: stmt"
      // some tracking to make sure we are going to get stmt/line --> pointer to stmt graph prob
    }
    else if (cmd == "w"){
      Debugger::step();
    }
    else if (cmd == "pg"){
      programgraph_print(program);
    }
    else
    {

      cout << "unknown command" << endl;
    }

  } // while

} // run

//
// Get_Next_Stmt --- See header file for comments
struct STMT* Debugger::get_next_stmt(struct STMT* stmt)
{
    if (stmt == nullptr){
      return nullptr;
    }
    else if (stmt->stmt_type == STMT_ASSIGNMENT){
      return stmt->types.assignment->next_stmt;
    }
    else if (stmt->stmt_type == STMT_FUNCTION_CALL){
      return stmt->types.function_call->next_stmt;
    }
    else if (stmt->stmt_type == STMT_IF_THEN_ELSE){
      return stmt->types.if_then_else->next_stmt;
    }
    else if (stmt->stmt_type == STMT_PASS){
      return stmt->types.pass->next_stmt;
    }
    else if (stmt->stmt_type == STMT_WHILE_LOOP){
      return stmt->types.while_loop->next_stmt;
    }
    else 
      return nullptr;
} // get_next_stmt


// step function
// see header file for comments
void Debugger::step(){
  if(curr_stmt == nullptr){
    cout << "program has completed" << endl;
    return;
  }
  // temp variable for next_stmt 
  struct STMT* saved_next = next_stmt;
  set_next_stmt(curr_stmt, nullptr);

  execute(curr_stmt, nullptr);

  // relink and move to next line
  set_next_stmt(curr_stmt, saved_next);
  curr_stmt = saved_next;
  next_stmt = get_next_stmt(curr_stmt);
}

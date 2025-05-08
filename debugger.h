/*debugger.h*/

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

#pragma once

#include "programgraph.h"

using namespace std;


class Debugger
{
private:
  struct STMT* program; //root of the program graph struct
  struct STMT* curr_stmt; //statement we are on 
  struct STMT* next_stmt; // next statement
  struct RAM* mem; // execution needed memory
  string state; 

public:
  Debugger(struct STMT* program);

  ~Debugger();

  void run();

  // Get_Next_Stmt
  //
  // Gets the next stmt based on the statement type of the current statement inputted
  // Gets the pointer pointing to the specific stmt type and then gets the pointer 
  // to the next stmt in the program graph
  struct STMT* get_next_stmt(struct STMT* stmt);


  //
  // set_next_stmt
  //
  // Sets the `next_stmt` pointer of the given nuPython statement to a new
  // value. This is used to temporarily break or restore links in the program
  // graph so that only one statement is executed at a time (e.g., during step()).
  //
  void set_next_stmt(struct STMT* stmt, struct STMT* next);


  // step
  //
  // Performs a "step" operation on the program by linking and unlinking
  // the next_stmt and curr_stmt pointers along the program graph, while also executing the lines 
  // that correspond to the curr_stmt as it traverses the program graph
  void step();


  // print_line
  //
  // Saves the next_stmt pointer in a temp var, then sets the next_stmt of curr_stmt to null
  // This allows for programgraph_print() to print only a passed stmt pointer
  // The link to next_stmt is restored before the function exits by using the saved pointer
  void print_line();


  // print_ram_value
  //
  // Prints the value, ram addr, and variable name of a variable inputted
  // Goes through the different 'RAM_TYPE's and outputs in the format 
  // "varname (type): value" using pass by value. The copy is freed after the output is made.
  void print_ram_value();
  
};


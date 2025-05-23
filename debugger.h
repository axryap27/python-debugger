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


#pragma once
#include <vector>
#include "programgraph.h"
#include "ram.h"

using namespace std;


class Debugger
{
private:
  struct STMT* program; //root of the program graph struct
  struct STMT* curr_stmt; //statement we are on 
  struct STMT* next_stmt; // next statement
  struct RAM* mem; // execution needed memory
  string state; 
  vector<int> breakpoints;
  // used for if "r" is clicked more than once
  int completed_r_count = 0;
  
  //used to make sure if we already printed out breakpoint so next time "s" is pressed we run that breakpoint
  int last_bp_line = -1; 

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
  // the next_stmt and curr_stmt pointers along the program graph, whilqe also executing the lines 
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
  void print_ram_value(string varname, RAM_VALUE* value);
  
  // unlink_stmt
  //
  // Unlinks the curr stmt and a passed by reference of the saved next stmt 
  // which is initilaized in debugger.cpp as nullptr. The pointer to the pointer is needed
  // for the value to change outside of the function as well and not just locally as the copy of the pointer.
  //
  void unlink_stmt(STMT* stmt, STMT** saved_next, STMT** saved_true = nullptr, STMT** saved_false = nullptr);
  
  // relink_stmt
  //
  // Relinks passed stmts together by just setting next_stmt to the saved_next temp variable
  // Checks to see what type of stmt curr is because if it is an if-then-stmt we need to 
  // fork the curr_stmt and unlink all paths down condtional 
  void relink_stmt(STMT* stmt, STMT* saved_next, STMT* saved_true = nullptr, STMT* saved_false = nullptr);
  
  // set_bp
  // 
  // If the inputted line number is not in the vector, it adds the line to breakpoints.
  // If the line number exists outputs to let the user know.
  void set_bp(int linenum);

  // line_exists
  //
  // Traverses through the program graph using a pointer and checks
  // if the line number is within the scope/range of the program graph.
  // Returns a bool corresponding to if the line number exists in the program
  bool line_exists(STMT* stmt, int linenum);

  //remove_bp
  //
  // If the inputted line number is in the vector of breakpoints, it removes that element.
  // Outputs a statement to indicate if the action was successful or failed
  void remove_bp(int linenum);

  //clear_bps
  //
  // Removes every element of breakpoints vector by popping the last element repeatedly
  void clear_bps();

};


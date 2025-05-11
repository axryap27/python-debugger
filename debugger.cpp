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
#include <vector>
#include <algorithm>

#include "debugger.h"
#include "programgraph.h"
#include "scanner.h"
#include "execute.h"
#include "ram.h"

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
  this->mem = ram_init();
  this->state = "Loaded";
}

//
// destructor:
//
Debugger::~Debugger()
{
  ram_destroy(mem);
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
    // steps through the programgraph STRUCT
   else if (cmd == "s"){
    if (state == "Completed" || curr_stmt == nullptr){
      cout << "program has completed" << endl;
    }
    else {
      if (state == "Loaded"){
        state = "Running";
      }
    step();
    
    // We don't need to check if step caused completion here, as the
    // next time 's' is pressed, it will show "program has completed"
  }
}
    // what line is this? cmd
    else if (cmd == "w"){
      // if (state == "Completed"){
      //   cout << "completed execution" << endl;
      // }
      if (curr_stmt == nullptr || state == "Completed"){
        cout << "completed execution" << endl;
      }
      if (curr_stmt != nullptr && state != "Completed"){
        cout << "line " << curr_stmt->line << endl;
        print_line();
      }
        // temp unlinks to access the program's specific line info
        // set_next_stmt(curr_stmt, next_stmt);

        // cout << "line " << curr_stmt->line << endl;
        // cout << curr_stmt->line << ": " << programgraph_print(curr_stmt) << endl;
        // programgraph_print(curr_stmt);
    }
    // prints full program
    else if (cmd == "pg"){
      programgraph_print(program);
    }
    // shows programs RAM
    else if (cmd == "sm"){
      ram_print(mem);
    }
    // shows program's state
    else if (cmd == "ss"){
      cout << state << endl;
    }
    // prints varname retrieves from memory
    else if (cmd == "p"){
      string varname;
      cin >> varname;
      
      const char* name = varname.c_str();
      RAM_VALUE* value = ram_read_cell_by_name(mem, (char*) name);

      // print helper function
      print_ram_value(varname, value);
    }
    else if (cmd == "b"){
      int linenum;
      cin >> linenum;

      if (!line_exists(program, linenum)){
        cout << "no such line" << endl;
      }
      // if linenum within the bounds of program set_bp in breakpoints
      set_bp(linenum);
    }
    else if (cmd == "rb"){
      int linenum;
      cin >> linenum;

      remove_bp(linenum);

    }
    else if (cmd == "lb") {
      if (breakpoints.empty()) {
        cout << "no breakpoints" << endl;
      } 
      else {
        // Copy and sort
        vector<int> sorted = breakpoints;
        sort(sorted.begin(), sorted.end());
        cout << "breakpoints on lines: ";
        for (int bp : sorted) {
            cout << bp << " ";
        }
      cout << endl;
      } 
    }
    else if (cmd == "cb"){
      clear_bps();
    }
    else if (cmd == "r") {

      if (state == "Completed" || curr_stmt == nullptr) {
        cout << "program has completed" << endl;
        continue;
      }
      
      if (state == "Loaded") {
        state = "Running";
      }
      
        while (curr_stmt != nullptr && state != "Completed") {
        // Check for breakpoints at the current statement
        bool hit = false;
        for (int bp : breakpoints) {
          if (curr_stmt != nullptr && curr_stmt->line == bp && curr_stmt->line != last_bp_line) {
            cout << "hit breakpoint at line " << curr_stmt->line << endl;
            print_line();
            last_bp_line = curr_stmt->line;
            hit = true;
            break;
          }
        }
        
        if (hit) {
          break;  // stop at breakpoint before executing
        }
        
        // Execute the current statement and update pointers
        step();
        
        if (state == "Completed") {
          break;
        }
      }


      // if (state == "Completed" || curr_stmt == nullptr) {
      //   cout << "program has completed" << endl;
      //   continue;
      // }
      
      // if (state == "Loaded") {
      //   state = "Running";
      // }
      
      // while (curr_stmt != nullptr && state != "Completed") {
      //   bool hit = false;
        
      //   // Check for breakpoints
      //   for (int bp : breakpoints) {
      //     if (curr_stmt != nullptr && curr_stmt->line == bp && curr_stmt->line != last_bp_line) {
      //       cout << "hit breakpoint at line " << curr_stmt->line << endl;
      //       print_line();
      //       last_bp_line = curr_stmt->line;
      //       hit = true;
      //       break;
      //     }
      //   }
        
      //   if (hit) {
      //     break;  // stop at breakpoint before executing
      //   }
        
      //   step();  // run current statement
        
      //   if (state == "Completed") {
      //     break;  // Exit the loop but don't print any message
      //   }
      // }
    }
    
    else if (cmd == ""){
      cout << "unknown command" << endl;
    }
    else{
      cout << "unknown command" << endl;
    }
  } // while
 }// run


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

//
// set_next_stmt
// see header for comments
void Debugger::set_next_stmt(struct STMT* stmt, struct STMT* next){
    if (stmt == nullptr)
      return;
    if (stmt->stmt_type == STMT_ASSIGNMENT) {
    stmt->types.assignment->next_stmt = next;
    }
    else if (stmt->stmt_type == STMT_FUNCTION_CALL) {
      stmt->types.function_call->next_stmt = next;
    }
    else if (stmt->stmt_type == STMT_PASS) {
      stmt->types.pass->next_stmt = next;
    }
    else if (stmt->stmt_type == STMT_IF_THEN_ELSE) {
      stmt->types.if_then_else->next_stmt = next;
    }
    else if (stmt->stmt_type == STMT_WHILE_LOOP) {
      stmt->types.while_loop->next_stmt = next;
    }
}

//
// step function
// see header file for comments
void Debugger::step(){
  if (state == "Completed" || curr_stmt == nullptr) {
    state = "Completed";
    return;
  }
  
  if (state == "Loaded") {
    state = "Running";
  }

  // Special handling for if statements
  if (curr_stmt != nullptr && curr_stmt->stmt_type == STMT_IF_THEN_ELSE) {
    // Get pointers to the next, true, and false paths
    STMT* next_path = curr_stmt->types.if_then_else->next_stmt;
    STMT* true_path = curr_stmt->types.if_then_else->true_path;
    STMT* false_path = curr_stmt->types.if_then_else->false_path;
    
    // Get the condition expression
    EXPR* condition = curr_stmt->types.if_then_else->condition;
    
    // Evaluate just the condition using execute_expr
    RAM_VALUE* condition_result = execute_expr(curr_stmt, mem, condition);
    
    if (condition_result == nullptr) {
      // Condition evaluation failed
      state = "Completed";
      return;
    }
    
    // Determine which branch to take based on the condition result
    bool take_true_branch = false;
    
    if (condition_result->value_type == RAM_TYPE_BOOLEAN || 
        condition_result->value_type == RAM_TYPE_INT) {
      take_true_branch = (condition_result->types.i != 0);
    } else if (condition_result->value_type == RAM_TYPE_REAL) {
      take_true_branch = (condition_result->types.d != 0.0);
    } else if (condition_result->value_type == RAM_TYPE_STR) {
      take_true_branch = (condition_result->types.s != nullptr && 
                        condition_result->types.s[0] != '\0');
    } else if (condition_result->value_type == RAM_TYPE_PTR) {
      take_true_branch = (condition_result->types.i != 0);
    } else {
      take_true_branch = false;  // For RAM_TYPE_NONE or other types
    }
    
    // Free the condition result as we're done with it
    ram_free_value(condition_result);
    
    // Set curr_stmt to the appropriate branch
    if (take_true_branch && true_path != nullptr) {
      curr_stmt = true_path;
    } else if (!take_true_branch && false_path != nullptr) {
      curr_stmt = false_path;
    } else {
      curr_stmt = next_path;  // If the selected branch is nullptr, go to next statement
    }
    
    if (curr_stmt != nullptr) {
      next_stmt = get_next_stmt(curr_stmt);
    } else {
      next_stmt = nullptr;
      state = "Completed";
    }
    
    return;
  }
  
  // For non-if statements, proceed as normal
  STMT* saved_next = nullptr;
  STMT* saved_true = nullptr;
  STMT* saved_false = nullptr;
  
  unlink_stmt(curr_stmt, &saved_next, &saved_true, &saved_false);
  ExecuteResult result = execute(curr_stmt, mem);
  last_bp_line = -1;
  relink_stmt(curr_stmt, saved_next, saved_true, saved_false);
  
  if (!result.Success) {
    state = "Completed";
    return;
  }
  
  curr_stmt = saved_next;
  if (curr_stmt != nullptr) {
    next_stmt = get_next_stmt(curr_stmt);
  } else {
    next_stmt = nullptr;
    state = "Completed";
  }
}
  
  // if (state == "Completed" || curr_stmt == nullptr){
  //   state = "Completed";
  //   return;
  // }
  
  // if (state == "Loaded"){
  //   state = "Running";
  // }

  // if (curr_stmt->stmt_type == STMT_IF_THEN_ELSE){
  //   // check execute result to see if we need to go down this path
  // }

  // // Check if we are at a breakpoint first before doing anything
  // for (int bp : breakpoints) {
  //   if (curr_stmt->line == bp && curr_stmt->line != last_bp_line) {
  //     cout << "hit breakpoint at line " << curr_stmt->line << endl;
  //     print_line();  // Show the line in Python syntax
  //     last_bp_line = curr_stmt->line; // this is the line we stopped at
  //     return;  // do not execute this line yet
  //   }
  // }

  // STMT* saved_next = nullptr;
  // STMT* saved_true = nullptr;
  // STMT* saved_false = nullptr;

  // unlink_stmt(curr_stmt, &saved_next, &saved_true, &saved_false);
  // ExecuteResult result = execute(curr_stmt, mem);
  // last_bp_line = -1;

  // if (curr_stmt->stmt_type == STMT_IF_THEN_ELSE) {
  //   relink_stmt(curr_stmt, saved_next, saved_true, saved_false);
    
  //   curr_stmt = result.LastStmt;
  //   // Get the result of the condition evaluation
  //   if (result.Success) {
  //     curr_stmt = saved_true;  // Take the true path
  //   } 
  //   else {
  //     curr_stmt = saved_false; // Take the false path
  //   }
  // } 
  // else {
  //   relink_stmt(curr_stmt, saved_next, saved_true, saved_false);
  //   curr_stmt = saved_next;  // For non-if statements, continue to next statement
  // }

  // if (!result.Success){
  //   state = "Completed";
  //   return;
  // }
  
  // // curr_stmt = saved_next;
  // if (curr_stmt != nullptr) {
  //   next_stmt = get_next_stmt(curr_stmt);
  // } 
  // else {
  //   next_stmt = nullptr;
  //   state = "Completed";
  // }

//
// print_line function
// see .h file for comments
void Debugger::print_line(){
  
  if (state == "Completed" || curr_stmt == nullptr) {
    cout << "completed execution" << endl;
    return;
  }
  STMT* saved_next = nullptr;
  unlink_stmt(curr_stmt, &saved_next);
  programgraph_print(curr_stmt);
  relink_stmt(curr_stmt, saved_next);
}

//
// print_ram_value
// see .h file for comments
void Debugger::print_ram_value(string varname, RAM_VALUE* value){
   
   if (value == nullptr){
    cout << "no such variable" << endl;
    return;
   }

   cout << varname << " (";

  if (value->value_type == RAM_TYPE_INT) {
    cout << "int): " << value->types.i;
  }
  else if (value->value_type == RAM_TYPE_BOOLEAN) {
    cout << "bool): " << value->types.i;
  }
  else if (value->value_type == RAM_TYPE_PTR) {
    cout << "ptr): " << value->types.i;
  }
  else if (value->value_type == RAM_TYPE_REAL) {
    cout << "real): " << value->types.d;
  }
  else if (value->value_type == RAM_TYPE_STR) {
    cout << "str): " << value->types.s;
  }
  else if (value->value_type == RAM_TYPE_NONE) {
    cout << "none): None";
  }
  cout << endl;
  ram_free_value(value);  // only once, at the end

}

//
// unlink_stmt
//
void Debugger::unlink_stmt(STMT* stmt, STMT** saved_next, STMT** saved_true, STMT** saved_false){
    if (stmt == nullptr)
      return;

    if (stmt->stmt_type == STMT_ASSIGNMENT) {
      *saved_next = stmt->types.assignment->next_stmt;
      stmt->types.assignment->next_stmt = nullptr;
    }
    else if (stmt->stmt_type == STMT_FUNCTION_CALL) {
      *saved_next = stmt->types.function_call->next_stmt;
      stmt->types.function_call->next_stmt = nullptr;
    }
    else if (stmt->stmt_type == STMT_PASS) {
      *saved_next = stmt->types.pass->next_stmt;
       stmt->types.pass->next_stmt = nullptr;
    }
    else if (stmt->stmt_type == STMT_IF_THEN_ELSE) {
      if (saved_next)  
        *saved_next = stmt->types.if_then_else->next_stmt;
      if (saved_true)  
        *saved_true = stmt->types.if_then_else->true_path;
      if (saved_false) 
        *saved_false = stmt->types.if_then_else->false_path;

      stmt->types.if_then_else->next_stmt = nullptr;
      stmt->types.if_then_else->true_path = nullptr;
      stmt->types.if_then_else->false_path = nullptr;
  }
}

//
// relink_stmt
//
void Debugger::relink_stmt(STMT* stmt, STMT* saved_next, STMT* saved_true, STMT* saved_false) {
  if (stmt == nullptr) return;

  if (stmt->stmt_type == STMT_ASSIGNMENT) {
    stmt->types.assignment->next_stmt = saved_next;
  }
  else if (stmt->stmt_type == STMT_FUNCTION_CALL) {
    stmt->types.function_call->next_stmt = saved_next;
  }
  else if (stmt->stmt_type == STMT_PASS) {
    stmt->types.pass->next_stmt = saved_next;
  }
  else if (stmt->stmt_type == STMT_IF_THEN_ELSE) {
    stmt->types.if_then_else->next_stmt = saved_next;
    stmt->types.if_then_else->true_path = saved_true;
    stmt->types.if_then_else->false_path = saved_false;
  }
}


//
// set_breakpoint
// 
void Debugger::set_bp(int linenum){
  if (!line_exists(program, linenum)){
    return;
  }
  bool exists = false;
  for (int bp : breakpoints){
    if (bp == linenum){
      exists = true;
      break;
    }
  }

  if (exists){
    cout << "breakpoint already exists" << endl;
  }
  else{
    breakpoints.push_back(linenum);
    cout << "breakpoint set" << endl;
  }
}
//
// line_exists
//
bool Debugger::line_exists(STMT* stmt, int linenum){
  if (stmt == nullptr) 
    return false;
  // using a stack-esque ds to search all paths if stmt pointer is a if-then or while stmt
  vector<STMT*> stack;
  stack.push_back(stmt);

  while (!stack.empty()) {
    STMT* current = stack.back();
    stack.pop_back();

    if (current == nullptr) 
      continue;
    if (current->line == linenum) 
      return true;

    // Add subsidary branches (true/false branches, loop bodies) to stack
    if (current->stmt_type == STMT_IF_THEN_ELSE) {
      stack.push_back(current->types.if_then_else->next_stmt);
      stack.push_back(current->types.if_then_else->true_path);
      stack.push_back(current->types.if_then_else->false_path);
    }
    else if (current->stmt_type == STMT_WHILE_LOOP) {
      stack.push_back(current->types.while_loop->next_stmt);
      stack.push_back(current->types.while_loop->loop_body);
    }
    else {
      stack.push_back(get_next_stmt(current));
    }
  }
  return false;
  // while (stmt != nullptr){
  //   if (stmt->line == linenum){
  //     return true;
  //   }
  //   stmt = get_next_stmt(stmt);
  // }
  // return false;

}
//
// remove_bp
//
void Debugger::remove_bp(int linenum){
  for (auto iter = breakpoints.begin(); iter < breakpoints.end(); iter++){
    if(*iter == linenum){
      iter = breakpoints.erase(iter);
      cout << "breakpoint removed" << endl;
      return;
    }
  }
  cout << "no such breakpoint" << endl;
}
//
// clear_bps
//
void Debugger::clear_bps(){
  for (int bp : breakpoints)
    breakpoints.pop_back();
  cout << "breakpoints cleared" << endl;
}

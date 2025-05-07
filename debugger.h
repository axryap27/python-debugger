/*debugger.h*/

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

#pragma once

#include "programgraph.h"

using namespace std;


class Debugger
{
private:



public:
  Debugger(struct STMT* program);

  ~Debugger();

  void run();
};


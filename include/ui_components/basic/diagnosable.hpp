#pragma once

#include <string>

class Diagnosable {
 public:
  virtual std::string toString(int indent = 0) const = 0;

 protected:
  virtual void debugFillProperties(std::ostringstream &os, int indent) const = 0;
};

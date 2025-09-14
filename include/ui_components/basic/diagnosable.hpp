#pragma once

#include <string>

/**
 * @brief An interface for objects that can provide a string representation for debugging.
 *
 * The Diagnosable interface provides a way to get a string representation of an object's
 * internal state, which is useful for debugging and logging.
 */
class Diagnosable {
 public:
  /** @brief Generates a string representation of the objects state.
   *
   * This method provides a detailed string representation of the object, including its
   * properties and potentially its children, for debugging purposes.
   * @param indent The current indentation level for formatting the output.
   * @return A string representing the objects state.
   */
  virtual std::string toString(int indent = 0) const = 0;

 protected:
  /**
   * @brief Fills the output stream with the objects debug properties.
   *
   * This pure virtual method is intended to be implemented by derived classes to provide
   * their specific properties for debugging.
   * @param os The output string stream to fill with properties.
   * @param indent The current indentation level for formatting the output.
   */
  virtual void debugFillProperties(std::ostringstream &os, int indent) const = 0;
};

/*! \file expression.hpp
Defines the Expression type and assiciated functions.
 */
#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

// system includes
#include <map>
#include <string>
#include <vector>
#include <algorithm> 
#include <csignal>
#include <cstdlib>
#include "token.hpp"
#include "atom.hpp"

const double N = 20.0;
const double A = 3.0;
const double B = 3.0;
const double C = 2.0;
const double D = 2.0;
const double P = 0.5;

extern sig_atomic_t global_status_flag;

// forward declare Environment
class Environment;

/*! \class Expression
\brief An expression is a tree of Atoms.

An expression is an atom called the head followed by a (possibly empty) 
list of expressions called the tail.
 */
class Expression {
public:

  typedef std::vector<Expression>::const_iterator ConstIteratorType;

  /// Default construct and Expression, whose type in NoneType
  Expression();

  /*! Construct an Expression with given Atom as head an empty tail
    \param atom the atom to make the head
  */
  Expression(const Atom & a);

  /// deep-copy construct an expression (recursive)
  Expression(const Expression & a);

  // List Constructor
  Expression(const std::vector<Expression> & list);

  // Lambda Constructor
  Expression(const std::vector<Expression> & args, const Atom & a);

  // Discrete-Plot Constructor
  Expression(const std::vector<Expression> & args, std::string & str);

  /// deep-copy assign an expression  (recursive)
  Expression & operator=(const Expression & a);

  /// return a reference to the head Atom
  Atom & head();

  /// return a const-reference to the head Atom
  const Atom & head() const;

  /// append Atom to tail of the expression
  void append(const Atom & a);

  /// return a pointer to the last expression in the tail, or nullptr
  Expression * tail();

  /// return a const-iterator to the beginning of tail
  ConstIteratorType tailConstBegin() const noexcept;

  /// return a const-iterator to the tail end
  ConstIteratorType tailConstEnd() const noexcept;

  /// convienience member to determine if head atom is a number
  bool isHeadNumber() const noexcept;

  /// convienience member to determine if head atom is a symbol
  bool isHeadSymbol() const noexcept;

  /// convenience member to determine if head atom is a complex
  bool isHeadComplex() const noexcept;

  /// convenience member to determine if head atom is a list
  bool isHeadList() const noexcept;

  /// convenience member to determine if head atom is a string
  bool isHeadString() const noexcept;

  /// Evaluate expression using a post-order traversal (recursive)
  Expression eval(Environment & env);

  /// equality comparison for two expressions (recursive)
  bool operator==(const Expression & exp) const noexcept;

  std::string makeString() const noexcept;

  std::vector<Expression> makeTail() const noexcept;

  bool isPoint() const noexcept;

  bool isLine() const noexcept;
  
  bool isText() const noexcept;

  bool isDiscrete() const noexcept;

  double getSize() const noexcept;

  double getThickness() const noexcept;

  Expression getPosition() const noexcept;

  double getTextScale() const noexcept;
  
  double getTextRotation() const noexcept;
  
private:

  // the head of the expression
  Atom m_head;

  // the tail list is expressed as a vector for access efficiency
  // and cache coherence, at the cost of wasted memory.
  std::vector<Expression> m_tail;

  // convenience typedef
  typedef std::vector<Expression>::iterator IteratorType;

  // typedef for List
  typedef std::vector<Expression>::iterator ListType;
  
  // internal helper methods
  Expression set_property(Environment & env);
  Expression get_property(Environment & env);
  Expression handle_lookup(const Atom & head, const Environment & env);
  Expression handle_define(Environment & env);
  Expression handle_begin(Environment & env);
  Expression handle_lambda(Environment & env);
  Expression handle_apply(Environment & env);
  Expression handle_map(Environment & env);
  Expression discrete_plot(Environment & env);
  Expression continuous_plot(Environment & env);

  std::map<std::string, Expression> propertymap;
};

/// Render expression to output stream
std::ostream & operator<<(std::ostream & out, const Expression & exp);

/// inequality comparison for two expressions (recursive)
bool operator!=(const Expression & left, const Expression & right) noexcept;
  
#endif

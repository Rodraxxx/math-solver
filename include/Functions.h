#ifndef FUNCTIONS_H
#define FUNCTIONS_H


/*
  Main sources:

  https://en.wikipedia.org/wiki/List_of_mathematical_abbreviations
  https://amath.innolan.net/
*/

enum class Functions {
  // NULL OPERATOR

  NULLOPERATOR,
  // IGNORE OPERATOR

  IDENTITY, // x -> x 
  //BASIC ARITHMETIC OPERATORS

  ADDITION,
  SUBTRACTION,
  MULTIPLICATION,
  DIVISION,
  EXPONENTIATION,
  PSQRT, // PRINCIPAL SQUARE ROOT
  SQRT,
  PROOT, // PRINCIPAL ROOT
  ROOT,
  LOG,
  ABS,
  // SPECIAL ARITHMETIC OPERATIONS

  TETRATION,
  SROOT, // https://en.wikipedia.org/wiki/Tetration#Inverse_operations
  NTHSROOT,
  SLOG,
  // RELATIONAL OPERATORS

  EQUALS,
  GT, // GREATER THAN
  LT, // LOWER THAN
  GE, // GREATER OR EQUAL TO
  LE, // LOWER OR EQUAL TO
  // ELLIPTIC TRIG FUNCTIONS

  SIN,
  COS,
  TAN,
  CSC,
  SEC,
  COT,
  ARCSIN,
  ARCCOS,
  ARCTAN,
  ARCCSC,
  ARCSEC,
  ARCCOT,
  // HYPERBOLIC TRIG FUNCTIONS

  SINH,
  COSH,
  TANH,
  CSCH,
  SECH,
  COTH,
  ARCSINH,
  ARCCOSH,
  ARCTANH,
  ARCCSCH,
  ARCSECH,
  ARCCOTH,
  // SPHERICAL TRIG FUNCTIONS

  VER, // VERSINE
  CVS, // COVERSINE
  VCS, // VERCOSINE
  CVC, // COVERCOSINE
  HV, // HAVERSINE
  HCV, // HACOVERSINE
  HVC, // HAVERCOSINE
  HCC, // HACOVERCOSINE
  SV, // SEMIVERSINE
  SCV, // SEMICOVERSINE
  ARCVER, // ARCVERSINE
  ARCVCS, // ARCVERCOSINE
  ARCCVS, // ARCCOVERSINE
  ARCCVC, // ARCCOVERCOSINE
  ARCHV, // ARCHAVERSINE
  ARCHVC, // ARCHAVERCOSINE
  ARCHCV, // ARCHACOVERSINE
  ARCHCC, // ARCHACOVERCOSINE
  // SPECIAL TRIG FUNCTIONS

  ATAN2,
  SINC,
  GD, // https://en.wikipedia.org/wiki/Gudermannian_function
  CRD, // CHORD
  ACRD, // ANTICHORD / INVERSE CHORD
  CCD, // COCHORD
  ACCD, // ANTICOCHORD

  // ELLIPTIC INTEGRAL TRIG FUNCTIONS

  SI, // https://en.wikipedia.org/wiki/Trigonometric_integral
  si,
  CIN,
  CI,
  // HYPERBOLIC INTEGRAL TRIG FUNCTIONS

  SHI,
  CHI,
  // DEPRECATED TRIG FUNCTIONS

  CIS, // https://en.wikipedia.org/wiki/Cis_(mathematics)
  ARCCIS,
  CAS, // https://en.wikipedia.org/wiki/Hartley_transform#cas
  EXS, // EXSECANT https://en.wikipedia.org/wiki/Exsecant
  EXC, // EXCOSEC
  ARCEXS, // ARC EXSECANT
  ARCEXCS, // ARC EXCOSECANT
  // https://en.wikipedia.org/wiki/Chord_(geometry)
  // COMBINATORICS & NUMBER THEORY

  GCD,
  LCM,
  MODULO,
  FACTORIAL,
  PERM,
  CHOOSE,
  CEIL,
  FLOOR,
  FRAC,
  ROUND,
  SIGN,
  FIB, // FIBBONACCI
  PHI, // EULER TOTIENT FUNCTION https://en.wikipedia.org/wiki/Euler%27s_totient_function
  PI, // PRIME COUNTING FUNCTION https://en.wikipedia.org/wiki/Prime-counting_function
  SIGMA, // DIVISOR FUNCTION https://en.wikipedia.org/wiki/Divisor_function
  PARTITION, // PARTITION FUNCTION https://en.wikipedia.org/wiki/Partition_function_(number_theory)
  MU, // MÖBIUS FUNCTION https://en.wikipedia.org/wiki/M%C3%B6bius_function
  

  // SPECIAL NUMBER THEORY

  FKSTIRLING, // https://en.wikipedia.org/wiki/Stirling_number first kind stirling
  UNGSTIRLING, // unsigned first kind stirling
  SKSTIRLING, // second kind stirling
  PRIMORIAL,

  // PIECEWISE SPECIAL FUNCTIONS
  DDELTA, // DIRAC DELTA https://en.wikipedia.org/wiki/Dirac_delta_function
  HEAVISIDE, // https://en.wikipedia.org/wiki/Heaviside_step_function
};

#endif
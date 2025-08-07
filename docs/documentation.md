MATHSOLVER uses basic LaTeX math mode notation to express inputs, however it presents some limitations and implementations:

Spaces are ignored

Typographic comands are not allowed

Most functions can be expressed in MATHSOLVER the same way they're expressed in LaTeX, however, there are some exceptions:
  Intervals: Don't express intervals literally or the compiler will break. Instead, use the \interval command, followed by your interval expressed normally (Using (parentheses) for open ends and [square brackets] for closed ends): \interval'Your interval here'
    e. g. \interval[0,1), \interval(-\infty,\infty)
  
  Vectors: \vec('Your','parameters','here')
    e. g. \vec(0,1)
  
  Matrices: \matrix('x11','x21','x31';'x12','x22','x32')

  Absolute values: Placing |expressions between bars| is invalid to express absolute values. Use \abs instead
    e. g. \abs(x-1)>0 \iff x \nin
  
  Sets: Don't use {curly braces} directly to express sets, instead use the \set() command followed by the content of your set. If you wish to express your set using set builder notation use \setbuild('Variables and domain','Condition')
    e. g. \set(1,2,3), \setbuilder(n\in \nat, n/2 \in \nat)

Operators involving scripting follow a strict set of rules:
  The scripting combination is one of three ways:
    \operator{content}
    \operator_{subscript}{content}
    \operator_{subscript}^{superscript}{content}
  Which means that if the subscript exists, it must be declared before the superscript if it exists, and both of these must be declared before the content

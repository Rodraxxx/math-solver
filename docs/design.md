Parser:

Starts by converting the input into tokens.

Some tokens have a special hard-coded behaviour:
\sqrt checks for the left bracket symbol to account for the top indexing which indicates the root.
  Its ASTLeaf value is modified in the following way:
0 by default -> One argument (e.g. \sqrt{x}) 
1 if a left bracket is found right after -> Two arguments (The first being the root) (e.g. \sqrt[x]{x})

Large operators like \sum and \prod use the following values:
0 -> \operator{content} (e.g. \int{x\d{x}})
1 -> \operator_{subscript}{content} (e.g. \int_{\real}{x\d{x}})
2 -> \operator_{subscript}^{superscript}{content} (e.g. \lim_{n\to \infty}{\int_{-n}^{n}{s\d{x}}})

Intervals save their edge types in the functions property of the ast itself.

Uses the shunting-yard algorithm to get the tokens into a RPN

AI: Graphormer simplificado / GNN + Transformer híbrido

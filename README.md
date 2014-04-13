## Recursive Descent Parser and more.
Parses arithmetic expression and makes variable assignments. Use variables in math.

####How it works
Takes shell input and tokenizes it by dividing it into an array of strings. From this array, it builds a binary tree that represents the order of operations. Building a recursive descent parser begins with a contex-free-grammar. 
Our grammar is this:
* Expression = Term | Term + Term ... | Term - Term ...
* Term = Terminal | Terminal * Terminal ... | Terminal * Terminal ...
* Terminal = variable | number

Each of these represents a function in the algorithm, that mutually recur "down" to the leaf nodes, or terminals. Once the binary tree is built, we execute it by traversing it depth-first and post-order (with regard to when the parent node is visited).

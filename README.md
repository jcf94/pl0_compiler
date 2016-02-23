# PL/0_compiler

A Compiler for PL/0 Language written by C++.

PL/0 is a subset of Pascal.

# Usage

Put PL/0 code in `***.pl0`.

```bash
$ make
$ ./main
```

# 说明

这是一份编译原理的课程设计作业。

原始的PL/0支持的基本文法如下：

- Program → Block .

- Block → [ConstDecl][VarDecl][ProcDecl] Stmt

- ConstDecl → const ConstDef {, ConstDef} ;

- ConstDef → ident = number

- VarDecl → var ident {, ident} ;

- ProcDecl → procedure ident ; Block ; {procedure ident ; Block ;}

- Stmt → ident := Exp | call ident | begin Stmt {; Stmt} end | if Cond then Stmt | while Cond do Stmt | ε

- Cond → odd Exp | Exp RelOp Exp

- RelOp → = | <> | < | > | <= | >=

- Exp → [+ | − ] Term {+ Term | − Term}

- Term → Factor {∗ Factor | / Factor}

- Factor → ident | number | ( Exp )

目标是将其拓展，以支持如下文法：

- Program → Block .

- Block → [ConstDecl][TypeDecl][VarDecl][FuncDecl] begin Stmt {; Stmt } end

- ConstDecl → const ConstDef ; {ConstDef ;}

- ConstDef → ident = number ;

- TypeDecl → type TypeDef {TypeDef }

- TypeDef → ident = TypeExp ;

- TypeExp → integer | real | Boolean | array ‘[’number .. number‘]’ of TypeExp

- VarDecl → var VarDec {VarDec }

- VarDec → ident {, ident} : Type ;

- Type → integer | real | Boolean | ident FuncDecl → FuncDec { FuncDec }

- FuncDec → procedure ident [ ( ForParal ) ]; Block ; | function ident [ ( ForParal ) ] : Type ; Block ;

- ForParal → ident : Type {; ident : Type }

- Stmt → IdentRef := Exp | if Exp then Stmt | if Exp then Stmt else Stmt | begin Stmt {; Stmt } end | while Exp do Stmt | exit | ε | call ident [ ( ActParal ) ] | write ( Exp {, Exp } ) | read (IdentRef {, IdentRef } )

- IdentRef → ident [ ‘[’Exp‘]’ { ‘[’Exp‘]’ } ]

- ActParal → Exp {, Exp }

- Exp → SimpExp RelOp SimpExp | SimpExp

- RelOp →  = | <> | < | > | <= | >=

- SimpExp → [+ | - ] Term {+ Term | - Term | or Term}

- Term → Factor {* Factor | / Factor | div Factor | mod Factor | and Factor}

- Factor →  IdentRef | number | ( Exp ) | not Factor | ident [ ( ActParal ) ] | odd (SimpExp) | true | false
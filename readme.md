# File format

Input files consist of expressions and rules.

## Expressions

An expression looks like:

```
expression some-name
    [a-bA-Y09]lyoun(kkol)?
```

`expression` is a keyword that indicates this is an expression. It must start in
the first column. `some-name` is the name you want to refer to the expression by
later. It can contain anything but whitespace.

Following the name is a regular expression. The expression must start on the
next line, and be indented with either 4 spaces or 1 tab. The expression
continues to the end of the line.

The expression currently uses the ECMAScript format. This won't be supported
forever. In particular, backreferences will be removed long-term.

Expressions cannot start with a literal space. If you need to start with a
space, you can put it in parenthesis, e.g. `( )expression`.

Expressions cannot start with a literal tab. You can escape leading tabs as
`\t` instead.

Expressions cannot contain `#` at all. You can use a unicode escape if needed.

## Rules

A rule looks like:

```
rule what-to-match
    action1
    action2 parameter
    action3
    &c.
```

`rule` is a keyword that indicates this is a rule. It must start in the first
column. `what-to-match` is the name of an expression. The rule will only apply
when this expression matches. Expressions can be referenced in a rule before
they're defined.

Following is a list of actions. Each one must be on its own line, indented. If
an action has a parameter, that must be on the same line as the action.

Allowed actions include:

- `produce-nothing`    : The lexer won't produce any tokens when applying this
                         rule.
- `produce token-name` : The lexer will produce a token named `token-name` when
                         applying this rule.
`++line`, `line++`     : Increment the current line number.
`--line`, `line--`     : Decrement the current line number.

## The generated lexer

The generated lexer will include a `.hpp` and `.cpp` file, named after the input
file and placed in the same folder. The lexer itself will also be named after
the input file. For example, if your lexer is described in `lexer.plex`, then
the generated lexer will be in `lexer.hpp` and `lexer.cpp` and named `lexer`.

The lexer API includes the following members. Members assume your lexer is named
`lexer`.

- `enum TokenType`:
	The token types produced by the lexer. These correspond with the names used
	in rules' `produce` actions. Two special types will always be present, even
	if not used in the lexer description. `PLEXIGLASS_NO_MATCH_TOKEN` is
	generated when the lexer fails to match any input, and `PLEXIGLASS_EOF` is
	generated when no input is available.
- `std::strin ToString(TokenType type)`:
	A utility function that converts a TokenType to a string containing its
	name. e.g. `PLEXIGLASS_EOF` will be converted to `"PLEXIGLASS_EOF"`.
- `lexer::lexer(std::string path)`:
	Constructs the lexer. `path` is the path to the lexer's input file.
- `lexer::PeekToken()`:
	Retrieve the next token's `TokenType` without modifying the lexer.
- `lexer::PeekText()`:
	Retrieve the text matched by the next token without modifying the lexer.
- `lexer::PeekLine()`:
	Retrieve the line number the token started on without modifying the lexer.
- `lexer::Shift()`:
	Advance the lexer to the next token.

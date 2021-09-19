# File format

Input files consist of expressions, patterns and rules.

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

The expression's syntax has not yet been decided. Right now it's just whatever
the underlying regex engine will accept. Backreferences won't be supported long
term.

Expressions cannot start with a literal space. If you need to start with a
space, you can put it in parenthesis, e.g. `( )expression`.

Expressions cannot start with a literal tab. You can escape leading tabs as
`\t` instead.

Expressions cannot contain `#` at all. You can use a unicode escape if needed.

## Patterns

A pattern looks like:

```
pattern some-name
      abcde
    | fghij klmno pqrs
    | wxyz
```

`pattern` is a keyword that indicates this is a pattern. It must start in the
first column. `some-name` is the name you want to refer to the pattern by later.
It can contain anything but whitespace.

Following the name is a regular grammar. This grammar must start on the next
line, and be indented like a regular expression. Options can be separated by a
`|`. Each piece is the name of a regular expression, as defined above.
Expressions can be referenced in a pattern before they're defined.

Pattern syntax will likely follow EBNF.


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
column. `what-to-match` is the name of a pattern or expression. The rule will
only apply when this pattern/expression matches. Patterns and expressions can be
referenced in a rule before they're defined.

Following is a list of actions. Each one must be on its own line, indented. If
an action has a parameter, that must be on the same line as the action.

Allowed actions include:

- `produce-nothing` : The lexer won't produce any tokens.
- `produce token-name` : The lexer will produce a token named `token-name`.
- `rewind` : The lexer will leave the text for this token behind to be matched
             by the next iteration. This will cause an infinite loop unless you
             also include a `transition` action.
-`transition another-state` : The lexer will transition to `another-state`.
                              States are currently unsupported since the syntax
                              is undecided.
`++line`, `line++` : Increment the current line number.
`--line`, `line--` : Decrement the current line number.

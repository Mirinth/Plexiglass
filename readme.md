# File format

Input files consist of expressions, patterns and rules.

## Expressions

An expression looks like:

```
expression some-name
	[a-bA-Y09]lyoun(kkol)?
```

`expression` is a keyword that indicates this is an expression. It must occur
in the first column. `some-name` is the name you want to refer to the
expression by later. It can contain letters, numbers, hyphens and underscores.

Following the name is a regular expression. This can be on the same line or the
following line. If on the following line, it should be indented. Leading
whitespace will be discarded. The expression will continue to the end of the
line.

The expression's syntax has not yet been decided. Right now it's just whatever
the underlying regex engine will accept. Backreferences won't be supported long
term.


## Patterns

A pattern looks like:

```
pattern some-name
	  abcde
	| fghij klmno pqrs
	| wxyz
```

`pattern` is a keyword that indicates this is a pattern. It must occur in the
first column. `some-name` is the name you want to refer to the pattern by later.
It can contain letters, numbers, hyphens, and underscores.

Following the name is a regular grammar. This can start on the same line as the
name, but it's recommended to put it afterward. Options can be separated by a
`|`.

Pattern syntax will likely follow EBNF.


## Rules

A rule looks like:

```
rule what-to-match
	action1
	action2
	action3
	&c.
```

`rule` is a keyword that indicates this is a rule. It must occur in the first
column. `what-to-match` is the name of a pattern or expression. The rule will
only apply if this pattern/expression matches.

Following is a list of actions. These can all be on the same line as the start,
but it's preferred to have them each on their own lines.

Allowed actions include:

`produce-nothing` : The lexer won't produce any tokens.
`produce token-name` : The lexer will produce a token named `token-name`.
`rewind` : The lexer will leave the text for this token behind to be matched by
           next iteration.
`transition another-state` : The lexer will transition to `another-state`.
                             States are currently unsupported since the syntax
							 is undecided.
`+line n`, `line+ n`, `-line n`, `line- n`: n will be added to or subtracted
                                            from the current line number.
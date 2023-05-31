# db
a simple text-based db with CLI for project management, etc.

## Ideas

Create a sub-db of `main` called `undone` with a subset of data in table format

`db --branch undone --filter ".done F ==" --fmt table`

Commit changes made manually in `undone` back into `main`

`db --merge undone`

## Formats

undone.txt
```
title      milestone state       notes         origin-date
do thingee stuff     unscheduled we're screwed 5/5/2023
```

main.txt
```
record {
guid=0
title=do thingee
milestone=stuff
state=unscheduled
notes=we're screwed
origin-date=5/5/2023
}
```

presave.txt
```
list state {
done
unscheduled
}
now origin-date
unique title
rename milestone "stuff" "newStuff"
```

filter options BNF-ish
```
<filter> ::== <particle> <filter>
            | e
<particle> ::== .fieldName
              | "string-literal"
              | <comp-operators>
              | <log-operators>
<comp-operators> ::== '=='
<log-operators> ::== '&&'
                   | '||'
                   | '!'
```

sort options
```
<sort-expr> ::== fieldName <enum-sort> <sort-expr'>
               | e
<sort-expr'> ::== ',' <sort-expr>
<enum-sort> ::== '[' <value-list> ']'
               | e
```

view options (table)
```
headingName=fieldName[maxWidth]
```
NOTE: view options must be persistent so tables can be read!

## Todo
--core--
[x] unmap command
[x] map command

--up rules--
[ ] viewSpec: default<random>

--core--
[ ] check command

--check rules--
[ ] viewSpec: unique
[ ] viewSpec: enum<a,b,c>

--tedious; known--
[ ] viewSpec: filter: <<RPN>>
[ ] viewSpec: sort: a,-b,c
[ ] viewSpec: cols: a,b,c

--more of the same--
[ ] top-level merge verb
[ ] viewSpec: inherit
[ ] viewSpec: checkbox field when value
[ ] table view: viewSpec
[ ] table view: load
[ ] table view: save
[ ] viewSpec: default
[ ] viewSpec: default<now>

--polish--
[ ] top-level map verb

--deferred--
[ ] composite multi-view
[ ] multiline strings?

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

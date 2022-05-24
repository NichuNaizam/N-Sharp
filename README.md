# N-Sharp - A fully custom scripting language written in C++

N-Sharp, a fully custom interpreted and dynamically typed scripting language with OpenGL backend written in C++ mainly focusing on Game development ease without Game Engines

# Syntax

**Declaring a variable**
```
set variableName = 123
set variableName = 123.0f
set variableName = "Hello"
set variableName = true
set variableName = function()
```
**Declaring a global variable** (will not get deleted when the function ends)
```
function testFunction()
{
    global variableName = 123
}
```
**Declaring a function**
```
function add(num1, num2)
{
    set sum = num1 + num2
    return sum
}
```

# Functions

`NS.System.PrintLine(arguments...)` <br>
`Returns NULL` <br>
Print something to stdout <br>
`NS.System.Print(arguments...)` <br>
`Returns NULL` <br>
Print something to stdout without newline <br>
`NS.System.ReadLine()` <br>
`Returns string` <br>
Reads a full line from stdin <br>

# Sample program

```
NS.System.Print("Please enter your name: ")
set name = NS.System.ReadLine()
NS.System.PrintLine(name, ", Welcome to N-Sharp!")
```

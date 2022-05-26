# N-Sharp - A fully custom scripting language written in C++

N-Sharp, a fully custom interpreted and dynamically typed scripting language with OpenGL backend written in C++ mainly focused on Game development ease without Game Engines

# Syntax

**Declaring a variable**

```
// int
var variableName = 123

// float
var variableName = 123.0f

// string
var variableName = "Hello"

// bool
var variableName = true

// Reading from function
var variableName = function()
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
    var sum = num1 + num2
    return sum
}
```

# Functions

## Console

`NS.System.PrintLine(arguments...)` <br>
`Returns NULL` <br>
Print something to stdout <br>
`NS.System.Print(arguments...)` <br>
`Returns NULL` <br>
Print something to stdout without newline <br>
`NS.System.ReadLine()` <br>
`Returns string` <br>
Reads a full line from stdin <br>
`NS.System.SetTextColor(colorName)` <br>
`Returns NULL` <br>
Sets the text color to the colorName <br>

## Graphics

`NS.Graphics.Init(width, height, title)` <br>
`Returns NULL` <br>
Initializes the OpenGL context and creates a window <br>
`NS.Graphics.WindowShouldClose()` <br>
`Returns bool` <br>
Checks if the window should exit and returns it <br>
`NS.Graphics.Begin()` <br>
`Returns NULL` <br>
Prepares the graphics engine for rendering <br>
`NS.Graphics.End()` <br>
`returns NULL` <br>
Draws the final picture onto the window <br>

# Sample Programs

## Sample Console Program

```
// Provides the input(question) function
// which is better than NS.System.ReadLine()
using NS.Utility

// Declaring variables
var name = ""
var color = ""

// Reading data
name = input("Please enter your name: ")
color = input("Please enter your favorite color: ")

// Setting text color
NS.System.SetTextColor(color)

// Printing out name
NS.System.PrintLine(name, ", Welcome to N-Sharp.")
```

## Sample Window Program

```
var windowWidth = 800
var windowHeight = 600
var windowTitle = "Example Window"

function update()
{

}

function render()
{

}

function main()
{
    NS.Graphics.Init(windowWidth, windowHeight, windowTitle)

    while (NS.Graphics.WindowShouldClose() == false)
    {
        update()

        NS.Graphics.Begin()
        render()
        NS.Graphics.End()
    }
}

main()
```

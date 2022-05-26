# N-Sharp - A fully custom scripting language written in C++

N-Sharp, a fully custom interpreted and dynamically typed scripting language with OpenGL backend written in C++ mainly focused on Game development ease without Game Engines

# Syntax

**Declaring a variable**

```
// int
var variableName = 123

// float
var variableName = 123.0

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

## System

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
`NS.System.GetTime()` <br>
`Returns float` <br>
Returns the uptime of the script in milliseconds <br>

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
// Declaring variables
var windowWidth = 800
var windowHeight = 600
var windowTitle = "Test"
var fps = 0
var time = 0.0
var lastTick = NS.System.GetTime()

// Update function
function update(deltaTime)
{
	time = time + deltaTime

	if (time >= 1)
	{
		NS.System.PrintLine("FPS: ", fps)
		fps = 0
		time = 0
	}

	fps = fps + 1
}

// Render function
function render()
{
}

// Main function
function main()
{
	// Initializing the OpenGL Context and creating a window
	NS.Graphics.Init(windowWidth, windowHeight, windowTitle)

	// Game Loop until window closes
	while (!NS.Graphics.WindowShouldClose())
	{
		// Delta Time calculation
		var now = NS.System.GetTime()
		var deltaTime = now - lastTick
		lastTick = now

		// Calling the update function
		update(deltaTime)

		// Preparing the graphics engine for rendering
		NS.Graphics.Begin()
		// Calling the render function
		render()
		// Telling the graphics engine that we have done our drawing
		NS.Graphics.End()
	}
}

// Calling the main function
main()
```

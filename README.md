# N-Sharp - A fully custom scripting language written in C++

N-Sharp, a fully custom interpreted and dynamically typed scripting language with OpenGL backend written in C++ mainly focused on Game development ease without Game Engines

# Syntax

**Including another N-Sharp file**

```
using NS.Graphics
using NS.Vec2
using NS.Physics
using NS.Input
// ect
// using path.to.file.nsfile
```

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

**DO THIS FIRST** <br>
`using NS.Graphics` <br>
Includes the N-Sharp Graphics class <br>

`Graphics.Init(width, height, title)` <br>
`Returns NULL` <br>
Initializes the OpenGL context and creates a window <br>
`Graphics.WindowShouldClose()` <br>
`Returns bool` <br>
Checks if the window should exit and returns it <br>
`Graphics.Begin()` <br>
`Returns NULL` <br>
Prepares the graphics engine for rendering <br>
`Graphics.Clear(r, g, b)` <br>
`Returns NULL` <br>
Clears the screen with the rgb color given as parameter
`Graphics.End()` <br>
`returns NULL` <br>
Draws the final picture onto the window <br>

## Input

**DO THIS FIRST** <br>
`using NS.Input` <br>
Includes the N-Sharp Input class <br>

`Input.IsKeyDown(keyname)` <br>
`Returns bool` <br>
Returns true if the key is being held down <br>

`Input.IsKeyPressed(keyname)` <br>
`Returns bool` <br>
Return true during the frame the key was pressed <br>

`Input.IsMouseButtonDown(buttoncode)` <br>
`Returns bool` <br>
Returns true when the mouse button is being held down <br>

`Input.GetMousePosition()` <br>
`Returns vec2` <br>
Returns the mouse position as Vector2 <br>

`Input.GetMouseScroll()` <br>
`Returns vec2` <br>
Returns the mouse scroll as Vector2 <br>

## Vector2

**DO THIS FIRST** <br>
`using NS.Vec2` <br>
Includes the N-Sharp Vec2 class <br>

`Vec2.Create(x, y)` <br>
`Returns vec2` <br>
Creates a new Vector2 with the given x and y

`Vec2.GetX(vec2)` <br>
`Returns float` <br>
Returns the current x value of the given vector <br>

`Vec2.GetY(vec2)` <br>
`Returns float` <br>
Returns the current y value of the given vector <br>

`Vec2.Compare(vec1, vec2)` <br>
`Returns bool` <br>
Compares two of the given vectors and returns true if it contains the same value <br>

`Vec2.Add(vec1, vec2)` <br>
`Returns vec2` <br>
Returns the sum of vec1 and vec2

## Physics

**DO THIS FIRST** <br>
`using NS.Physics` <br>
Includes the N-Sharp Physics class <br>

`Physics.DoesCollide(position1, scale1, position2, scale2)` <br>
`Returns bool` <br>
Checks for collision between the two rects

# Sample Programs

## Sample Console Program

```
// Declaring variables
var name = ""
var color = ""

// Reading data
NS.System.PrintLine("Please enter your name: ")
name = NS.System.ReadLine()
NS.System.PrintLine("Please enter your favorite color: ")
color = NS.System.ReadLine()

// Setting text color
NS.System.SetTextColor(color)

// Printing out name
NS.System.PrintLine(name, ", Welcome to N-Sharp.")
```

## Sample Window Program

```
// Including the N-Sharp Graphics class
using NS.Graphics

// Declaring variables
var windowWidth = 800
var windowHeight = 600
var windowTitle = "Test"

// Main function
function main()
{
	// Initializing the OpenGL Context and creating a window
	Graphics.Init(windowWidth, windowHeight, windowTitle)

	// Game Loop until window closes
	while (!Graphics.WindowShouldClose())
	{
		// Preparing the graphics engine for rendering
		Graphics.Begin()
		// Clearing the screen with white color
		Graphics.Clear(1, 1, 1)
		
		// Telling the graphics engine that we have done our drawing
		Graphics.End()
	}
}

// Calling the main function
main()
```

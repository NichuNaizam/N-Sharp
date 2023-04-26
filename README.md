<p align="center">
    <img src="https://github.com/NichuNaizam/N-Sharp/blob/master/N-Sharp/res/N%23.png" width="120px">
</p>

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

**DO THIS FIRST** <br>
`using NS.System` <br>
Includes the N-Sharp System class <br>

`System.PrintLine(arguments...)` <br>
`Returns NULL` <br>
Print something to stdout <br>

`System.Print(arguments...)` <br>
`Returns NULL` <br>
Print something to stdout without newline <br>

`System.ReadLine()` <br>
`Returns string` <br>
Reads a full line from stdin <br>

`System.SetTextColor(colorName)` <br>
`Returns NULL` <br>
Sets the text color to the colorName <br>

`System.Exit()` <br>
`Returns NULL` <br>
Exits the N# Interpreter <br>

`System.GetTime()` <br>
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
Clears the screen with the rgb color given as parameter <br>

`Graphics.LoadTexture(path)` <br>
`Returns Texture` <br>
Loads a texture from the path into the memory for rendering <br>

`Graphics.RenderTexture(texture, position, rotation, scale)` <br>
`Returns NULL` <br>
Renders the loaded texture into the screen with the given position, rotation and scale<br> 

`Graphics.End()` <br>
`Returns NULL` <br>
Draws the final picture onto the window <br>

`Graphics.Terminate()` <br>
`Returns NULL` <br>
Terminates the OpenGL Context and glfwTerminate is called<br>

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
Returns the sum of vec1 and vec2 <br>

`Vec2.Sub(vec1, vec2)` <br>
`Returns vec2` <br>
Returns the difference of vec1 and vec2 <br>

`Vec2.Multiply(vec1, vec2)` <br>
`Returns vec2` <br>
Returns the product of vec1 and vec2 <br>

`Vec2.Divide(vec1, vec2)` <br>
`Returns vec2` <br>
Returns the quotient of vec1 and vec2 <br>

## Physics

**DO THIS FIRST** <br>
`using NS.Physics` <br>
Includes the N-Sharp Physics class <br>

`Physics.DoesCollide(position1, scale1, position2, scale2)` <br>
`Returns bool` <br>
Checks for collision between the two rects

## Array

**DO THIS FIRST** <br>
`using NS.Array` <br>
Includes the N-Sharp Array Class <br>

`Array.Create()` <br>
`Returns Array` <br>
This method returns a new array object <br>

`Array.Push(array, value)` <br>
`Returns Array` <br>
This method returns a new array which contains the elements of the old array and the new object <br>

`Array.Get(array)` <br>
`Returns int` <br>
This method returns the size of the given array <br>

`Array.Pop(array)` <br>
`Returns Array` <br>
This method removes the last element of the given array and returns it <br>

`Array.Concat(array1, array2)` <br>
`Returns Array` <br>
This method joins both `array1` and `array2` into a new array and returns it <br>

`Array.Remove(array, index)` <br>
`Returns Array` <br>
This method removes the element in the `index` index <br>

`Array.Set(array, value, index)` <br>
`Returns Array` <br>
This method sets the `index` of the provided `array` to the `value` and returns it <br> 

# Sample Programs

## Sample Console Program

```
// Including our classes
using NS.System

// Declaring variables
var name = ""
var color = ""

// Reading data
System.PrintLine("Please enter your name: ")
name = System.ReadLine()
System.PrintLine("Please enter your favorite color: ")
color = System.ReadLine()

// Setting text color
System.SetTextColor(color)

// Printing out name
System.PrintLine(name, ", Welcome to N-Sharp.")
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

	// Terminating our OpenGL Context
	Graphics.Terminate()
}

// Calling the main function
main()
```

## Sample Texture Loading And Rendering
```
// Including The N-Sharp Graphics and Vector2 classes
using NS.Graphics
using NS.Vec2

// Setting our variables
var width = 800
var height = 600
var title = "Texture"

// Initializing our Window with the given width height and title
Graphics.Init(width, height, title)

// Loading the texture from "./res/N#.png"
var texture = Graphics.LoadTexture("./res/N#.png")

// The position we want our texture to be (For now its the top left corner (0, 0))
var pos = Vec2.Create(0, 0) // Position in x and y axis
var rotation = 0 // Rotation in degrees
var scale = Vec2.Create(100, 100) // scale in pixels (width, height) 100 by 100 pixels

// Game loop
while (!Graphics.WindowShouldClose())
{
	// Telling the OpenGL Graphics Engine that we are ready to draw to the screen
	Graphics.Begin()
	// Clearing the screen to black (black = (0, 0, 0), white = (1, 1, 1), etc...)
	Graphics.Clear(0, 0, 0)

	// Renders the texture onto the position, rotation and scale passed as arguments
	Graphics.RenderTexture(texture, pos, rotation, scale)

	// Telling the OpenGL Graphics Engine that we are done drawing to the screen
	Graphics.End()
}

// Terminating our OpenGL Context
Graphics.Terminate()
```
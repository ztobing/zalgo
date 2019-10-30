# zalgo

An interpreter for a basic, simple and easy to use scripting language.

A final project for Analysis of Algorithms class.

Made by
- David Amadeo - [GitHub](https://github.com/davidamadeo)
- Eris Suryaputra - [GitHub](https://github.com/GetLiberatedSon)
- Livander Surya
- Zefanya G - [GitHub](https://github.com/zefryuuko)

## Compilation
To compile, you need a C++ compiler installed on your machine. Once installed, clone this repository and run terminal in the cloned repository's directory. After that, run
```
cd src/
g++ main.cpp -o zalgo --std=c++11
```
Once the compilation process is finished, you can try it by typing `./zalgo` in the terminal. You should see this message
```
Usage: zalgo [filename]
```
If that message showed up, you are ready to go.

## Usage

To execute a script, run this command in the terminal
```
zalgo test_file.zal
```
Where `zalgo` is the program name and `test_file.zal` is the file that you want to execute.

## Language Rules
We implemented a few of core components typically found in scripting languages:
- Variable
- Input
- Print
- Comment
- Arithmetic operations
- If-else
- For loop
- While loop

### Variable
Variable will be assigned when a variable name is followed by equals and an expression.
```
variable_name = "Hello World!"
```
Where `variable_name` is the identifier and `"Hello World!"` is the value of the variable. A variable should start with a letter or an underscore, followed by alphanumeric characters and underscore. 

To use a variable, simply write the variable name.
```
var_2 = variable_name + " Hello!"
```

### Input
To receive user input at the program's runtime, the `input` keyword will handle it. To show a prompt for the user input, add an expression next to the input keyword.
```
name = "Enter your name: "
```
This will ask the user for an input and the input received will be assigned to the variable `name`.

### Print
The `print` keyword will print whatever is passed next to it.
```
message = "Hello, "
name = input "Enter your name: "

print message + mame + "!"
```
Output:
```
Enter your name: Zef
Hello, Zef!
```

### Comment
A comment will not be read by the interpreter. To make a comment, use `#` and every character from the hashtag until the end of the line will be ignored.
```
# print "Test"
print "Hello!"
```
Output:
```
Hello!
```


### Arithmetic Operations
Arithmetic expressions will be executed and returns a value to the previous call,
```
a = 10
b = a / 2
print b
```
Output:
```
5
```
Supported operators:
- Add
- Subtract
- Multiply
- Divide
- Unary +
- Unary -
- Parentheses

String multiplication is also supported, removing the need to create loops when you want to print the same string multiple times.
```
print "a" * 5
```
Output:
```
aaaaa
```

### If-else
You can use if block to do comparisons and do actions if the condition is correct.
```
num = input "Enter number between 1 to 10: "
if num > 10 then
   print "Number too large"
else if num < 1 then
   print "Number too small"
else
   print "OK!"
end if
```
Output:
```
Enter number between 1 to 10: 5
OK!
```

### For loop
For loop is used when you want to iterate n times based on the given range of numbers.
```
for i in 1 to 5
    print "*" * i
end for
```
Output:
```
*
**
***
****
*****
```
A range can also be backwards, for example `5 to 1`.

### While loop
While loop is used when you want to do the same actions until a certain condition is met.
```
name = ""
while name != "Zef" then
    input "Enter name: "
end while
print "OK!"
```
Output:
```
Enter name: Eris
Enter name: David
Enter name: Livander
Enter name: Zef
OK!
```

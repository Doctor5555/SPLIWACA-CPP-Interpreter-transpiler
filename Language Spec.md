# SPLIWACA

Standardised Pseudo-Lang Implemented With(out) A Cool Acronym

## Specification

### How to read this documentation

- Keywords marked with angular brackets (`<>`) are PLACEHOLDERS and should not be typed as-is and should not be typed with the `<>` included.
- Keywords or placeholders marked with brackets (`()`) are grouped. Don't include the brackets but be aware that these must come together if repeated.
- Keywords, placeholders, or groups within square brackets (`[]`) are OPTIONAL, this means that they can be left out. Don't include the brackets. These also act as grouping brackets.
- Placeholders and groups marked with an asterisk (`*`) are optionally repeatable
- Placeholders and groups marked with an ellipsis (`...`) within the name are greedy repeated arguments - they consume everything after them

### Commenting

- Single-line comment
  - Begin the comment with `//`. It lasts the rest of the line
- Multi-line comment
  - Begin the comment with `/*`. It lasts until a `*/`

### Whitespace

- Whitespace, other than between keywords and in strings, is ignored

### Keyword instructions/structures

- Statements
  - `INPUT <type> <variable name>`
    - Takes an input of type `<type>` and stores it in `<variable name>`
  - `OUTPUT <string...>`
    - Consumes the rest of the line, as plaintext. Words prefixed with `$` will be looked up as variables and, if the variable exists, replaced with the variable contents; otherwise, leaves the `$` and word unchanged. Prints the captured and formatted string to standard out
    - Aliases:
      - `PRINT <string...>`
  - `CREATE <type or class> [WITH <arg>*]`
    - Returns an instance of `<type or class>`, instantiated with `<arg>*`
  - `CAST <type> <expression>`
    - Casts the expression `<expression>` to the type `<type>` and returns it
  - `RAW <text...>`
    - Consumes the rest of the line, as plaintext. Words prefixed with `$` will be looked up as variables and, if the variable exists, replaced with the variable contents; otherwise, leaves the `$` and word unchanged. Returns the captured and formatted string
    - Aliases:
      - `PLAINTEXT <text...>`
  - `CALL <function> [WITH <arg>*]`
    - Calls `<function>` with `<arg>*`, if supplied. Equivalent to `<function>(<arg>*)` in most languages
  - `QUIT [value]`
    - Shuts down the interpreter and stops execution. If provided, returns with error code `[value]`
    - Aliases:
      - `EXIT [value]`
      - `STOP [value]`
  - `REQUIRE <interpreter or language>*`
    - Define that this program requires to be run in of the specified interpreters or language in order to work correctly; e.g. programs specific to the official Python interpreter may rely on implementation details, and programs specific to Python may rely on package names e.g. tkinter
- Structures
  - `IF <expression> DO <code> [ELSE IF <expression> DO <code>]* [ELSE DO <code>] END IF`
    - This is just a standard if-else structure
  - `FUNCTION <function name> [TAKES (<arg type> <arg name>)*] -> <return type> AS <code> RETURN <value>`
    - This defines a function
    - Aliases:
      - `FUNC <name> [TAKES (<type> <arg>)*] -> <rt> AS <code> RETURN <rv>`
  - `PROCEDURE <procedure name> [TAKES (<arg type> <arg name>)*] AS <code> END PROCEDURE`
    - This defines a procedure
    - Aliases:
      - `PROC <procedure name> [TAKES (<arg type> <arg name>)*] AS <code> END PROCEDURE`
  - `ANONF [<arg type> <arg name>]* -> <return type> AS <code> RETURN <return value>`
    - An anonymous function
    - Aliases:
      - `ANONFUNC [<arg type> <arg name>]* -> <return type> AS <code> RETURN <return value>`
      - `ANONFUNCTION [<arg type> <arg name>]* -> <return type> AS <code> RETURN <return value>`
      - `ANONYMOUSF [<arg type> <arg name>]* -> <return type> AS <code> RETURN <return value>`
      - `ANONYMOUSFUNC [<arg type> <arg name>]* -> <return type> AS <code> RETURN <return value>`
      - `ANONYMOUSFUNCTION [<arg type> <arg name>]* -> <return type> AS <code> RETURN <return value>`
  - `ANONP [<arg type> <arg name>]* -> <return type> AS <code> END PROCEDURE`
    - An anonymous procedure
    - Aliases:
      - `ANONPROC [<arg type> <arg name>]* -> <return type> AS <code> END PROCEDURE`
      - `ANONPROCEDURE [<arg type> <arg name>]* -> <return type> AS <code> END PROCEDURE`
      - `ANONYMOUSP [<arg type> <arg name>]* -> <return type> AS <code> END PROCEDURE`
      - `ANONYMOUSPROC [<arg type> <arg name>]* -> <return type> AS <code> END PROCEDURE`
      - `ANONYMOUSPROCEDURE [<arg type> <arg name>]* -> <return type> AS <code> END PROCEDURE`
  - `SET <variable name> TO <expression...>`
    - Sets `<variable name>` to `<expression...>`
  - `FOR <variable name> OF <iterable expression> DO <code> END FOR`
    - This is a for-each loop
  - `WHILE <expression> DO <code> END WHILE`
    - This is a while loop

### Literals, Types, and Constants

- Literals and types
  - `str`
    - This is a string. Strings can be instantiated either with literals (sequences of characters surrounded either by `'` or `"`) or by using the RAW keyword
    - Aliases:
      - `STR`
      - `Str`
      - `string`
      - `STRING`
      - `String`
  - `int`
    - This is an integer. Integers are instantiated with plain numbers anywhere except inside a string or plaintext line
    - Aliases:
      - `INT`
      - `Int`
      - `integer`
      - `INTEGER`
      - `Integer`
  - `float`
    - This is a floating point number; a number with decimal places
    - Aliases:
      - `FLOAT`
      - `Float`
      - `real`
      - `REAL`
      - `Real`
      - `number`
      - `NUMBER`
      - `Number`
  - `list`
    - Any number of values separated by a comma
    - Can be made up to 2D by separating 1D lists with `||`
    - Aliases:
      - `LIST`
      - `List`
      - `array`
      - `ARRAY`
      - `Array`
      - `tuple`
      - `TUPLE`
      - `Tuple`
  - `dict`
    - Any set of keys mapped to values
    - Instantiated with `<key>=<value>` pairs separated by `||`
    - Aliases:
      - `DICT`
      - `Dict`
      - `dictionary`
      - `DICTIONARY`
      - `Dictionary`
      - `map`
      - `MAP`
      - `Map`
      - `mapping`
      - `MAPPING`
      - `Mapping`
  - `bool`
    - Boolean value - `TRUE` or `FALSE`
    - Aliases:
      - `BOOL`
      - `Bool`
      - `boolean`
      - `BOOLEAN`
      - `Boolean`
  - `complex`
    - Complex number - any multiple of `i`
    - Instantiated with `Xi` where `X` represents any valid `int` or `float` literal
    - Aliases:
      - `COMPLEX`
      - `Complex`
- Constants
  - Static constants
    - `TRUE`
      - Represents the Boolean value True
      - Aliases:
        - `True`
        - `true`
    - `FALSE`
      - Represents the Boolean value False
      - Aliases:
        - `False`
        - `false`
    - `NULL`
      - Represents the empty value
      - Aliases:
        - `Null`
        - `null`
        - `NONE`
        - `None`
        - `none`
    - `INFINITY`
      - Represents positive infinity. This is a `float`
      - Aliases:
        - `Infinity`
        - `infinity`
        - `INF`
        - `Inf`
        - `inf`
    - `NAN`
      - Represents a non-number. This is a `float`
      - Aliases:
        - `NaN`
        - `nan`
  - State-based constants
    - `IS_MAIN_FILE`
      - `TRUE` if the current file is being run as the main file, `FALSE` if it's being imported
      - Aliases:
        - `I_AM_MAIN_FILE`
        - `MAIN_FILE`
        - `IS_MAIN`
        - `THIS_IS_MAIN_FILE`
    - `_INTERPRETER`
      - Acts as a module, allowing direct use of interpreter methods/functions.
      - Should be sandboxed i.e. unable to change program memory outside of that specified.
      - Can be used to directly make interpreter calls if the interpreter supports an `eval` method
      - It is a syntax error to use this without a `REQUIRE` statement

### Boolean Operators

- `IS`
  - Tests if two variables are the same object
  - Aliases:
    - `≣`
    - `≡`
- `NOT`
  - Negates a Boolean value
  - Aliases:
    - `!`
- `=`
  - Tests if two variables are equal
  - Aliases:
    - `==`
- `≠`
  - Tests if two variables are unequal
  - Aliases:
    - `=/=`
    - `!=`
- `≨`
  - Tests if the left variable is less than the right variable
  - Aliases:
    - `≱`
    - `<`
- `≩`
  - Tests if the left variable is more than the right variable
  - Aliases:
    - `≰`
    - `>`
- `≤`
  - Tests if the left variable is less than or equal to the right variable
  - Aliases:
    - `⩽`
    - `≦`
    - `≯`
    - `<=`
- `≥`
  - Tests if the left variable is more than or equal to the right variable
  - Aliases:
    - `⩾`
    - `≧`
    - `≮`
    - `>=`

### Calculation

- Decimal operations
  - `*`
    - Multiplies two numbers together
    - Also extends a string with itself if multiplied by an int
  - `**`
    - Takes the power of one number to another
  - `/`
    - Divides one number by another
  - `//`
    - Divides one number by another and returns an integer
  - `+`
    - Adds two numbers
    - Also used to concatenate strings
  - `-`
    - Subtracts one number from another
  - `%`
    - Modulo operator
- Bitwise operations
  - `^`
    - Bitwise XOR
  - `&`
    - Bitwise AND
  - `|`
    - Bitwise OR
  - `>>`
    - Arithmetic Shift Right
  - `<<`
    - Arithmetic Shift Left

### Iteration

- Strings:
  - Iteration should occur over each character in the string
- Lists:
  - Iteration should occur over each element of the list
- Dicts:
  - Iteration should occur over the keys of the dict

### Object Access

- Attribute `x` of `y`
  - `y.x`
- Item `x` of `y`
  - `y[x]`

### Miscellaneous

- If a variable is used before it's defined, the following failsafes will occur, in order:
  - The interpreter will attempt to import a SPLW module with the same name as the variable, then perform the operation again with the variable set as the imported module
  - The interpreter will attempt to import a native module with the same name as the variable, then perform the operation again with the variable set as the imported module
  - The interpreter will attempt to install and import a native module with the same name as the variable, then perform the operation again with the variable set as the imported module
  - The interpreter will treat the variable name as a bare word, and retry the operation
  - If nothing previous works, then an error will be thrown

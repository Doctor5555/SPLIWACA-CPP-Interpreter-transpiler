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

### Variable naming rules

- There are no reserved words, due to the structured nature of the language. This means it is perfectly valid to call your variables `OUTPUT` or `FALSE` if you want to
- Variable names may start with numbers only if they contain at least one letter; thus `2abc4` is valid but not `345`
- Variable naming should be `snake_case`, or if requiring a specific interpreter should follow the conventions of that language (this is a convention, not an enforced rule)

### Keyword instructions/structures

- Statements
  - `INPUT <type> <variable name>`
    - Takes an input of type `<type>` and stores it in `<variable name>`
    - For numeric types (`INT`, `FLOAT`, `COMPLEX`), allow prepending the type with a value specifier:
      - `POS` or `POSITIVE` to specify that the input must be positive
      - `NONNEG` or `NONNEGATIVE` to specify that the input must not be negative (positive or zero)
      - `NEG` or `NEGATIVE` to specify that it must be negative
      - `NONPOS` or `NONPOSITIVE` to specify that the input must not be positive (negative or zero)
      - `NONZERO` to specify that it must not be zero
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
    - Calls `<function>` with `<arg>*`, if supplied. Equivalent to `<function>(<arg>*)` in most languages. Arguments should be separated by commas (`CALL my_func WITH arg1, arg2, arg3`)
  - `QUIT [value]`
    - Shuts down the interpreter and stops execution. If provided, returns with error code `[value]`
    - Aliases:
      - `EXIT [value]`
      - `STOP [value]`
  - `REQUIRE <interpreter or language>*`
    - Define that this program requires to be run in of the specified interpreters or language in order to work correctly; e.g. programs specific to the official Python interpreter may rely on implementation details, and programs specific to Python may rely on package names e.g. tkinter
  - `INCREMENT <int or float variable>`
    - Adds 1 to the variable, as long as the variable is a numeric type
    - Aliases:
      - `INC <var>`
  - `DECREMENT <int or float variable>`
    - Subtracts 1 to the variable, as long as the variable is a numeric type
    - Aliases:
      - `DEC <var>`
  - `IMPORT <module> [AS <name>]`
    - Explicit import. Enables warnings on subsequent implicit imports (see Undefined Variables). If import fails, set to `NONE` and print a warning
  - `NOIMPORT`
    - Disables subsequent implicit imports entirely (see Undefined Variables)
  - `NOINSTALL`
    - Disables subsequent implicit system installations entirely (if a specific interpreter is required; see Undefined Variables)
  - `NOBARE`
    - Disables subsequent bare words failsafes entirely (see Undefined Variables)
  - `RETURN [value]`
    - Returns while inside a function or procedure.
      - If in a function, must pass a value of the correct type.
      - If in a procedure, must not pass a value
    - There must be at least one of these in every code path of a function to prevent the interpreter from reaching the `END FUNCTION` statement (if this occurs an error will be raised by the interpreter)
- Structures
  - `IF <expression> DO <code> [ELSE IF <expression> DO <code>]* [ELSE DO <code>] END IF`
    - This is just a standard if-else structure
  - `FUNCTION <function name> [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] RETURNS <return type> AS <code> END FUNCTION`
    - This defines a function
    - Aliases:
      - `FUNC <name> [TAKES (<type> <arg> [WITH DEFAULT <value>])*] RETURNS <rt> AS <code> END FUNCTION`
      - `FUNCTION <function name> [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] -> <return type> AS <code> END FUNCTION`
      - `FUNC <name> [<- (<type> <arg> [WITH DEFAULT <value>])*] -> <rt> AS <code> END FUNCTION`
  - `PROCEDURE <procedure name> [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
    - This defines a procedure
    - Aliases:
      - `PROC <procedure name> [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `PROCEDURE <procedure name> [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `PROC <procedure name> [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
  - `ANONF [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] RETURNS <return type> AS <code> END FUNCTION`
    - An anonymous function
    - Aliases:
      - `ANONFUNC [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] RETURNS <return type> AS <code> END FUNCTION`
      - `ANONFUNCTION [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] RETURNS <return type> AS <code> END FUNCTION`
      - `ANONYMOUSF [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] RETURNS <return type> AS <code> END FUNCTION`
      - `ANONYMOUSFUNC [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] RETURNS <return type> AS <code> END FUNCTION`
      - `ANONYMOUSFUNCTION [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] RETURNS <return type> AS <code> END FUNCTION`
      - `ANONF [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] -> <return type> AS <code> END FUNCTION`
      - `ANONFUNC [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] -> <return type> AS <code> END FUNCTION`
      - `ANONFUNCTION [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] -> <return type> AS <code> END FUNCTION`
      - `ANONYMOUSF [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] -> <return type> AS <code> END FUNCTION`
      - `ANONYMOUSFUNC [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] -> <return type> AS <code> END FUNCTION`
      - `ANONYMOUSFUNCTION [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] -> <return type> AS <code> END FUNCTION`
  - `ANONP [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
    - An anonymous procedure
    - Aliases:
      - `ANONPROC [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `ANONPROCEDURE [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `ANONYMOUSP [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `ANONYMOUSPROC [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `ANONYMOUSPROCEDURE [TAKES (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `ANONP [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `ANONPROC [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `ANONPROCEDURE [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `ANONYMOUSP [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `ANONYMOUSPROC [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
      - `ANONYMOUSPROCEDURE [<- (<arg type> <arg name> [WITH DEFAULT <value>])*] AS <code> END PROCEDURE`
  - `SET <variable name> TO <expression...>`
    - Sets `<variable name>` to `<expression...>`
  - `FOR <variable name> OF <iterable expression> DO <code> END FOR`
    - This is a for-each loop
  - `WHILE <expression> DO <code> END WHILE`
    - This is a while loop
  - `STRUCTURE <name> AS [<property type> <property name>]* END STRUCTURE`
    - This is a data structure
    - Aliases:
      - `STRUCT <name> AS [<property type> <property name>]* END STRUCTURE`

### Literals, Types, and Constants

- Literals and types
  - `STR`
    - This is a string. Strings can be instantiated either with literals (sequences of characters surrounded either by `'` or `"`) or by using the RAW keyword
    - Aliases:
      - `Str`
      - `str`
      - `STRING`
      - `String`
      - `string`
  - `INT`
    - This is an integer. Integers are instantiated with plain numbers anywhere except inside a string or plaintext line
      - Numeric literals may have underscores (`_`) in them as digit separators
    - Aliases:
      - `Int`
      - `int`
      - `INTEGER`
      - `Integer`
      - `integer`
  - `FLOAT`
    - This is a floating point number; a number with decimal places
      - Numeric literals may have underscores (`_`) in them as digit separators
    - Aliases:
      - `Float`
      - `float`
      - `REAL`
      - `Real`
      - `real`
      - `NUMBER`
      - `Number`
      - `number`
  - `LIST`
    - Any number of values separated by a comma
    - Can have their value consumption curbed by brackets (`()`)
      - e.g. a 3D list consisting of the indices multiplied together can be defined as follows:
        - `((0, 0, 0), (0, 0, 0), (0, 0, 0)), ((0, 0, 0), (0, 1, 2), (0, 2, 4)), ((0, 0, 0), (0, 2, 4), (0, 4, 8))`
    - Aliases:
      - `List`
      - `list`
      - `ARRAY`
      - `Array`
      - `array`
      - `TUPLE`
      - `Tuple`
      - `tuple`
  - `DICT`
    - Any set of keys mapped to values
    - Instantiated with `<key>:<value>` pairs separated by `,`
    - Should be wrapped by brackets (`()`)
    - Aliases:
      - `Dict`
      - `dict`
      - `DICTIONARY`
      - `Dictionary`
      - `dictionary`
      - `MAP`
      - `Map`
      - `map`
      - `MAPPING`
      - `Mapping`
      - `mapping`
  - `BOOL`
    - Boolean value - `TRUE` or `FALSE`
    - Aliases:
      - `Bool`
      - `bool`
      - `BOOLEAN`
      - `Boolean`
      - `boolean`
  - `COMPLEX`
    - Complex number - any multiple of `i`
    - Instantiated with `X + Yi` where `X` and `Y` represent any valid `int` or `float` literal. `X` may be omitted
      - Numeric literals may have underscores (`_`) in them as digit separators
    - Sequential comparison:
      - Compare `a.real` with `b.real`
        - If they are the same, the result of the comparison is determined by `a.imag` and `b.imag`
        - Otherwise the result of the comparison is determined by `a.real` and `b.real`
    - Aliases:
      - `Complex`
      - `complex`
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
        - `EMPTY`
        - `Empty`
        - `empty`
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
      - Should be sandboxed i.e. unable to interface with the program's own environment via the interpreter's memory interfaces.
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
- `AND`
  - Tests if both sides evaluate to `TRUE`
  - Aliases:
    - `∧`
    - `&&`
- `OR`
  - Tests if two variables both evaluate to `TRUE`
  - Aliases:
    - `∨`
    - `??`
- `=`
  - Tests if two variables are equal
  - Aliases:
    - `==`
    - `EQUALS`
- `≠`
  - Tests if two variables are unequal
  - Aliases:
    - `!=`
    - `=/=`
- `<`
  - Tests if the left variable is less than the right variable
  - Aliases:
    - `≨`
    - `≱`
- `>`
  - Tests if the left variable is more than the right variable
  - Aliases:
    - `≩`
    - `≰`
- `≤`
  - Tests if the left variable is less than or equal to the right variable
  - Aliases:
    - `⩽`
    - `≦`
    - `<=`
    - `≯`
- `≥`
  - Tests if the left variable is more than or equal to the right variable
  - Aliases:
    - `⩾`
    - `≧`
    - `>=`
    - `≮`

### Calculation

- Decimal operations
  - `*`
    - Multiplies two numbers together
    - Also extends a string with itself if multiplied by an int
  - `**`
    - Takes the power of one number to another
  - `/`
    - Divides one number by another
  - `DIVI`
    - Integer division operation (rounding any remainder towards `-INFINITY` i.e. `1 DIVI 2 = 0`)
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

- `STR`s:
  - Iteration should occur over each character in the object
- `LIST`s:
  - Iteration should occur over each element of the object
- `DICT`s:
  - Iteration should occur over the keys of the object

### Object Access

- Attribute `x` of `y`
  - `y.x`
- Item `x` of `y`
  - Literal code: `y[x]`
  - Code example: `mylist[0]`
  - `LIST`s start at `0`

### Grouping

- Objects can be grouped, for clarity, with brackets (`()`). This also terminates argument consumption and causes grouped evaluation of terms (`(1 + 2) * 3 = 9`)

### Undefined variables

- If an undefined variable is referenced, the following failsafes will occur, in order:
  - The interpreter will attempt to import a SPLW module in scope with the same name as the variable, and set the variable to the imported module
  - If running with a specified interpreter (see `REQUIRE`), the interpreter will attempt to import a native module with the same name as the variable, and set the variable to the imported module
  - If running with a specified interpreter (see `REQUIRE`), the interpreter will attempt to install and import a native module with the same name as the variable, and set the variable to the imported module
  - The interpreter will treat the variable name as a bare word
    - If referencing a property, e.g. `myvar.foo`, only the `myvar` portion will be treated as a bare word. This means that if `myvar` is undefined and `foo` is not a property of `STR`s, this will fail

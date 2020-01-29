//This algoritm details converting an input file into a list of tokens for the parser to parse

enum TokenType:
    Int = 0, //
	Float, //
    Complex, //
    String, //
    Bool, //
	Plus,
	Minus,
	Multiply,
	Divide,
    Intdiv,
    Modulo,
    Power,
    And,
    Or,
    Not,
    Equal,
    NotEqual,
    LessThan,
    GreaterThan,
    LessThanEqual,
    GreaterThanEqual,
    Xor,
    BitwiseAnd,
    BitwiseOr,
    ShiftRight,
    ShiftLeft,
	LParen,
	RParen,
	Function, //
    Procedure, //
    AnonFunc, //
    AnonProc, //
    Struct, //
    As, //
    Return, //
    EndProc, //
    EndStruct, //
    If, //
    For, //
    While, //
    EndIf, //
    EndFor, //
    EndWhile, //
    Input, //
    Output, //
    Create, //
    Cast, //
    Raw, \\
    Quit,
    Require,
    Set, //
    Type, //
	Identifier,
    UncomfirmedExpression,
    ReturnValue,
    BooleanExpr

structure Token:
    member integer tokenID
    member TokenType type
    member string contents
    member int lineNumber
    member int characterNumber

structure UnfinishedToken:
    member integer tokenID
    member TokenType type = TokenType->UncomfirmedExpression
    member TokenType insideBlock
    member string contents

class Lexer:
    member string fileLocation
    member string fileString 
    member Token list tokens

    constructor(string fileLocation):
        member string fileLocation = fileLocation
        string fileString = readFile at fileLocation
        member Token list tokens = create list of Token
		append an UnfinishedToken containing the file string to tokens

    function MakeTokens() -> Token list:
        string-string dictionary regexes = read in regex file or declare manually
        strip comments
        MakeFunctionsAndProcedures()
		MakeIfForWhileAnons()
		MakeIO()
		MakeCreateCastSet()
		MakeNumberStringLiterals()
		MakeRequireQuit()
		MakeOperators()
		MakeFinalIdentifiers()

    function MakeFunctionsProceduresStructs() -> Boolean:
		for each UnfinishedToken with type UnconfirmedExpression	
			run function+procedure+struct regex
			if there were matches:
			    split the fileString around each match.
			    Create UnfinishedToken s for the parts of the string not matched
			    Create UnfinishedToken s for the function/procedure/struct bodies, with insideBlock set to function/procedure/struct respectively
			    Create Identifier tokens for the function/procedure/struct names - 13th, 3rd, 27th capturing group respectively in the regex defined in regexes.txt
			    Create function/procedure, as, end/return tokens
			    Create return value UnfinishedToken type ReturnValue tokens
			    Insert tokens in correct order into tokens list
			    //Matches know the location - extrapolate from this to the location for each token.
			    return True
			else:
			    return False

    function MakeIfForWhileAnons() -> Boolean:
        list of integer indexes
        for each UnfinishedToken with type UncomfirmedExpression:
            run regex
            if matches are present:
                Create tokens - UnfinishedToken with type BooleanExpr for conditions, Call/Identifier etc. for anonf and p.
                Add indexes of block bodies to indexes list
        if length of indexes = 0:
            return False
        while length of indexes is greater than 0:
            reset indexes to empty
            for each index in indexes:
                run regex over corresponding token
                if matches are present:
                    Create tokens - UnfinishedToken with type BooleanExpr for conditions, Call/Identifier etc. for anonf and p.
                    Add indexes of block bodies to indexes list
        return True
    
    function MakeIO() -> Boolean:
        list of integer indexes
        for each UnfinishedToken with type UncomfirmedExpression:
            run regex for input and output
            if matches are present:
                if output:
                    run output variable regex
                Create tokens
                Add indexes of block bodies to indexes list
        if length of indexes is 0:
            return False
        return True

    function MakeCreateCastSet() -> Boolean:
        list of integer indexes
        for each UnfinishedToken with type UncomfirmedExpression:
            run regex
            if matches are present:
                Create tokens
                Add indexes of block bodies to indexes list
        if length of indexes = 0:
            return False
        while length of indexes is greater than 0:
            reset indexes to empty
            for each index in indexes:
                run regex over corresponding token
                if matches are present:
                    Create tokens
                    Add indexes of block bodies to indexes list
        return True

    function MakeNumberStringLiterals() -> Boolean:
        success = False
        for each UncomfirmedExpression:
            run string/RAW regex
            if matches are present:
                success = true
                make string tokens
            run number regex
            if matches are present:
                success = true
                for each match:
                    if i in full match:
                        make Complex
                    else if . in full match:
                        make Float
                    else:
                        make Int
            run bool regex
            if matches are present:
                make tokens
        return success
            
    function MakeRequireQuit() -> Boolean:
        success = False
        for each UncomfirmedExpression:
            run regex
            if matches are present:
                success = True
                make tokens

    function MakeOperators() -> Boolean:
        success = False
        for each UncomfirmedExpression:
            run regex
            if matches are present:
                success = True
                make token based on which capturing group is filled.
        for each BooleanExpr:
            run bool_ops and bool literal regex
            if matches are present:
                success = True
                make tokens
	
	function MakeFinalIdentifiers() -> Boolean:
		success = False
		for each UnconfirmedExpression:
			run indexed_identifier regex
			if there are matches:
				success = True
				for each match:
					if it is in the list of keywords:
						create relevant keyword token
					else:
						create identifier token
			if there are any characters left unmatched:
				output location and unexpected characters
				debug break
                

/// COMMENTS/NOTES

Each set of things which cannot be inside another thing should be a priority level, and can be in an individual priority level
Tokenisation priority levels:
Comments - remove first
Functions, Procedures, Structs
if/for/while/(call/set anonymous funcs/procs) -- Scan recursively each body until no further hits, as these can be inside each other
I/O
Create/Cast/Set



Possibly:
Call/Set Anonymous funcs/procs -- Must indicate where these are in the text as these can occur inside if statements and for/while loops. Indicate as Token[<id> <type>], with regex Token\[[0-9]*? [a-zA-Z]*\] scanning at the end of if/for/while scan to remove and split the string?
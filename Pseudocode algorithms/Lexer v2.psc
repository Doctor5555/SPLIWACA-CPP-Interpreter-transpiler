Dictionary of all possible keywords to the type of keyword to make "keyword_dict"
List of tokens "tokens"
struct token:
    type
    contents
    location

int line_number = 0
int column_number = 0

split_string = split input string
for each string, index in split_string:
    if string in keyword_dict and not "RAW" or "OUTPUT":
        //Keywords will include basic operators and things like that.
        Append to tokens a new token with the type pointed to in the dict by the string
    else if string is newline:
        Append newline token
    else if string fits mathematical_or_string_or_boolean_operation regex:
        split = call SplitMathematicalOrStringOrBooleanOperation
        for section in split:
            Append token of type GetMathematicalOrStringOrBooleanTokenType(section)
    else if string is "RAW":
        Consume up to the next newline as a raw token
    else if string is "OUTPUT":
        Append an output token
        Consume up to the next newline as a raw token
    else if string fits dict_list_decl regex:
        split = call SplitDictDeclt
        for section in split:
            Append token of type GetDictDeclTokenType(section)
    else if string fits identifier regex:
        Append identifier token
    else if string is a space or tab:
        Do nothing
    else:
        Lexical error unexpected characters
    end if
    
    if string is a new line:
        increment line number
        column_number = 0
    else:
        column_number += length of string
    end if
end for

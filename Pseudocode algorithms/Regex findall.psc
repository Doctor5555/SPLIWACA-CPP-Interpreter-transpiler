//This algorithm details a function to find all matches to a regex in a string

function find_all(string regex, string match_string):
    bool success = true
    string list remaining_program
    match list matches
    while success:
        success, match = regex_search(match_string, regex)
        if success:
            append match.prefix to remaining_program
            match_string = match.suffix
            append match to matches
    return remaining_program, matches
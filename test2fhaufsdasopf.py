import libsplw
scope_vars = libsplw.default_scope.copy()

import time  # Source line 2

@libsplw.type_check()
def __func_name_line_7_char_9(prev_scope_vars: dict, arg0: int) -> int: # Source line 8
    scope_vars = prev_scope_vars.copy()
    scope_vars['i'] = arg0
    if libsplw.get_safe(scope_vars, 'i', True, False, False, True) == 0:   # Source line 9
        scope_vars["rv"] = 1  # Source line 10

    else: # Source line 11
        scope_vars["i_1"] = libsplw.get_safe(scope_vars, 'FactRecursive', True, False, False, True)(scope_vars, (libsplw.get_safe(scope_vars, 'i', True, False, False, True) - 1))  # Source line 12

        scope_vars["rv"] = libsplw.get_safe(scope_vars, 'i', True, False, False, True) * libsplw.get_safe(scope_vars, 'i_1', True, False, False, True)  # Source line 13


    return libsplw.get_safe(scope_vars, 'rv', True, False, False, True)  # Source line 15

    raise libsplw.FunctionEndError
scope_vars['FactRecursive'] = __func_name_line_7_char_9

@libsplw.type_check()
def __func_name_line_17_char_9(prev_scope_vars: dict, arg0: int) -> int: # Source line 18
    scope_vars = prev_scope_vars.copy()
    scope_vars['i'] = arg0
    if libsplw.get_safe(scope_vars, 'i', True, False, False, True) == 0:   # Source line 19
        scope_vars["rv"] = 1  # Source line 20

    else: # Source line 21
        scope_vars["rv"] = 1  # Source line 22

        for __for_var_line_22_char_12 in libsplw.get_safe(scope_vars, 'i', True, False, False, True): # Source line 23
            scope_vars["j"] = __for_var_line_22_char_12
            scope_vars["rv"] = libsplw.get_safe(scope_vars, 'rv', True, False, False, True) * (libsplw.get_safe(scope_vars, 'i', True, False, False, True) + 1)  # Source line 24



    return libsplw.get_safe(scope_vars, 'rv', True, False, False, True)  # Source line 27

    raise libsplw.FunctionEndError
scope_vars['FactIterative'] = __func_name_line_17_char_9

print(fr"Enter a number to calculate the factorial of: ")  # Source line 30

scope_vars["number"] = libsplw.handle_input('int')  # Source line 31

if libsplw.get_safe(scope_vars, 'number', True, False, False, True) < 0:   # Source line 33
    print(fr"Number cannot be less than 0")  # Source line 34

    exit()  # Source line 35


scope_vars["timeRecursiveBegin"] = 0  # Source line 38

scope_vars["timeRecursiveEnd"] = 0  # Source line 39

scope_vars["timeIterativeBegin"] = 0  # Source line 40

scope_vars["timeIterativeEnd"] = 0  # Source line 41

scope_vars["timeRecursiveBegin"] = time.time()  # Source line 43

scope_vars["a"] = libsplw.get_safe(scope_vars, 'FactRecursive', True, False, False, True)(scope_vars, libsplw.get_safe(scope_vars, 'number', True, False, False, True))  # Source line 44

scope_vars["timeRecursiveEnd"] = time.time()  # Source line 45

scope_vars["timeIterativeBegin"] = time.time()  # Source line 47

scope_vars["b"] = libsplw.get_safe(scope_vars, 'FactIterative', True, False, False, True)(scope_vars, libsplw.get_safe(scope_vars, 'number', True, False, False, True))  # Source line 48

scope_vars["timeIterativeEnd"] = time.time()  # Source line 49

print(fr"\n")  # Source line 51

print(fr"Recursive factorial result: {libsplw.get_safe(scope_vars, 'a', True, False, False, True)} \n")  # Source line 52

print(fr"Iterative factorial result: {libsplw.get_safe(scope_vars, 'b', True, False, False, True)} \n\n")  # Source line 53

scope_vars["timeDiffRecursive"] = libsplw.get_safe(scope_vars, 'timeRecursiveEnd', True, False, False, True) - libsplw.get_safe(scope_vars, 'timeRecursiveBegin', True, False, False, True)  # Source line 55

scope_vars["timeDiffIterative"] = libsplw.get_safe(scope_vars, 'timeIterativeEnd', True, False, False, True) - libsplw.get_safe(scope_vars, 'timeIterativeBegin', True, False, False, True)  # Source line 56

print(fr"Time difference recursive: {libsplw.get_safe(scope_vars, 'timeDiffRecursive', True, False, False, True)} \n")  # Source line 58

print(fr"Time difference iterative: {libsplw.get_safe(scope_vars, 'timeDiffIterative', True, False, False, True)} \n\n")  # Source line 59

@libsplw.type_check()
def __func_name_line_64_char_9(prev_scope_vars: dict, arg0: int) -> int: # Source line 65
    scope_vars = prev_scope_vars.copy()
    scope_vars['iterations'] = arg0
    if libsplw.get_safe(scope_vars, 'iterations', True, False, False, True) == 1 or libsplw.get_safe(scope_vars, 'iterations', True, False, False, True) == 2:   # Source line 66
        scope_vars["rv"] = 1  # Source line 67

    elif libsplw.get_safe(scope_vars, 'iterations', True, False, False, True) == 0:   # Source line 68
        scope_vars["rv"] = 0  # Source line 69

    else: # Source line 70
        scope_vars["i_1"] = libsplw.get_safe(scope_vars, 'FactRecursive', True, False, False, True)(scope_vars, (libsplw.get_safe(scope_vars, 'iterations', True, False, False, True) - 1))  # Source line 71

        scope_vars["i_2"] = libsplw.get_safe(scope_vars, 'FactRecursive', True, False, False, True)(scope_vars, (libsplw.get_safe(scope_vars, 'iterations', True, False, False, True) - 2))  # Source line 72

        scope_vars["rv"] = libsplw.get_safe(scope_vars, 'i_1', True, False, False, True) + libsplw.get_safe(scope_vars, 'i_2', True, False, False, True)  # Source line 73


    return libsplw.get_safe(scope_vars, 'rv', True, False, False, True)  # Source line 75

    raise libsplw.FunctionEndError
scope_vars['FibonacciRecursive'] = __func_name_line_64_char_9

@libsplw.type_check()
def __func_name_line_77_char_9(prev_scope_vars: dict, arg0: int) -> int: # Source line 78
    scope_vars = prev_scope_vars.copy()
    scope_vars['iterations'] = arg0
    if libsplw.get_safe(scope_vars, 'i', True, False, False, True) == 0:   # Source line 79
        scope_vars["rv"] = 0  # Source line 80

    else: # Source line 81
        scope_vars["rv"] = 1  # Source line 82

        scope_vars["prev"] = 0  # Source line 83

        for __for_var_line_83_char_12 in libsplw.get_safe(scope_vars, 'i', True, False, False, True): # Source line 84
            scope_vars["j"] = __for_var_line_83_char_12
            scope_vars["temp"] = libsplw.get_safe(scope_vars, 'rv', True, False, False, True)  # Source line 85

            scope_vars["rv"] = libsplw.get_safe(scope_vars, 'rv', True, False, False, True) + libsplw.get_safe(scope_vars, 'prev', True, False, False, True)  # Source line 86

            scope_vars["prev"] = libsplw.get_safe(scope_vars, 'temp', True, False, False, True)  # Source line 87



    return libsplw.get_safe(scope_vars, 'rv', True, False, False, True)  # Source line 90

    raise libsplw.FunctionEndError
scope_vars['FibonacciIterative'] = __func_name_line_77_char_9

print(fr"Enter a index to calculate the number in the fibonacci sequence: ")  # Source line 93

scope_vars["number"] = libsplw.handle_input('int')  # Source line 94

if libsplw.get_safe(scope_vars, 'number', True, False, False, True) < 0:   # Source line 96
    print(fr"Number cannot be less than 0")  # Source line 97

    exit()  # Source line 98


scope_vars["timeRecursiveBegin"] = 0  # Source line 101

scope_vars["timeRecursiveEnd"] = 0  # Source line 102

scope_vars["timeIterativeBegin"] = 0  # Source line 103

scope_vars["timeIterativeEnd"] = 0  # Source line 104

scope_vars["timeRecursiveBegin"] = time.time()  # Source line 106

scope_vars["a"] = libsplw.get_safe(scope_vars, 'FibonacciRecursive', True, False, False, True)(scope_vars, libsplw.get_safe(scope_vars, 'number', True, False, False, True))  # Source line 107

scope_vars["timeRecursiveEnd"] = time.time()  # Source line 108

scope_vars["timeIterativeBegin"] = time.time()  # Source line 110

scope_vars["b"] = libsplw.get_safe(scope_vars, 'FibonacciIterative', True, False, False, True)(scope_vars, libsplw.get_safe(scope_vars, 'number', True, False, False, True))  # Source line 111

scope_vars["timeIterativeEnd"] = time.time()  # Source line 112

print(fr"\n")  # Source line 114

print(fr"Recursive fibonacci result: {libsplw.get_safe(scope_vars, 'a', True, False, False, True)} \n")  # Source line 115

print(fr"Iterative fibonacci result: {libsplw.get_safe(scope_vars, 'b', True, False, False, True)} \n\n")  # Source line 116

scope_vars["timeDiffRecursive"] = libsplw.get_safe(scope_vars, 'timeRecursiveEnd', True, False, False, True) - libsplw.get_safe(scope_vars, 'timeRecursiveBegin', True, False, False, True)  # Source line 118

scope_vars["timeDiffIterative"] = libsplw.get_safe(scope_vars, 'timeIterativeEnd', True, False, False, True) - libsplw.get_safe(scope_vars, 'timeIterativeBegin', True, False, False, True)  # Source line 119

print(fr"Time difference recursive: {libsplw.get_safe(scope_vars, 'timeDiffRecursive', True, False, False, True)} \n")  # Source line 121

print(fr"Time difference iterative: {libsplw.get_safe(scope_vars, 'timeDiffIterative', True, False, False, True)} \n")  # Source line 122



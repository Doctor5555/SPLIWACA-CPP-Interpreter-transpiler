import libsplw
scope_vars = {}


@libsplw.type_check()
def __func_name_line_3_char_9(prev_scope_vars: dict, arg0: float) -> float: # Source line 4
    scope_vars = prev_scope_vars.copy()
    scope_vars['fahrenheit'] = arg0

    return (libsplw.get_safe(scope_vars, 'fahrenheit', False, False, False, True) - 32) * 5 / 9  # Source line 6

    raise libsplw.FunctionEndError
scope_vars['fahrenheit_to_celsius'] = __func_name_line_3_char_9

@libsplw.type_check()
def __func_name_line_8_char_9(prev_scope_vars: dict, arg0: float) -> float: # Source line 9
    scope_vars = prev_scope_vars.copy()
    scope_vars['celsius'] = arg0
    return (libsplw.get_safe(scope_vars, 'celsius', True, False, False, True) * 9 / 5) + 32  # Source line 10

    raise libsplw.FunctionEndError
scope_vars['celsius_to_fahrenheit'] = __func_name_line_8_char_9

@libsplw.type_check()
def __func_name_line_12_char_9(prev_scope_vars: dict, arg0: float, arg1: str) -> float: # Source line 13
    scope_vars = prev_scope_vars.copy()
    scope_vars['temperature'] = arg0
    scope_vars['CorF'] = arg1
    if libsplw.get_safe(scope_vars, 'CorF', True, False, False, True) == "C":   # Source line 14
        return libsplw.get_safe(scope_vars, 'celsius_to_fahrenheit', True, False, False, True)(scope_vars, libsplw.get_safe(scope_vars, 'temperature', True, False, False, True))  # Source line 15

    else: # Source line 16
        return libsplw.get_safe(scope_vars, 'fahrenheit_to_celsius', True, False, False, True)(scope_vars, libsplw.get_safe(scope_vars, 'temperature', True, False, False, True))  # Source line 17


    raise libsplw.FunctionEndError
scope_vars['tempConvert'] = __func_name_line_12_char_9

@libsplw.type_check()
def __func_name_line_20_char_10(prev_scope_vars: dict, arg0: str): # Source line 21
    scope_vars = prev_scope_vars.copy()
    scope_vars['a'] = arg0
    print(fr"{libsplw.get_safe(scope_vars, 'a', True, False, False, True)} is a string")  # Source line 22

scope_vars['DoSomething'] = __func_name_line_20_char_10

@libsplw.type_check()
def __func_name_line_24_char_9(prev_scope_vars: dict) -> int: # Source line 25
    scope_vars = prev_scope_vars.copy()
    scope_vars["temp"] = libsplw.handle_input('float')  # Source line 26

    scope_vars["c_or_f_in"] = libsplw.handle_input('str')  # Source line 27

    if libsplw.get_safe(scope_vars, 'c_or_f_in', True, False, False, True) == "C":   # Source line 28
        scope_vars["c_or_f"] = libsplw.get_safe(scope_vars, 'C', True, False, False, True)  # Source line 29

    elif libsplw.get_safe(scope_vars, 'c_or_f_in', True, False, False, True) == "F":   # Source line 30
        scope_vars["c_or_f"] = libsplw.get_safe(scope_vars, 'F', True, False, False, True)  # Source line 31

    else: # Source line 32
        print(fr"{libsplw.get_safe(scope_vars, 'c_or_f_in', True, False, False, True)} is not 'C' or 'F'")  # Source line 33

        exit(1)  # Source line 34


    scope_vars["temp"] = libsplw.get_safe(scope_vars, 'tempConvert', True, False, False, True)(scope_vars, libsplw.get_safe(scope_vars, 'temp', True, False, False, True), libsplw.get_safe(scope_vars, 'c_or_f', True, False, False, True))  # Source line 36

    print(fr"{libsplw.get_safe(scope_vars, 'temp', True, False, False, True)} ⁰ {libsplw.get_safe(scope_vars, 'c_or_f', True, False, False, True)}")  # Source line 37

    libsplw.get_safe(scope_vars, 'DoSomething', True, False, False, True)(scope_vars, "Hello World")  # Source line 38

    scope_vars["b"] = 10  # Source line 38

    print(fr"{libsplw.get_safe(scope_vars, 'b', True, False, False, True)} is ten if I did this right!")  # Source line 38

    return 0  # Source line 39

    raise libsplw.FunctionEndError
scope_vars['main'] = __func_name_line_24_char_9

libsplw.get_safe(scope_vars, 'main', True, False, False, True)(scope_vars)  # Source line 42



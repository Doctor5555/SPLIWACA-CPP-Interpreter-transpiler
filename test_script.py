import libsplw
scope_vars = libsplw.default_scope.copy()


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
    return (libsplw.get_safe(scope_vars, 'celsius', True, True, False, True) * 9 / 5) + 32  # Source line 10

    raise libsplw.FunctionEndError
scope_vars['celsius_to_fahrenheit'] = __func_name_line_8_char_9

@libsplw.type_check()
def __func_name_line_12_char_9(prev_scope_vars: dict, arg0: float, arg1: str) -> float: # Source line 13
    scope_vars = prev_scope_vars.copy()
    scope_vars['temperature'] = arg0
    scope_vars['CorF'] = arg1
    if libsplw.get_safe(scope_vars, 'CorF', True, True, False, True) == "C":   # Source line 14
        return libsplw.get_safe(scope_vars, 'celsius_to_fahrenheit', True, True, False, True)(scope_vars, libsplw.get_safe(scope_vars, 'temperature', True, True, False, True))  # Source line 15

    else: # Source line 16
        return libsplw.get_safe(scope_vars, 'fahrenheit_to_celsius', True, True, False, True)(scope_vars, libsplw.get_safe(scope_vars, 'temperature', True, True, False, True))  # Source line 17


    raise libsplw.FunctionEndError
scope_vars['tempConvert'] = __func_name_line_12_char_9

@libsplw.type_check()
def __func_name_line_20_char_10(prev_scope_vars: dict) -> None: # Source line 21
    scope_vars = prev_scope_vars.copy()
    scope_vars["temp"] = libsplw.handle_input('float')  # Source line 22

    scope_vars["c_or_f"] = libsplw.handle_input('str')  # Source line 23

    if libsplw.get_safe(scope_vars, 'c_or_f_in', True, True, False, True) == "C":   # Source line 24
        scope_vars["c_or_f"] = libsplw.get_safe(scope_vars, 'C', True, True, False, True)  # Source line 25

    elif libsplw.get_safe(scope_vars, 'c_or_f_in', True, True, False, True) == "F":   # Source line 26
        scope_vars["c_or_f"] = libsplw.get_safe(scope_vars, 'F', True, True, False, True)  # Source line 27

    else: # Source line 28
        print(fr"{libsplw.get_safe(scope_vars, 'c_or_f_in', True, True, False, True)} is not 'C' or 'F'")  # Source line 29

        exit(1)  # Source line 30


    scope_vars["temp"] = libsplw.get_safe(scope_vars, 'tempConvert', True, True, False, True)(scope_vars, libsplw.get_safe(scope_vars, 'user_temp', True, True, False, True), libsplw.get_safe(scope_vars, 'c_or_f', True, True, False, True))  # Source line 32

    print(fr"{libsplw.get_safe(scope_vars, 'temp', True, True, False, True)} ⁰ {libsplw.get_safe(scope_vars, 'c_or_f', True, True, False, True)}")  # Source line 33

    raise libsplw.FunctionEndError
scope_vars['main'] = __func_name_line_20_char_10



import libsplw
scope_vars = libsplw.default_scope.copy()

@libsplw.type_check()
def __func_name_line_1_char_9(prev_scope_vars: dict, arg0: int, arg1: int) -> int: # Source line 2
    scope_vars = prev_scope_vars.copy()
    scope_vars['a'] = arg0
    scope_vars['b'] = arg1

    scope_vars["my_a"] = libsplw.get_safe(scope_vars, 'a', False, False, False, True)  # Source line 4

    print(fr"{libsplw.get_safe(scope_vars, 'my_a', False, False, False, True)}")  # Source line 5

    raise libsplw.FunctionEndError
scope_vars['my_func'] = __func_name_line_1_char_9

scope_vars["continue"] = libsplw.get_safe(scope_vars, 'true', True, False, False, True)  # Source line 8

while libsplw.get_safe(scope_vars, 'continue', True, False, False, True) == libsplw.get_safe(scope_vars, 'true', True, False, False, True): # Source line 9
    scope_vars["dice"] = 7  # Source line 10

    scope_vars["attempts"] = 0  # Source line 11

    while libsplw.get_safe(scope_vars, 'dice', True, False, False, True) != 6: # Source line 12
        scope_vars["dice"] = (libsplw.get_safe(scope_vars, 'random', True, False, False, True)(scope_vars, 1, 6))  # Source line 13

        print(fr"{libsplw.get_safe(scope_vars, 'dice', True, False, False, True)}")  # Source line 14

        scope_vars["attempts"] += 1  # Source line 15


    print(fr"that took {libsplw.get_safe(scope_vars, 'attempts', True, False, False, True)} attempts")  # Source line 17

    print(fr"do you want to rerun 	")  # Source line 18

    scope_vars["continue"] = libsplw.handle_input('bool')  # Source line 19


if libsplw.get_safe(scope_vars, 'True', True, False, False, True):   # Source line 22
    print(fr"IF line test")  # Source line 23

elif libsplw.get_safe(scope_vars, 'False', True, False, False, True):   # Source line 24
    print(fr"This should not OUTPUT")  # Source line 25


scope_vars["_INTERPRETER"] = libsplw.get_safe(scope_vars, 'a', True, False, False, True)  # Source line 28



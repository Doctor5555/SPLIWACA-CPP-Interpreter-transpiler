import libsplw
scope_vars = libsplw.default_scope.copy()

scope_vars["continue"] = libsplw.get_safe(scope_vars, 'true', True, False, True)  # Source line 3
while libsplw.get_safe(scope_vars, 'continue', True, False, True) == libsplw.get_safe(scope_vars, 'true', True, False, True): # Source line 4
    scope_vars["dice"] = 7  # Source line 5
    scope_vars["attempts"] = 0  # Source line 6
    while libsplw.get_safe(scope_vars, 'dice', True, False, True) != 6: # Source line 7
        scope_vars["dice"] = (libsplw.get_safe(scope_vars, 'random', True, False, True)(scope_vars, 1, 6))  # Source line 8
        print(fr"{libsplw.get_safe(scope_vars, 'dice', True, False, True)}")  # Source line 9
        scope_vars["attempts"] += 1  # Source line 10
    print(fr"that took {libsplw.get_safe(scope_vars, 'attempts', True, False, True)} attempts")  # Source line 12
    print(fr"do you want to rerun 	")  # Source line 13
    scope_vars["continue"] = libsplw.handle_input('bool')  # Source line 14
if libsplw.get_safe(scope_vars, 'True', True, False, True):   # Source line 17
    print(fr"IF line test")  # Source line 18
elif libsplw.get_safe(scope_vars, 'False', True, False, True):   # Source line 19
    print(fr"This should not OUTPUT")  # Source line 20
scope_vars["_INTERPRETER"] = libsplw.get_safe(scope_vars, 'a', True, False, True)  # Source line 23


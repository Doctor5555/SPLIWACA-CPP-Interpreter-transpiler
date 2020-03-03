import libsplw
scope_vars = libsplw.default_scope.copy()

scope_vars["continue"] = True
scope_vars["dice"] = 7
scope_vars["attempts"] = 0
while scope_vars['continue'] == True:
    while scope_vars['dice'] != 6:
        scope_vars["dice"] = (scope_vars['random'](scope_vars, 1, 6))
        print(fr"{dice}")
        scope_vars["attempts"] += 1

    print(fr"that took {attempts} attempts")
    print(fr"do you want to rerun 	")
    scope_vars["continue"] = libsplw.handle_input('bool')



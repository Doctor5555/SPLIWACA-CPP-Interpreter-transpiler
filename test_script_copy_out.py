import libsplw, math
scope_vars = {}

print(fr"" + "\"" + fr"Enter a base 10 number: " + "\"" + fr"")
scope_vars["base10"] = libsplw.handle_input('float')
scope_vars["integer_part"] = math.floor(scope_vars['base10'])
scope_vars["float_part"] = scope_vars['base10'] - scope_vars['integer_part']
scope_vars["integer_part"] = (bin(scope_vars['integer_part']))
scope_vars["integer_part"] = scope_vars['integer_part'][2:]
scope_vars["after_point"] = ""
for __for_var_line_9_char_4 in (range(8)):
    scope_vars["i"] = __for_var_line_9_char_4
    scope_vars["float_part"] = scope_vars['float_part'] * 2
    if scope_vars['float_part'] > 1:
        scope_vars["float_part"] = scope_vars['float_part'] - 1
        scope_vars["after_point"] = scope_vars['after_point'] + "1"
    else:
        scope_vars["after_point"] = scope_vars['after_point'] + "0"


print(fr"{scope_vars['integer_part']} . {scope_vars['after_point']}")

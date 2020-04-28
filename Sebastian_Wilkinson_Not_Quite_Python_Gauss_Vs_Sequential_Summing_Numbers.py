import libsplw
<<<<<<< Updated upstream
<<<<<<< Updated upstream
scope_vars = {}

import time  # Source line 3

@libsplw.type_check()
=======
=======
>>>>>>> Stashed changes
scope_vars = {} #libsplw.default_scope.copy()

import time  # Source line 3

@libsplw.type_check
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
def __func_name_line_4_char_9(prev_scope_vars: dict) -> int: # Source line 5
    scope_vars = prev_scope_vars.copy()
    print(fr"Enter a number: ")  # Source line 6

    scope_vars["n"] = libsplw.handle_input('int')  # Source line 7

    print(fr"\n")  # Source line 8

    scope_vars["sumSequential"] = int()  # Source line 10

    scope_vars["sumGaussian"] = int()  # Source line 11

<<<<<<< Updated upstream
<<<<<<< Updated upstream
    scope_vars["timeStartSequential"] = time.time()  # Source line 13
=======
    scope_vars["timeStartSequential"] = time)  # Source line 13
>>>>>>> Stashed changes
=======
    scope_vars["timeStartSequential"] = time)  # Source line 13
>>>>>>> Stashed changes

    for __for_var_line_14_char_5 in (range(libsplw.get_safe(scope_vars, 'n', True, True, True, True))): # Source line 15
        scope_vars["i"] = __for_var_line_14_char_5
        scope_vars["sumSequential"] = libsplw.get_safe(scope_vars, 'sumSequential', True, True, True, True) + libsplw.get_safe(scope_vars, 'i', True, True, True, True) + 1  # Source line 16


<<<<<<< Updated upstream
<<<<<<< Updated upstream
    scope_vars["timeEndSequential"] = time.time()  # Source line 19

    scope_vars["timeStartGaussian"] = time.time()  # Source line 21

    scope_vars["sumGaussian"] = libsplw.get_safe(scope_vars, 'n', True, True, True, True) * (libsplw.get_safe(scope_vars, 'n', True, True, True, True) + 1) * 0.5  # Source line 23

    scope_vars["timeEndGaussian"] = time.time()  # Source line 25

    if libsplw.get_safe(scope_vars, 'sumGaussian', True, True, True, True) != libsplw.get_safe(scope_vars, 'sumSequential', True, True, True, True):   # Source line 27
        print(fr"Sums are not equal. Something is wrong! Gaussian result: {libsplw.get_safe(scope_vars, 'sumGaussian', True, True, True, True)} , sequential result: {libsplw.get_safe(scope_vars, 'sumSequential', True, True, True, True)}")  # Source line 28
=======
=======
>>>>>>> Stashed changes
    scope_vars["timeEndSequential"] = time)  # Source line 19

    scope_vars["timeStartGaussian"] = time)  # Source line 21

    scope_vars["sumGaussian"] = libsplw.get_safe(scope_vars, 'sumGaussian', True, True, True, True) * (libsplw.get_safe(scope_vars, 'sumGaussian', True, True, True, True) + 1)  # Source line 23

    scope_vars["timeEndGaussian"] = time)  # Source line 25

    if libsplw.get_safe(scope_vars, 'sumGaussian', True, True, True, True) != libsplw.get_safe(scope_vars, 'sumSequential', True, True, True, True):   # Source line 27
        print(fr"" + "\"" + fr"Sums are not equal. Something is wrong!" + "\"" + fr"")  # Source line 28
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

        scope_vars["rv"] =  - 1  # Source line 29

    else: # Source line 30
        scope_vars["differenceSequential"] = libsplw.get_safe(scope_vars, 'timeEndSequential', True, True, True, True) - libsplw.get_safe(scope_vars, 'timeStartSequential', True, True, True, True)  # Source line 31

        scope_vars["differenceGaussian"] = libsplw.get_safe(scope_vars, 'timeEndGaussian', True, True, True, True) - libsplw.get_safe(scope_vars, 'timeStartGaussian', True, True, True, True)  # Source line 32

        scope_vars["strSumGaussian"] = str(libsplw.get_safe(scope_vars, 'sumGaussian', True, True, True, True))  # Source line 34

        print(fr"The sum is: {libsplw.get_safe(scope_vars, 'strSumGaussian', True, True, True, True)}")  # Source line 35

        scope_vars["strTimeSequential"] = str(libsplw.get_safe(scope_vars, 'differenceSequential', True, True, True, True))  # Source line 37

        print(fr"Time sequential: {libsplw.get_safe(scope_vars, 'strTimeSequential', True, True, True, True)}")  # Source line 38

        scope_vars["strTimeGaussian"] = str(libsplw.get_safe(scope_vars, 'differenceGaussian', True, True, True, True))  # Source line 39

        print(fr"Time gaussian: {libsplw.get_safe(scope_vars, 'strTimeGaussian', True, True, True, True)}")  # Source line 40

        scope_vars["rv"] = 0  # Source line 42


    return libsplw.get_safe(scope_vars, 'rv', True, True, True, True)  # Source line 44

    raise libsplw.FunctionEndError
scope_vars['Main'] = __func_name_line_4_char_9

libsplw.get_safe(scope_vars, 'Main', True, True, True, True)(scope_vars)  # Source line 47



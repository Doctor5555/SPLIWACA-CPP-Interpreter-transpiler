import libsplw as __interpreter_lib
@__interpreter_lib.type_check
def fahrenheit_to_celsius(fahrenheit: __builtins__.float) -> __builtins__.float:
	return ((fahrenheit - 32) * 5 / 9)
	raise __interpreter_lib.FunctionEndError
@__interpreter_lib.type_check
def celsius_to_fahrenheit(celsius: __builtins__.float) -> __builtins__.float:
	return ((celsius * 9 / 5) + 32)
	raise __interpreter_lib.FunctionEndError
@__interpreter_lib.type_check
def tempConvert(temperature: __builtins__.float, CorF: __builtins__.str) -> __builtins__.float:
	if (CorF == "C"):
		return (celsius_to_fahrenheit(temperature))
	else:
		return (fahrenheit_to_celsius(temperature))

	raise __interpreter_lib.FunctionEndError
@__interpreter_lib.type_check
def main() -> __builtins__.None:
	temp = __interpreter_lib.input('__builtins__.float')
	c_or_f = __interpreter_lib.input('__builtins__.str')
	if (c_or_f_in == "C"):
		c_or_f = (C)
	elif (c_or_f_in == "F"):
		c_or_f = (F)
	else:
		print(fr"{c_or_f_in} is not 'C' or 'F'")
		exit(1)

	temp = (tempConvert(user_temp, c_or_f))
	print(fr"{temp} ⁰ {c_or_f}")



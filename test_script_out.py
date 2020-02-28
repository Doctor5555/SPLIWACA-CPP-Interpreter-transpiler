import libsplw
@libsplw.type_check
def fahrenheit_to_celsius(fahrenheit: float) -> float:
	return (fahrenheit - 32) * 5 / 9
	raise libsplw.FunctionEndError
@libsplw.type_check
def celsius_to_fahrenheit(celsius: float) -> float:
	return (celsius * 9 / 5) + 32
	raise libsplw.FunctionEndError
@libsplw.type_check
def tempConvert(temperature: float, CorF: str) -> float:
	if CorF == "C":
		return celsius_to_fahrenheit(temperature)
	else:
		return fahrenheit_to_celsius(temperature)

	raise libsplw.FunctionEndError
@libsplw.type_check
def main() -> None:
	temp = libsplw.input('float')
	c_or_f = libsplw.input('str')
	if c_or_f_in == "C":
		c_or_f = C
	elif c_or_f_in == "F":
		c_or_f = F
	else:
		print(fr"{c_or_f_in} is not 'C' or 'F'")
		exit(1)

	temp = tempConvert(user_temp, c_or_f)
	print(fr"{temp} ⁰ {c_or_f}")



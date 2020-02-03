def test_func(a: int) -> int:
    return a


a = "Hello World"

if True:
    a = 5
else:
    a = "no"

print(test_func(a))

def test():
    print("Failure!")
test()
del test
test = lambda: print("Success!")
print(test())

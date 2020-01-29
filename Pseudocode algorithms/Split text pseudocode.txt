V2::
For split-char in split-chars:
	Store if there is a split-char at the beginning
	split by split-char
	Insert split-char between each element
	Insert split-char at beginning if there was one there.

V3::
string intermediate
list tokens
For char in string:
	if char in split-chars:
		Push intermediate to tokens
		Reset intermediate to char
		Push intermediate to tokens
		Reset intermediate to empty
	else:
		Add char to intermediate
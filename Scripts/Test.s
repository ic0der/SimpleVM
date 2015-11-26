; Simple tests that test SVM capabilities

push 'SVM Test\n'
native print
halt

.export CallableFromC
CallableFromC:
	push 'WORX\n'
	native print
	ret

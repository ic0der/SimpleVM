; Factorial calculator

hrsrv 0 ; heap reserve (For fun):P

push 'Enter a number to calculate its factorial: '
native print
native read
tonum
call FaCtoriaL
store 0 ; store result of FaCtoriaL

; you push arguments in reverse, right? :D
push '\n'
load 0 tostr
push 'Result: '
push 3 ; strconcat takes number of strings to concat
native strconcat
native print

ret

.export FaCtoriaL
FaCtoriaL:
	store 0 ; counter
	push 1 store 1 ; result

	L0C:
		load 0
		jnz L0D
		jmp L0E
	L0D:
		load 1
		load 0
		mul ; result * counter
		store 1 ; result

		load 0 ; counter
		dec ; counter - 1
		store 0
		jmp L0C
	L0E:

	load 1
	ret

;R1 stores the time value
;R2 is set to 4 to compare with the input value in R1
;R3 is used to add with R1 to indicate the time
;R4 is used to count the times of running a loop
;R5 is used to store the ASCII value of #0, which is #48
;R6 is used to store the ASCII vlaue of " ", which is #32
.ORIG x3000

PRINT_SLOT
	ST R0,ZERO1
	ST R1,ONE1
	ST R2,TWO1
	ST R3,THREE1
	ST R4,FOUR1
	ST R5,FIVE1
	ST R6,SIX1
	ST R7,SEVEN1
	AND R0,R0,#0		;Initialize registers as described above	
	AND R4,R4,#0
	ADD R2,R0,#4			
	ADD R3,R0,#6
	ADD R5,R0,#15
	ADD R5,R5,#15
	ADD R5,R5,#15
	ADD R5,R5,#3		;R5 stores the ascii value of 0
	ADD R6,R0,#15
	ADD R6,R6,#15
	ADD R6,R6,#2		;R6 stores the ascii value of " "

	ADD R4,R4,#3
	ADD R0,R0,R6
PRINT_3SPACES			;print 3 spaces before the time
	OUT
	ADD R4,R4,#-1
	BRp PRINT_3SPACES

	AND R0,R0,#0
	ADD R2,R1,#-4		;R2 = N - 4, as 4 indicates the 10:00, this 
	BRn PRINT_1ZERO		;indicates whether the input time>=10:00
	BRnzp PRINT_TIME
PRINT_1ZERO
	ADD R0,R0,R5		
	OUT
PRINT_TIME
	AND R0,R0,#0
	ADD R1,R1,R3
	ADD R2,R1,#-10		;Now R1 store the time want as R1 = N + 6 and
	BRzp LARGE		;use R2 to indicate whether it is lager than 10
	ADD R0,R1,R5		;Now R0 store the ascii value of the time when it is less than 10
	OUT			
	BRnzp PREZERO
LARGE	ADD R4,R2,#-10		;As number lager than 10 has to be printed seperately,
	BRz TWENTY		;I use N-10 to indicate the second digit I need to print
	ADD R0,R5,#1		;which is stored in R2 and with R5+R2 to print out
	OUT
	ADD R0,R2,R5
	OUT
	BRnzp PREZERO
TWENTY	ADD R0,R5,#2		;If N-20 is 0, then print 20 seperately
	OUT
	ADD R0,R0,#-2
	OUT
PREZERO	AND R4,R4,#0		;prepare ascii value of 0 into R0
	ADD R4,R4,#2
	ADD R0,R5,#0
PRINT_2ZERO
	OUT
	ADD R4,R4,#-1
	BRp PRINT_2ZERO

	AND R4,R4,#0
	ADD R4,R4,#2
	ADD R0,R6,#0
PRINT_2SPACES
	OUT
	ADD R4,R4,#-1
	BRp PRINT_2SPACES
	
	LD R0,ZERO1
	LD R1,ONE1
	LD R2,TWO1
	LD R3,THREE1
	LD R4,FOUR1
	LD R5,FIVE1
	LD R6,SIX1
	LD R7,SEVEN1
	RET
		
PRINT_CENTERED
; R1 is used to store the address of the string
; R2 is used to store the address of the string temporarily
;    and then  to store the total number of spaces
; R3 is used to store the number of pre-spaces
; R4 is used to count the length of string and then to store 
;    the number of trailing spaces
; R5 is used flexibly to count or compare
; R6 is used to store the ascii value of " ", which is #32
	ST R0,ZERO2
	ST R1,ONE2
	ST R2,TWO2
	ST R3,THREE2
	ST R4,FOUR2
	ST R5,FIVE2
	ST R6,SIX2
	ST R7,SEVEN2

	ADD R2,R1,#0
	AND R4,R4,#0
	ADD R6,R4,#15
	ADD R6,R6,#15
	ADD R6,R6,#2
	LDR R3,R2,#0
	BRz NULL

LENGTH	LDR R3,R2,#0		;Every time R3 load in a char, check if it is 0
	ADD R4,R4,#1		;if it is not zero, R4++ to calculate the length
	ADD R2,R2,#1
	ADD R3,R3,#0
	BRnp LENGTH
	ADD R4,R4,#-1		;R4 has been counted for one more time
	ADD R5,R4,#-9		;use R5 to indicate of the length is lager than 9
	BRn LESS9
	AND R3,R3,#0
	ADD R2,R1,#0
MORE9	LDR R0,R2,#0		;If len>=9, just print out first 9 chars
	OUT
	ADD R2,R2,#1
	ADD R3,R3,#1
	ADD R5,R3,#-9
	BRn MORE9
	BRnzp FIN

LESS9	NOT R4,R4		;If len<9, we need to count the spaces needed
	ADD R4,R4,#1
	AND R3,R3,#0
	ADD R2,R4,#9		;R2 is the number of total sapces
	ADD R5,R2,#0		;Duplicate R2 into R5
COUNT	ADD R3,R3,#1		;R3 is the number of pre-spaces
	ADD R5,R5,#-2		;For odd numbers: N_odd = n*2 - 1, whose n is pre-spaces
	BRp COUNT		;For even numbers:N_even = n*2, whose n is pre-spaces 
	NOT R4,R3
	ADD R4,R4,#1
	ADD R4,R2,R4		;R4 is the number of trailing sapces
	
	ADD R0,R6,#0
SPACE1	OUT
	ADD R3,R3,#-1
	BRp SPACE1
	LDR R0,R1,#0
STRING	OUT
	ADD R1,R1,#1
	LDR R0,R1,#0
	BRnp STRING 
	
	ADD R0,R6,#0
	ADD R4,R4,#0
	BRz FIN
SPACE2	OUT
	ADD R4,R4,#-1
	BRp SPACE2
	BRnzp FIN
NULL
	AND R1,R1,#0
        ADD R1,R1,#9
        ADD R0,R6,#0
SPACES9 OUT
        ADD R1,R1,#-1
        BRp SPACES9
FIN
	LD R0,ZERO2
	LD R1,ONE2
	LD R2,TWO2
	LD R3,THREE2
	LD R4,FOUR2
	LD R5,FIVE2
	LD R6,SIX2
	LD R7,SEVEN2
	RET
ZERO1	.FILL x0000
ONE1	.FILL X0000
TWO1	.FILL x0000
THREE1	.FILL x0000
FOUR1	.FILL x0000
FIVE1	.FILL x0000
SIX1	.FILL x0000
SEVEN1	.FILL x0000

ZERO2	.FILL x0000
ONE2	.FILL x0000
TWO2	.FILL x0000 
THREE2	.FILL x0000
FOUR2	.FILL x0000
FIVE2	.FILL x0000
SIX2	.FILL x0000
SEVEN2	.FILL x0000
	.END
























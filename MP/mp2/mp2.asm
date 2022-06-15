.ORIG	x3000
; Machine Problem 2
; Name: Guanshujie Fu
; Studnet ID: 3190110666

; This program could print a student's schedule presented as Day and Time
;   using data stored in x4000 and x3800

; EVENT label point to x4000, SCHEDULE label point to x3800
; R0 is used as temporary register
; R1 is used to store the value for PRINT_SLOT and PRINT_CENTERED
; R2 is mainly used as pointer of SCHEDULE address in print part
; R3 is mainly used as loop counter 
; R4 is mainly used to store the address of EVENT
; R5 is used to store the value in EVENT address
; R6 is used as the pointer of address
    LD  R6,SCHEDULE
    LD  R4,COUNT
    AND R5,R5,#0    
INITIALIZE                  ; Initialize the 75 memories to x0000
    STR	R5,R6,#0
    ADD R6,R6,#1
    ADD R4,R4,#-1
    BRp INITIALIZE
TRANSLATE_DATE
    LD  R4,EVENT            ; Now R4 point at the bit vector field
    LDR R5,R4,#0
    ADD R3,R5,#1            ; Indicate if M[R4] is -1
    BRz PRINT_SCHEDULE      ; If M[R4] is -1, jump to the PRINT part
    AND R3,R3,#0
    ADD R3,R3,#11           ; As the biggiest date is #16, there will have 11 zeros
POWER
    ADD R5,R5,R5
    ADD R3,R3,#-1
    BRp POWER               ; Now R5 has the bit vector

    LEA R3,DATE             ; R3 point at the DATE
    LD  R0,MINUSONE         ; R0 stores the -1 to reset DATE from Mon. to Fri.
    STR R0,R3,#0
    STR R0,R3,#1
    STR R0,R3,#2
    STR R0,R3,#3
    STR R0,R3,#4

    AND R2,R2,#0
INDICATE_DATE
    ADD R5,R5,#0
    BRzp PASS1              ; If the bit for this day is 0, then PASS
    LDR R1,R3,#0            ; Indicate if the DATE has been loaded value into it.
    BRzp ERROR              ; If >=0, then has been previously loaded, ERROR
    STR R2,R3,#0            ; If <0, then M[R3] <- R2
PASS1    ADD R5,R5,R5       ; R5 = R5*2, come to judge next bit
    ADD R3,R3,#1
    ADD R2,R2,#1
    ADD R0,R2,#-4
    BRnz INDICATE_DATE

    ADD R4,R4,#1            ; Now R4 point at the address of the begining of event(the string) field
    ADD R0,R4,#0            ; Store the address (x4000+) in R0
INDICATE_NULL
    LDR R5,R4,#0            ; Load the char of string into R5
    BRz INDICATE_SLOT       ; If it is null, then come to get slot
    ADD R4,R4,#1
    BRnzp INDICATE_NULL

INDICATE_SLOT
    ADD R4,R4,#1            ; Now R4 points at the value of SLOT
    LDR R5,R4,#0            ; R5 <-- M[R4], R5 stores the value of SLOT
							; Indicate if the slot number is correct
	ADD R5,R5,#-14
	BRp ERROR
	ADD R5,R5,#14

    ADD R4,R4,#1            ; Now R4 points at the next event
    ST  R4,EVENT            ; Put the address of next event into EVENT

    ADD R3,R5,#0
    ADD R3,R3,R3
    ADD R3,R3,R3
    ADD R3,R3,R5            
    ADD R5,R3,#0            ; Now R5 = SLOT*5
    ADD R1,R5,#0            ; Copy R5 into R1

    LEA R2,DATE             ; R2 to store the address of DATE
    AND R3,R3,#0
    ADD R3,R3,#5            ; R3 is use as loop counter (days count)
CALCULATE_ADDRESS           ; x3800+(SLOT-6)*5+DATE
    ADD R3,R3,#0
    BRz TRANSLATE_DATE
    LDR R4,R2,#0            ; R4 store the value in DATE to indicate which day has events
    BRn PASS2
    ADD R5,R1,R4            ; Now R5 = SLOT*5+DATE
    LD  R6,SCHEDULE
    ADD R6,R6,R5            ; Now R6 = x3800+(SLOT-6)*5+DATE
							; Indicate if there exists conflict
	LDR R5,R6,#0
	BRnp CONFLICT

    STR R0,R6,#0            ; Put the EVENT stored in R0 into target address for printing
PASS2    ADD R2,R2,#1       ; R2 points at next DATE address
    ADD R3,R3,#-1
    BRnzp CALCULATE_ADDRESS  


PRINT_SCHEDULE         
    LEA R0,FIRSTLINE
    PUTS
    LD  R2,SCHEDULE
    AND R3,R3,#0            ; R3 works as ROW counter = 15 and indication of SLOT (0~14)
ROWLOOP
    AND R4,R4,#0            ; R4 works as COL counter = 5
    ADD R4,R4,#5
    ADD R1,R3,#0            ; Put the SLOT indicated by R3 into R1
    JSR	PRINT_SLOT
PRINT_EVENT                 ; It is actually COLLOOP
    LD R0,SEPERATE          ; Print a '|' at the begining of each event
    OUT
    ST  R2,WHATIN           ; R2 is x3800+ while R5 will indicate x4000+
    LDR R5,R2,#0            ; Load the value in M[R2] into R5 to indicate whether there exists event
    BRz PRINT_EMPTY         ; If it is empty, print spaces
    ADD R1,R5,#0            ; If it is not empty, store the address of R2 into R1
    JSR PRINT_CENTERED
BACK
    ADD R2,R2,#1
    ADD R4,R4,#-1
    BRp PRINT_EVENT
    LD R0,NEWLINE
    OUT
    ADD R3,R3,#1
    ADD R6,R3,#-14
    BRnz ROWLOOP
    HALT

PRINT_EMPTY
    LEA R1,EMPTY
    JSR PRINT_CENTERED
    BRnzp BACK
ERROR
	PUTS
    LEA R0,ERRORINFO1
    PUTS
    HALT

CONFLICT
	PUTS
	LEA R0,ERRORINFO2
	PUTS
	HALT

SCHEDULE    .FILL x3800
EVENT       .FILL x4000
COUNT       .FILL #5
MINUSONE    .FILL xFFFF
WHATIN      .FILL x0000
SEPERATE    .FILL x007C     ;'|'
NEWLINE     .FILL x000A     ;'\n'
EMPTY       .FILL x0000
DATE        .FILL xFFFF     ; date of Monday to Friday
            .FILL xFFFF     ; used for store the value that indicate this day have events
            .FILL xFFFF     ; Initialize to -1 since 0 indicates Monday
            .FILL xFFFF
            .FILL xFFFF
FIRSTLINE   .STRINGZ	"         |  MONDAY | TUESDAY |WEDNESDAY| THURSDAY|  FRIDAY \n"
ERRORINFO1  .STRINGZ	" has an invalid slot number.\n"
ERRORINFO2  .STRINGZ	" conflicts with an earlier event.\n"


;****************************************************************************************************
PRINT_SLOT
;This subroutine accept a value in R1 and print out the corresponding time limiting in 9 characters
;R1 stores the time value
;R2 is set to 4 to compare with the input value in R1
;R3 is used to add with R1 to indicate the time
;R4 is used to count the times of running a loop
;R5 is used to store the ASCII value of #0, which is #48
;R6 is used to store the ASCII vlaue of " ", which is #32
	ST R0,ZERO			;As I use many registers, I need to restore the primary value of all registers
	ST R1,ONE
	ST R2,TWO
	ST R3,THREE
	ST R4,FOUR
	ST R5,FIVE
	ST R6,SIX
	ST R7,SEVEN

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
	BRzp LARGE			;use R2 to indicate whether it is lager than 10
	ADD R0,R1,R5		;Now R0 store the ascii value of the time when it is less than 10
	OUT			
	BRnzp PREZERO
LARGE	
	ADD R4,R2,#-10		;As number lager than 10 has to be printed seperately,
	BRz TWENTY			;I use N-10 to indicate the second digit I need to print
	ADD R0,R5,#1		;which is stored in R2 and with R5+R2 to print out
	OUT
	ADD R0,R2,R5
	OUT
	BRnzp PREZERO
TWENTY	
	ADD R0,R5,#2		;If N-20 is 0, then print 20 seperately
	OUT
	ADD R0,R0,#-2
	OUT
PREZERO	AND R4,R4,#0	;prepare ascii value of 0 into R0
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
	
	LD R0,ZERO
	LD R1,ONE
	LD R2,TWO
	LD R3,THREE
	LD R4,FOUR
	LD R5,FIVE
	LD R6,SIX
	LD R7,SEVEN
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
	ST R0,ZERO
	ST R1,ONE
	ST R2,TWO
	ST R3,THREE
	ST R4,FOUR
	ST R5,FIVE
	ST R6,SIX
	ST R7,SEVEN

	ADD R2,R1,#0		;copy the address to R2
	AND R4,R4,#0
	ADD R6,R4,#15
	ADD R6,R6,#15
	ADD R6,R6,#2
	LDR R3,R2,#0		;if the first one is x0000, I simpily print 9 spaces in NULL part
	BRz NULL			;	as it might be direct and lazy

LENGTH	
	LDR R3,R2,#0		;Every time R3 load in a char, check if it is 0
	ADD R4,R4,#1		;if it is not zero, R4++ to calculate the length
	ADD R2,R2,#1
	ADD R3,R3,#0
	BRnp LENGTH
	ADD R4,R4,#-1		;R4 has been counted for one more time
	ADD R5,R4,#-9		;use R5 to indicate of the length is lager than 9
	BRn LESS9
	AND R3,R3,#0
	ADD R2,R1,#0
MORE9	
	LDR R0,R2,#0		;If len>=9, just print out first 9 chars
	OUT
	ADD R2,R2,#1
	ADD R3,R3,#1
	ADD R5,R3,#-9
	BRn MORE9
	BRnzp FIN

LESS9	
	NOT R4,R4			;If len<9, we need to count the spaces needed
	ADD R4,R4,#1
	AND R3,R3,#0
	ADD R2,R4,#9		;R2 is the number of total sapces
	ADD R5,R2,#0		;Duplicate R2 into R5
COUNT_SPACE	
	ADD R3,R3,#1		;R3 is the number of pre-spaces
	ADD R5,R5,#-2		;For odd numbers: N_odd = n*2 - 1, whose n is pre-spaces
	BRp COUNT_SPACE		;For even numbers:N_even = n*2, whose n is pre-spaces 
	NOT R4,R3
	ADD R4,R4,#1
	ADD R4,R2,R4		;R4 is the number of trailing sapces
	
	ADD R0,R6,#0
SPACE1	
	OUT
	ADD R3,R3,#-1
	BRp SPACE1
	LDR R0,R1,#0
STRING	
	OUT
	ADD R1,R1,#1
	LDR R0,R1,#0
	BRnp STRING 
	
	ADD R0,R6,#0
	ADD R4,R4,#0
	BRz FIN
SPACE2	
	OUT
	ADD R4,R4,#-1
	BRp SPACE2
	BRnzp FIN
NULL
	AND R1,R1,#0
    ADD R1,R1,#9
    ADD R0,R6,#0
SPACES9 
	OUT
    ADD R1,R1,#-1
    BRp SPACES9
FIN
	LD R0,ZERO
	LD R1,ONE
	LD R2,TWO
	LD R3,THREE
	LD R4,FOUR
	LD R5,FIVE
	LD R6,SIX
	LD R7,SEVEN
	RET

ZERO	.FILL x0000
ONE		.FILL X0000
TWO		.FILL x0000
THREE	.FILL x0000
FOUR	.FILL x0000
FIVE	.FILL x0000
SIX		.FILL x0000
SEVEN	.FILL x0000
.END
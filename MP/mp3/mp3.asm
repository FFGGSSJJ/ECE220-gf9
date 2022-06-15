.ORIG	x3000
; Machine Problem 3
; Name: Guanshujie Fu
; Studnet ID: 3190110666

; This program could print a student's schedule presented as Day and Time
;   using data stored in x4000, x3800 and x4800.
    JSR INIT_SCHED
    JSR TRANS_SCHED
    ADD R0,R0,#0
    BRz MAINCODE
    JSR MP3
    ADD R0,R0,#0
    BRz MAINCODE
    JSR PRINT_SCHEDULE
MAINCODE
    HALT
;***************************************************************************************************************
; This subroutine could translate event list stored at EVENT memory locations started from x4000 
;      and store the corresponding event value into SCHEDULE memory locations started from x3800.

; EVENT label point to x4000, SCHEDULE label point to x3800
; R0 is used as temporary register
; R1 is used to store the value for PRINT_SLOT and PRINT_CENTERED
; R2 is mainly used as pointer of SCHEDULE address in print part
; R3 is mainly used as loop counter 
; R4 is mainly used to store the address of EVENT
; R5 is used to store the value in EVENT address
; R6 is used as the pointer of address
INIT_SCHED
    LD  R6,SCHEDULE
    LD  R4,COUNT
    AND R5,R5,#0    
INITIALIZE                  ; Initialize the 75 memories to x0000
    STR	R5,R6,#0
    ADD R6,R6,#1
    ADD R4,R4,#-1
    BRp INITIALIZE
    RET

TRANS_SCHED
    ST  R7,STORER7
    LD  R4,EVENT            ; Now R4 point at the bit vector field
    LDR R5,R4,#0
    ADD R3,R5,#1            ; Indicate if M[R4] is -1
    BRz TRAN_DONE         	; If M[R4] is -1, jump to the MP3**********************
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
	BRn ERROR
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
    BRz TRANS_SCHED
    LDR R4,R2,#0            ; R4 store the value in DATE to indicate which day has events
    BRn PASS2
    ADD R5,R1,R4            ; Now R5 = SLOT*5+DATE
    LD  R6,SCHEDULE
    ADD R6,R6,R5            ; Now R6 = x3800+SLOT*5+DATE
							; Indicate if there exists conflict
	LDR R5,R6,#0
	BRnp CONFLICT

    STR R0,R6,#0            ; Put the EVENT stored in R0 into target address for printing
PASS2    ADD R2,R2,#1       ; R2 points at next DATE address
    ADD R3,R3,#-1
    BRnzp CALCULATE_ADDRESS  

ERROR
	PUTS
    LEA R0,ERRORINFO1
    PUTS
    LD R7,STORER7
    AND R0,R0,#0
    RET
CONFLICT
	PUTS
	LEA R0,ERRORINFO2
	PUTS
    LD R7,STORER7
    AND R0,R0,#0
	RET
TRAN_DONE
    LD R7,STORER7
    AND R0,R0,#0
    ADD R0,R0,#1
    RET

COUNT       .FILL #75
SCHEDULE    .FILL x3800
EVENT       .FILL x4000
MINUSONE    .FILL xFFFF
DATE        .FILL xFFFF     ; date of Monday to Friday
            .FILL xFFFF     
            .FILL xFFFF     ; Initialize to -1 since 0 indicates Monday
            .FILL xFFFF
            .FILL xFFFF
STORER7		.FILL x0000
ERRORINFO1  .STRINGZ	" has an invalid slot number.\n"
ERRORINFO2  .STRINGZ	" conflicts with an earlier event.\n"
;************************************************************************************************************************
; This is the subroutine for translating extra event and checking if it can be inserted into schedule
; If it can be inserted, then print the schdule
; If not, then print error message
MP3
; EXTRA point at x4800, STACK points at x8000
; MAX_SLOT is used to store the max possible hour of each event
; Slot_indicate vector is used to excute AND instruction with slot vector to indicate the possible hour
; The stack I set will be pushed in slot_indicate vector and slot value for each event

; R0 is mainly used to store the address of the string
; R1 
; R2 is mainly used as counter and used to calculate the value of SLOT
; R3 is mainly used as pointer of DATE
; R4 is mainly used to store and caculate the DATE vector and SLOT vector, and is used as counter
; R5 is used as pointer at x4800 and KEEP R5 points at DATE_VECTOR of each event
; R6 is used as the pointer at stack
    ST R7,STORER7
    LD R6,STACK             ; x8000
    LD R5,EXTRA             ; x4800
    LD R4,MINUSONE
    STR R4,R6,#0            ; Set the element at the bottom of stack to be -1

INITIALIZE_STACK
    AND R4,R4,#0
    ADD R6,R6,#-1           ; R6 points at slot_indicate vector
    ADD R4,R4,#1            ; R4 <-- x0001
    STR R4,R6,#0            ; Set the slot_indicate vector to be x0001 at begining
    AND R4,R4,#0
    STR R4,R6,#-1           ; Set the slot value to be 0 at begining

TRANSLATE_EXTRA
    LDR R4,R5,#0            ; Load the DATE vector for current event into R4
    ADD R4,R4,#1
    BRz SUCCEED_YEAH        ; If R4 is -1, then jump to print the schedule
    ADD R4,R4,#-1

    AND R2,R2,#0
    ADD R2,R2,#11           ; R2 is a counter now to left shift the DATE vector
CHECK_DATE_VEC
    ADD R4,R4,R4
    ADD R2,R2,#-1
    BRp CHECK_DATE_VEC

    LEA R3,DATE             ; R3 point at the DATE
    LD  R0,MINUSONE         ; R0 stores the -1 to reset DATE from Mon. to Fri.
    STR R0,R3,#0
    STR R0,R3,#1
    STR R0,R3,#2
    STR R0,R3,#3
    STR R0,R3,#4

    AND R2,R2,#0
DATE_VEC_IN
    ADD R4,R4,#0            ; Check if the first bit is 1
    BRzp PASS3
    STR R2,R3,#0            ; Store the corresponding DATE value into DATE
PASS3
    ADD R3,R3,#1
    ADD R2,R2,#1
    ADD R0,R2,#-4
    BRp SLOT_VECTOR_CALCULATE
    ADD R4,R4,R4
    BRnzp DATE_VEC_IN

SLOT_VECTOR_CALCULATE
    AND R2,R2,#0
    ADD R2,R2,#14          ; R2 is used to count for max slot
    LDR R4,R5,#2           ; R4 <-- slot vector
    BRz EXTRA_FAIL
    ADD R4,R4,R4           ; Left shift one bit
FIND_SLOT_MAX
    ADD R4,R4,#0
    BRn GET_SLOT_MAX
    ADD R4,R4,R4
    ADD R2,R2,#-1
    BRnzp FIND_SLOT_MAX
GET_SLOT_MAX
    ST R2,MAX_SLOT         ; Store the max slot to be used later
    LDR R4,R5,#2           ; Reload slot vector into R4
    LDR R2,R6,#-1          ; R2 <-- slot value, initially 0
    LDR R3,R6,#0           ; R3 <-- slot_indicate vector, initialize as x0001
LOOP1
    AND R1,R3,R4
    BRp SLOT_CALCULATE     ; If indicate a slot, jump to calculate SLOT
    ADD R3,R3,R3           ; Left shift
    LD  R1,MAX_SLOT
    NOT R1,R1
    ADD R1,R1,#1           ; R1 = -MAX
    ADD R1,R2,R1           ; If found SLOT >= MAX, this event can not change time anymore
    BRzp SLOT_FAIL         ; Jump to SLOT_FAIL
    ADD R2,R2,#1           ; R2 is actually used to calculate the shift times, which is equal to SLOT
    BRnzp LOOP1
SLOT_CALCULATE
    ADD R3,R3,R3           ; Left shift one more bit to prepare for the next indication
    ADD R2,R2,#1           ; Slot also add one 
    STR R3,R6,#0           ; PUSH slot_indicate vector into stack
    ADD R6,R6,#-1          ; R6 points at slot value
    STR R2,R6,#0           ; PUSH slot value into stack
    ADD R2,R2,#-1          ; Minus the extra one 

    ADD R3,R2,#0            ; Copy R2 to R3
    ADD R2,R2,R2
    ADD R2,R2,R2
    ADD R2,R2,R3            ; R2 = 5 * SLOT
    LD R3,SCHEDULE          ; R3 <-- x3800
    ADD R2,R3,R2            ; R2 = x3800 + 5*SLOT

    LEA R3,DATE
    AND R4,R4,#0
    ADD R4,R4,#5            ; R4 is a counter as well as a flag in CLEAR part
CHECK_SCHEDULE
    LDR R1,R3,#0            ; Load the DATE value into R1
    BRn PASS4
    ADD R1,R1,R2            ; R1 = x3800 + 5*SLOT + DATE, R1 indicates the target address
    LDR R0,R1,#0            ; Check the value in target address
    BRnp CLEAR              ; If there exist value, clear the previous inserted extra value
    LDR R0,R5,#1            ; If not, load the address of string into R0
    STR R0,R1,#0            ; Store the address in 
PASS4
    ADD R3,R3,#1
    ADD R4,R4,#-1
    BRp CHECK_SCHEDULE
NULL_SLOT                   ; If the program succeed in getting here, it means current event has been inserted in and 
    ADD R5,R5,#3            ;       we need to turn to next event, such that R5 points to next event's DATE vector
                            ;       and R6 points at next event's slot_indicate vector part
    BRnzp INITIALIZE_STACK  ; Go back and begin an new loop

CLEAR
    ADD R4,R4,#0
    ADD R4,R4,#-5           ; R4 = R4 - 5
    BRz CLEAR_FINISH        ; If R4 = 5, no value was inserted in previous steps
    NOT R4,R4
    ADD R4,R4,#1            ; If R4 - 5 <= 0, being clearing
CLEAR_BEGIN
    LEA R3,DATE
    AND R0,R0,#0
CLEAR_LOOP
    LDR R1,R3,#0            ; Load DATE value into R1
    BRn PASS5
    ADD R1,R1,R2            ; For now, R2 still equal to x3800 + 5*SLOT
    STR R0,R1,#0            ; Clear this address
PASS5
    ADD R3,R3,#1
    ADD R4,R4,#-1
    BRp CLEAR_LOOP

CLEAR_FINISH                ; Now clear finishes
    LDR R4,R6,#0            ; R6 points at slot value and load current slot value into R4
    LD  R0,MAX_SLOT
    NOT R0,R0
    ADD R0,R0,#1            ; R0 = -MAX
    ADD R4,R4,R0            ; R4 = SLOT - MAX
    BRnz CHANGE_TIME         ; If current extra event can still change time to indicate if it can be inserted, change it
GOBACK
    ADD R6,R6,#2            ; POP OUT current event and points at previous slot value (or bottom of stack)
    ADD R5,R5,#-3           ; R5 also turns to previous DATE_VECTOR
    LDR R4,R6,#0
    ADD R4,R4,#1
    BRz EXTRA_FAIL          ; If the value is -1, which is set at bottom, then terminate with error
    ADD R4,R4,#-1           ; If not, check if thif event could change and clear previously inserted value

CLEAR_PREVIOUS_EVENT        ; R6 points at slot value
    LDR R3,R5,#2            ; Load current event's SLOT vector
    ADD R3,R3,R3            ; Left shift one bit
    AND R2,R2,#0
    ADD R2,R2,#14
FIND_MAX
    ADD R3,R3,#0
    BRn GET_MAX
    ADD R2,R2,#-1
    ADD R3,R3,R3
    BRzp FIND_MAX
GET_MAX
    ST R2,MAX_SLOT
    NOT R4,R4
    ADD R4,R4,#1            ; R4 <-- -SLOT
    ADD R4,R4,R2            ; Compare current slot with max slot, R4 <-- MAX - SLOT
    BRn GOBACK             ; If slot hasn't reached max, clear previouly stored value
    LDR R2,R6,#0            ; Load current slot into R2
    ADD R2,R2,#-1           ; The slot-1 hour has been inserted values
    ADD R3,R2,#0
    ADD R2,R2,R2
    ADD R2,R2,R2
    ADD R2,R2,R3            ; R2 = 5*SLOT
    LD R3,SCHEDULE
    ADD R2,R2,R3            ; R2 = x3800 + 5*SLOT, it will be used in CLEAR

    LDR R4,R5,#0            ; Load the DATE vector for current event into R4
    AND R3,R3,#0
    ADD R3,R3,#11           ; R3 is a counter now to left shift the DATE vector
CHECK_DATE_VEC2
    ADD R4,R4,R4
    ADD R3,R3,#-1
    BRp CHECK_DATE_VEC2

    LEA R3,DATE             ; R3 point at the DATE
    LD  R0,MINUSONE         ; R0 stores the -1 to reset DATE from Mon. to Fri.
    STR R0,R3,#0
    STR R0,R3,#1
    STR R0,R3,#2
    STR R0,R3,#3
    STR R0,R3,#4

    AND R1,R1,#0
DATE_VEC_IN2
    ADD R4,R4,#0            ; Check if the first bit is 1
    BRzp PASS6
    STR R1,R3,#0            ; Store the corresponding DATE value into DATE
PASS6
    ADD R3,R3,#1
    ADD R1,R1,#1
    ADD R0,R1,#-4
    BRp CLEAR_READY
    ADD R4,R4,R4
    BRnzp DATE_VEC_IN2
CLEAR_READY
    AND R4,R4,#0
    ADD R4,R4,#5
    BRnzp CLEAR_BEGIN
CHANGE_TIME
    ADD R6,R6,#1            ; POP OUT: R6 points at current event's slot_indicate vector
    BRnzp	TRANSLATE_EXTRA
SLOT_FAIL
    ADD R6,R6,#-1           ; If this event can not change time anymore,
    STR R2,R6,#0            ;   PUSH the slot value into the stack for
    BRnzp CLEAR_FINISH      ;   CLEAR_FINISH part to judge what to do.
EXTRA_FAIL
    LEA R0,FAILED
    PUTS
    AND R0,R0,#0            ; R0 = 0
    LD R7,STORER7
    RET

SUCCEED_YEAH        
    AND R0,R0,#0
    ADD R0,R0,#1            ; R0 = 1
    LD  R7,STORER7
    RET

FAILED      .STRINGZ	"Could not fit all events into schedule.\n"
MAX_SLOT    .FILL x0000
STACK       .FILL x8000
EXTRA       .FILL x4800
WHATIN      .FILL x0000
STORER7_2   .FILL x0000  
;******************************************************************************************************************************
; This subroutine would check memory locations from x3800 and use the value stored to print a schedule

; R0 is used to store the address of string
; R1 is used get the value of SLOT
; R2 is used as pointer at SCHEDULE part
; R3 is used as ROW counter = 15 as well as indication of SLOT
; R4 is used as COL counter = 5
; R5 is used as pointer at EVENT part
; R6 is used as temporary register 
PRINT_SCHEDULE  
    ST R7,STORER7_2
    LEA R0,FIRSTLINE
    PUTS
    LD  R2,SCHEDULE2
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

    LD R7,STORER7_2
    RET

PRINT_EMPTY
    LEA R1,EMPTY
    JSR PRINT_CENTERED
    BRnzp BACK

SCHEDULE2   .FILL x3800
SEPERATE    .FILL x007C     ;'|'
NEWLINE     .FILL x000A     ;'\n'
EMPTY       .FILL x0000
FIRSTLINE   .STRINGZ	"         |  MONDAY | TUESDAY |WEDNESDAY| THURSDAY|  FRIDAY \n"


;****************************************************************************************************************
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
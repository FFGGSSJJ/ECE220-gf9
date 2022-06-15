;
; The maze appears below.  The first memory location is in
; the upper left.  Subsequent locations are to the right, 
; followed by the second row from right to left, and so on.
;
;  +-+-+-+-+-+-+-+-+-+-+
;  |           | |*|*|*|
;  + +-+-+-+-+-+ +-+-+-+
;  |       |         |*|
;  + + +-+-+-+ +-+-+-+-+
;  | | |         |   |*|
;  +-+ + +-+-+ +-+ +-+-+
;  | |       |   | |   |
;  + + +-+-+-+ + + + +-+
;  |         | |     |*|
;  + + + +-+-+-+-+ +-+-+
;  | | |     |*|   |*|*|
;  + +-+ +-+-+-+-+-+-+-+
;  |   |         |*|*|*|
;  + +-+ +-+ + + +-+-+-+
;  | |     | | | |*|*|*|
;  +-+-+-+ + + + +-+-+-+
;  |*|*| | | | | |*|*|*|
;  +-+-+ + + +-+-+-+-+-+
;  |*|     |   |*|*|*|*|
;  +-+-+-+-+-+-+-+-+-+-+
;  
;

	.ORIG x3000

	JSR CLEAR_REACH		; clear the REACH array
	JSR SEARCH_MAZE		; search the maze using DFS
	JSR PRINT_REACHABLE	; print reachable locations
	HALT

N_ROOMS		.FILL #100	; number of rooms in maze (10x10)
MAZE_ADDR	.FILL MAZE	; address of the maze data
STACK_ADDR	.FILL STACK	; address of base of stack
STACK_BASE	.FILL x0000
REACH_ADDR	.FILL REACH	; address of the reach array


; CLEAR_REACH -- writes 100 zeroes into the reach array to prepare for
;                searching the maze (SEARCH_MAZE) with DFS.
; INPUTS: none
; OUTPUTS: (the REACH array)
; SIDE EFFECTS: fills REACH array with 0s
; CALLER-SAVED: R0,R1,R2,R7
; CALLEE-SAVED: R3,R4,R5,R6

CLEAR_REACH
	LD	R0,N_ROOMS	; R0 holds number of 0s to write
	AND	R1,R1,#0	; R1 is 0
	LD	R2,REACH_ADDR	; R2 is the address to be written
CR_LOOP	STR	R1,R2,#0	; write one zero
	ADD	R2,R2,#1	; advance writing pointer
	ADD	R0,R0,#-1	; decrement counter
	BRp	CR_LOOP		; loop until finished
	RET			; all done!


; SEARCH_MAZE -- marks the reach array by performing a DFS of the maze,
;                starting from room (0,0)
; INPUTS: (the MAZE)
; OUTPUTS: (the REACH array)
; SIDE EFFECTS: marks reachable rooms as 1 in the REACH array, uses the stack
; CALLER-SAVED: R0,R1,R2,R3,R6,R7
; CALLEE-SAVED: R4,R5

SEARCH_MAZE

; R0 is the location to be pushed.  We first set it to the first room.
; R1 is the difference between MAZE and REACH.  We use it to find the
;    REACH array location corresponding to a room in the maze.
; R2 and R3 are temporary registers
; R6 is the top of the stack
	LD R6,STACK_ADDR
	ADD R6,R6,#-1
	ST R6,STACK_BASE
	AND R1,R1,#0
	ADD R1,R1,#-1
	STR R1,R6,#0
	LD	R0,MAZE_ADDR	; point R0 to the room (0,0)

	LD	R2,REACH_ADDR	; set R1 to REACH - MAZE
	NOT	R1,R0		;   (R2 is a temporary here)
	ADD	R1,R1,#1
	ADD	R1,R1,R2

	LD	R6,STACK_BASE	; set up stack
	ADD R6,R6,#-1

SM_PUSH ; push room R0 onto stack (along with directions)
	STR R0,R6,#0		; PUSH the current room address into stack
	AND R3,R3,#0		; mark room R0 as reachable
	ADD R3,R3,#1
	STR R3,R6,#-1		; PUSH the x0001 into indicate vector stack
	ADD R2,R1,R0		; R2 = REACH address
	STR R3,R2,#0		; Stroe 1 into the reachable room

SM_EXPL	; does the room on top of stack have more directions?
	LDR R3,R6,#-1		; Load the indicate vector into R3
	ADD R3,R3,#-8
	BRnz SM_MOVE		; if so, go choose one
	ADD R6,R6,#1		; if not, pop from stack and indicate if it is the last one
	LDR R3,R6,#0		; Load the indicate vector of last room (or bottom of stack) into R2
	ADD R3,R3,#1		; is stack empty?
	BRz RETURN
						; if not, explore more...
	ADD R6,R6,#-1
	BRnzp BACK_ROOM

RETURN
	RET
SM_MOVE	; move to a new room
	LDR R2,R0,#0		; Load the direction vector into R2
	LDR R3,R6,#-1		; Load the indicate vector into R3
	AND R5,R5,#0
	ADD R5,R5,#4
FIND_IT
    AND R4,R2,R3
	ADD R4,R4,#-1    
	BRz LEFT_MOVE		; check for moving left
	ADD R4,R4,#-1
	BRz RIGHT_MOVE		; check for moving right
	ADD R4,R4,#-2
	BRz UP_MOVE			; check for moving up
	ADD R4,R4,#-4
	BRz DOWN_MOVE		; check for moving down
CONTINUE
	ADD R3,R3,R3
	ADD R5,R5,#-1
	BRz BACK_ROOM
	BRnzp FIND_IT
LEFT_MOVE				
	ADD R0,R0,#-1
	BRnzp VECTOR_STORE
RIGHT_MOVE				
	ADD R0,R0,#1
	BRnzp VECTOR_STORE
UP_MOVE
	ADD R0,R0,#-10
	BRnzp VECTOR_STORE
DOWN_MOVE
	ADD R0,R0,#10
	BRnzp VECTOR_STORE
BACK_ROOM
	ADD R6,R6,#2		; R6 points to last room's address
	LDR R0,R6,#0		; Load the last room's address into R0
	BRnzp SM_EXPL

VECTOR_STORE
	ADD R4,R1,R0
	LDR R4,R4,#0
	ADD R4,R4,#-1
	BRz REACHED
	ADD R3,R3,R3		; Left shift for one more bit
	STR R3,R6,#-1		; Store the indicate vector into stack
	ADD R6,R6,#-2		; Change the pointer of stack for next room
	BRnzp SM_PUSH
REACHED
	LDR R0,R6,#0		; Load R0 back
	BRnzp CONTINUE
	; be sure in each case to remove the direction chosen
	; and to check whether the new room has been reached already
	; (otherwise you will generate an infinite loop and overflow
	; the stack)



; PRINT_REACHABLE -- prints a 10x10 array of characters representing
;                    the maze.  Asterisks represent reachable locations,
;                    and periods represent unreachable locations.
; INPUTS: (the REACH array)
; OUTPUTS: none
; SIDE EFFECTS: prints to the screen
; CALLER-SAVED: R0,R1,R2,R3,R7
; CALLEE-SAVED: R4,R5,R6

PRINT_REACHABLE
	ST	R7,PR_R7	; save R7 (need to use traps)
	LD	R1,REACH_ADDR	; R1 points to the REACH array
	AND	R2,R2,#0	; R2 is a row counter
PR_NXTR	AND	R3,R3,#0	; R3 is a column counter
PR_NXTC	LDR	R0,R1,#0	; read an element from REACH array
	BRz	PR_NO_R		; if 0, not reachable
	LD	R0,ASTER	; print '*' for reachable
	BRnzp	PR_PRT
PR_NO_R	LD	R0,DOT		; print '.' for unreachable
PR_PRT	OUT			; print character to display
	ADD	R1,R1,#1	; point to next element
	ADD	R3,R3,#1	; increment column
	ADD	R0,R3,#-10	; check for end of row (discard result)
	BRn	PR_NXTC
	LD	R0,LINEF	; print line feed to display
	OUT
	ADD	R2,R2,#1	; increment row
	ADD	R0,R2,#-10	; check for end of data (discard result)
	BRn	PR_NXTR		; continue with next row
	LD	R7,PR_R7	; restore R7 (need to use traps)
	RET
; data for PRINT_REACHABLE
PR_R7	.BLKW #1	; space for return address (R7) to be stored
ASTER	.FILL x002A	; ASCII '*'
DOT	.FILL x002E	; ASCII '.'
LINEF	.FILL x000A	; ASCII line feed


; The data below represent the 10x10 maze shown at the top of the file.
; Open directions are represented by bits for each room: 1 for left, 2
; for right, 4 for up, and 8 for down.

MAZE	.FILL x000A
	.FILL x0003
	.FILL x0003
	.FILL x0003
	.FILL x0003
	.FILL x0001
	.FILL x0008
	.FILL x0000
	.FILL x0000
	.FILL x0000
	.FILL x000E
	.FILL x000B
	.FILL x0003
	.FILL x0001
	.FILL x0002
	.FILL x000B
	.FILL x0007
	.FILL x0003
	.FILL x0001
	.FILL x0000
	.FILL x0004
	.FILL x000C
	.FILL x000A
	.FILL x0003
	.FILL x0003
	.FILL x000F
	.FILL x0001
	.FILL x000A
	.FILL x0001
	.FILL x0000
	.FILL x0008
	.FILL x000E
	.FILL x0007
	.FILL x0003
	.FILL x0001
	.FILL x000E
	.FILL x0009
	.FILL x000C
	.FILL x000A
	.FILL x0001
	.FILL x000E
	.FILL x000F
	.FILL x000B
	.FILL x0003
	.FILL x0001
	.FILL x0004
	.FILL x0006
	.FILL x000F
	.FILL x0005
	.FILL x0000
	.FILL x000C
	.FILL x0004
	.FILL x000E
	.FILL x0003
	.FILL x0001
	.FILL x0000
	.FILL x0002
	.FILL x0005
	.FILL x0000
	.FILL x0000
	.FILL x000E
	.FILL x0001
	.FILL x000E
	.FILL x0003
	.FILL x000B
	.FILL x000B
	.FILL x0009
	.FILL x0000
	.FILL x0000
	.FILL x0000
	.FILL x0004
	.FILL x0002
	.FILL x0007
	.FILL x0009
	.FILL x000C
	.FILL x000C
	.FILL x000C
	.FILL x0000
	.FILL x0000
	.FILL x0000
	.FILL x0000
	.FILL x0000
	.FILL x0008
	.FILL x000C
	.FILL x000C
	.FILL x0004
	.FILL x0004
	.FILL x0000
	.FILL x0000
	.FILL x0000
	.FILL x0000
	.FILL x0002
	.FILL x0007
	.FILL x0005
	.FILL x0006
	.FILL x0001
	.FILL x0000
	.FILL x0000
	.FILL x0000
	.FILL x0000

; 256 memory locations for the stack
	.BLKW #256
STACK	; base of stack is here

; 100 memory locations for marking which locations are reachable
; in the maze
REACH	.BLKW #100

	.END

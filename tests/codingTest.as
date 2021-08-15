

jmp	iWillReturn2
jmp iWillReturn

WarnMe:	.entry	iWillReturn

iWillReturn:	.dh		1,2,14,8   
iWillReturn2:	.asciz	"He will Return!" 



jmp	iWillReturn2
jmp iWillReturn    		
;	R Commands
;				reg reg reg
HelloR:	add		$0 ,$1 ,$2  
		sub		$3 ,$4 ,$5
		and		$6 ,$7 ,$8
		or		$9 ,$10,$11
		nor		$12,$13,$14 
;				reg reg
		move	$15,$16
		mvhi	$18,$19
		mvlo	$21,$22

;	I Commands
;				reg		immed	reg
HelloI:	addi	$24  ,	123  ,  $25
		subi	$26 , 	0	,	$27  
		andi	$28,	-30,	$29
		ori		$30,-2147483648,$31 
		nori	$17,2147483647,$20
;				reg		reg		label
		bne		$23 , 	$23,	LABE1   
		beq		$16,	$6,		LABE1
		blt		$15,	$0,		LABE2  
		bgt		$20,	$15,	LABE2
;				reg		immed	reg 
		lb		$24  ,	123  ,  $25    
		sb		$26 , 	0	,	$27
		lw		$28,	-30,	$29
		sw		$30,-2147483648,$31 
		lh		$17,2147483647,$20
		sh		$26 , 	0	,	$27  
  
;	J Commands
HelloJ:	jmp		LABE3
		jmp		$7
		la		LABE3
		la		LABE4
		call	LABE4
		call	LABE5
BYE:	stop


;	Data Commands 
LABE1:	.db		0,5,-3,+80, 1,1 ,-128,127  
LABE2:	.dh		0,5,-3,+80, 1,1 ,-32768,32767 
LABE3:	.dw		0,5,-3,+80, 1,1 ,-2147483648,2147483647  
LABE4:	.asciz	"Weeeeeeeee!!!! Sting!!!!!!!"  
LABE5:	.asciz	"now for some difficult string:"
		.asciz	"!@#$%^&*()_+}{L'?><MKL"    

DoLabel2:	jmp		DoLabel
			bne		$15, $6, DoLabel2

iWillReturn:	.extern		DoLabel
iWillReturn2:	.entry		DoLabel2

.entry LABE1
.entry LABE2
.entry LABE3
.entry LABE4
.entry LABE5

.extern EXT1
.extern EXT2
.extern EXT3
.extern EXT4

		bne		$23 , 	$23,	EXT1   
		beq		$16,	$6,		EXT2
		blt		$15,	$0,		EXT3  
		bgt		$20,	$15,	EXT4


		la		EXT1
		la		EXT1
		call	EXT1
		call	EXT1		


WarnMe:	.entry	iWillReturn2


jmp iWillReturn
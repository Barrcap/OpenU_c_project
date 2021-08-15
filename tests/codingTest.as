;	R Commands
;				reg reg reg
		add		$0 ,$1 ,$2
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
		addi	$24  ,	123  ,  $25
		subi	$26 , 	0	,	$27
		andi	$28,	-30,	$29
		ori		$30,-2147483648,$31
		nori	$17,2147483647,$20
;				reg		reg		label
		bne		$23 , 	$23,	LABE1
		beq		
		blt
		bgt



MAIN:	add		$30,$5,$9
LOOP:	ori		$9,-5,$2
		la		val1
		jmp		Next
Next:	move	$20,$4
		bgt		$4,$2,END
		la		K
		sw		$0,4,$10
		bne		$31,$9,LOOP
		call	val1
		jmp		$4
END:	stop
STR:	.asciz	"aBcd"
LIST:	.db		6,-9
		.dh		27056
.entry	K
K:		.dw		31,-12
.extern	val1

;Written by Barr Caplan


.extern	ExternLabel
;These commands cant accept extern label:
        bne     $23 ,   $23,    ExternLabel   
        beq     $16,    $6,     ExternLabel
        blt     $15,    $0,     ExternLabel  
        bgt     $20,    $15,    ExternLabel
;
        jmp		NotDefined
        call	NotDefined
        bne     $23 ,   $23,    NotDefined



		add     $0 ,$1 ,$2  
        sub     $3 ,$4 ,$5
        and     $6 ,$7 ,$8
        or      $9 ,$10,$11
        nor     $12,$13,$14





########################################333
;   R Commands
;               reg reg reg
        add     $0 ,$1 ,$2  
        sub     $3 ,$4 ,$5
        and     $6 ,$7 ,$8
        or      $9 ,$10,$11
        nor     $12,$13,$14 
;               reg reg
        move    $15,$16
        mvhi    $18,$19
        mvlo    $21,$22

;   I Commands
;               reg     immed   reg
        addi    $24  ,  123  ,  $25
        subi    $26 ,   0   ,   $27  
        andi    $28,    -30,    $29
        ori     $30,-32768,$31 
        nori    $17,32767,$20
;               reg     reg     label
        bne     $23 ,   $23,    ValidLabel   
        beq     $16,    $6,     ValidLabel
        blt     $15,    $0,     ValidLabel  
        bgt     $20,    $15,    ValidLabel
;               reg     immed   reg 
        lb      $24  ,  123  ,  $25    
        sb      $26 ,   0   ,   $27
        lw      $28,    -30,    $29
        sw      $30,-32768,$31 
        lh      $17,32767,$20
        sh      $26 ,   0   ,   $27  
  
;   J Commands
        jmp     ValidLabel
        jmp     $7
        la      ValidLabel
        la      ValidLabel
        call    ValidLabel
        call    ValidLabel
        stop

.db     0,5,-3,+80, 1,1 ,-128,127  
.dh     0,5,-3,+80, 1,1 ,-32768,32767 
.dw     0,5,-3,+80, 1,1 ,-2147483648,2147483647  
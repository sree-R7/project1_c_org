ABC:
	addi $sp, $sp, 16
	sw $t3, 0($sp)
	add $t3, $t9, $t2
DEF:
	bne $t3, $t9, ABC
	beq $t3, $t1, ABC
	bne $t3, $t9, ABC
	sw $t3, 0($sp)
	jr $ra
	or	$v1, $s1, $t6
	beq $0, $zero, ABAS
	beq $0, $zero, DEF
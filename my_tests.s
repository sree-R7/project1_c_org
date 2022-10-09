L1:
	add	$t1, $t2, $t3
	add	$t1, $t2, $t3
	and	$s1, $s2, $s3
	add	$a0, $a1, $a2
	add	$v1, $s1, $t1
	add	$s4, $s5, $s6
	add	$t4, $t5, $t6
	add	$v0, $a0, $t0
	add	$a1, $a2, $a3
	lw	$t1, 4($t0)
	lw	$t2, 100($t0)
	lw	$t1, 4($t0)
	lw	$t2, 100($t0)
	beq	$t1, $t2, L2
	lw	$t2, 4($s0)
	lw	$t1, 5293($s1)
	bne	$t1, $t2, L1
L2:
	add	$s0, $s1, 2
	add	$s1, $s0, 4
	add	$t7, $t8, 256
	add	$t8, $t7, 3916
	add	$t8, $t7, 3916
	beq	$t1, $t2, L1

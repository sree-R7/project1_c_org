sub	$v0, $a0, $t0
subu	$a1, $a2, $a3
lw	$t1, 4($t0)
lw	$t2, 100($t0)
sw	$t1, 4($t0)
sw	$t2, 100($t0)
lw	$t1, 4($s0)
sw	$t1, 420($s1)
sll	$s0, $s1, 2
srl	$s1, $s0, 4
	lw	0	1	one
	add	1	1	1
	sw	1	0	one
	add	1	0	1
	nor	1	1	1
	beq	0	1	end
	noop
	noop
	noop
	lw	0	1	one
	lw	1	2	one
	add	2	2	2
end	halt
one	.fill	1
negone	.fill	-1
result	.fill	0

	lw	0	1	one
	lw	0	2	negone
	add	1	2	1
	lw	0	1	result
	beq	0	1	skip
done	lw	0	3	result
	nor	3	3	3
	lw	0	4	one
	add	1	0	1
	add	0	4	4
	add	0	0	0
	beq	1	4	end
end	halt
skip	add	2	2	2
	nor	2	1	1
	lw	0	1	one
	sw	1	3	one
	beq	0	0	done
one	.fill	1
negone	.fill	-1
result	.fill	0

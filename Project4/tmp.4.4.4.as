	lw	0	1	n
	lw	0	2	r
	lw	0	6	Caddr	load combination function address
	jalr	6	7		call function
	halt
start	sw	5	7	Stack	store return address to stack
	lw	0	6	one
	add	5	6	5
	sw	5	1	Stack	store n to stack
	add	5	6	5
	sw	5	2	Stack	store r to stack
	add	5	6	5
	sw	5	4	Stack	store return value
	add	5	6	5
	beq	0	2	base	r==0
	beq	1	2	base	r==n
	lw	0	6	negone
	add	1	6	1	n=n-1
	lw	0	6	Caddr
	jalr	6	7
	add	0	3	4
	lw	0	6	negone
	add	2	6	2	r=r-1
	lw	0	6	Caddr
	jalr	6	7
	add	4	3	3
	beq	0	0	skip
base	add	0	6	3
skip	lw	0	6	negone
	add	5	6	5	decrement stack pointer
	lw	5	4	Stack	load return value
	add	5	6	5	decrement stack pointer
	lw	5	2	Stack	load original r
	add	5	6	5	decrement stack pointer
	lw	5	1	Stack	load original n
	add	5	6	5	decrement stack pointer
	lw	5	7	Stack	load original return address
	jalr	7	6
n	    .fill	7
r	    .fill	3
one	    .fill	1
negone	.fill	-1
Caddr	.fill	start
Stack	.fill	0

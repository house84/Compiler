LOAD	1
STORE	x
READ	x
LOAD	1
STORE	T0
LOAD	1
STORE	T1
LOAD	2
SUB	T1
STORE	T2
LOAD	x
MULT    -1
STORE	T3
LOAD	1
ADD	T3
STORE	T4
LOAD	x
ADD	T4
DIV	T2
SUB	T0
BRNEG	L0
BRPOS	L0
LOAD	1
STORE	T5
WRITE	T5
L0:	NOOP
STOP
x	0
T0	0
T1	0
T2	0
T3	0
T4	0
L0	0
T5	0

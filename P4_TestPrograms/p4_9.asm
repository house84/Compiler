LOAD	1
STORE	T0
LOAD	1
SUB	T0
BRNEG	L0
BRPOS	L0
LOAD	1
STORE	T1
WRITE	T1
L0:	NOOP
STOP
T0	0
L0	0
T1	0

LOAD	1
STORE	x
LOAD	2
STORE	y
LOAD	2
STORE	x
LOAD	5
STORE	z
LOAD	2
STORE	z
READ	x
LOAD	y
STORE	T0
LOAD	x
MULT	T0
STORE	T1
WRITE	T1
LOAD	z
STORE	T2
WRITE	T2
STOP
x	0
y	0
z	0
T0	0
T1	0
T2	0

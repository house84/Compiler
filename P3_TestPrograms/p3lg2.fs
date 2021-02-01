#p3lg2
start
let x : 1
let y : 2
main {
  let x : 3
  printf [ x + y ] .
  if [  x : : y ] then
  { let x : 4
    iter [ x =< y ]
    { printf [ x ] .
    } .
    printf [ x ] .
  } .
} stop

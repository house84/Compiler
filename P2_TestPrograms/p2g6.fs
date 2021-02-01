#p2g6
start
let x : 1
let y : 2
main {
  let z : 3
  scanf [ x ] .
  printf [ y + z / 3 ] .
  if [  x : : y ] then
  { let w : 4
    iter [ x =< 1 + 2 ]
    { printf [ 1 ] .
    } .
    printf [ 2 ] .
  } .
} stop
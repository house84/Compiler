start       #p3lb4 undefined y line 13
let x : 1
main {
  let w : 3
  printf [ x + y + w ] .
  if [  x : : y ] then
  { let y : 4
    iter [ w =< y ]
    { printf [ x ] .
    } .
    printf [ w ] .
  } .
    scanf [ y ] .
  } stop


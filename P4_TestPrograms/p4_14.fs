start
let x : 1
let y : 2
main {
  scanf [ x ] .
  iter [ x => 1 ]
  { y = x .
    iter [ y => 1 ]
    { printf [ y ] .
      y = y - 1 .
    } .
    x = x - 1 .
  } .
} stop

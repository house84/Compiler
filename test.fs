start
let x : 2
let y : 0
let z : 0
main
{
scanf [ x ].
    z = x .
    x = x * x .
    x = x / z .
    iter [ y == 0 ]
    {
        x = x - 2 .
        if [ x == 0 ] then
        {
            printf[ 1 ].
            y = 1 .
        } .
        if [ x == 1 ] then
        {
            y = 1 .
        } .
        if [ x =< 0 ] then
        {
            y = 1 .
        } .
    } .
}

stop
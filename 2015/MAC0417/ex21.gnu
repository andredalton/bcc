set terminal jpeg
F(x) = 14*sin(5*x)/x;
f(x) = ( x > -5 && x < 5) ? 7 : 0;
set samples 10000
plot F(x), f(x);
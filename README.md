# mcalc
Front-end to GMP

It is a basic UI of a multi-precision calculator.


Compile and run from the terminal using:
clang main.c -std=c89 -Wall -Wextra -pedantic -g -O2 -o mcalc -lglfw -lGL -lm -lGLU && ./mcalc


ATM it only does integer calculations. And with very basic parsing for now (no support for brackets).

Try calculating the largest prime number with it. https://en.wikipedia.org/wiki/Largest_known_prime_number

![mcalc](https://user-images.githubusercontent.com/19549703/164498824-fc521903-d376-4a63-9a78-52ddb58988d2.png)


License: GPL v2
(C) Nuno Ferreira 2022

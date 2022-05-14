# mcalc

***mcalc*** is a graphical **m**ulti-precision **calc**ulator, which focus on **Number Theory**.

It is a graphical front-end to [**GMP**](https://gmplib.org/).

Screenshot:

![Screenshot at 2022-05-13 21-21-33](https://user-images.githubusercontent.com/19549703/168386805-2a4f0a29-a979-481b-b53a-c1922bdf195b.png)

If the result is longer than 34 digits, it will be displayed in the terminal.



## Compile and run:

On UNIX-like:

    gcc main.c -std=c99 -Wall -Wextra -pedantic -O2 -o mcalc -lglfw -lGL -lm -lGLU -lgmp && ./mcalc

If there is a request for it, I am happy to release a version for **MS Windows**.



## Try calculating the largest prime number known:

Try **calculating** the largest prime number with it. https://en.wikipedia.org/wiki/Largest_known_prime_number

![mcalc](https://user-images.githubusercontent.com/19549703/164498824-fc521903-d376-4a63-9a78-52ddb58988d2.png)

Advantages of using mcalc are the speed and ease of use (when compared to programming tools like python or bc).



## List of GMP functions wrapped up so far ##

Currently, the following functions are available to use directly:

- mpz_mul();
- mpz_div();
- mpz_sub();
- mpz_add();
- mpz_pow_ui();
- mpz_fac_ui();
- mpz_probab_prime_p();
- mpz_nextprime();
- mpz_sqrt();
- mpz_primorial_ui().
- mpz_fib_ui();

Feel free to place requests for more functions in the "issues" section :P



## License ##

**GPL v2**

(C)2022 Nuno Ferreira

Uses the Nuklear GUI toolkit 
https://github.com/Immediate-Mode-UI/Nuklear

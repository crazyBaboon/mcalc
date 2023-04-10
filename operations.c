#include "operations.h"

#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void check_if_calculation_is_new(bool*    start_new_calc_flag, 
                                 char*       text_to_display_on_screen,
                                 const char* digit)
{
    /* Check if this is a new calculation */
    if (*start_new_calc_flag == true)
    {
        *start_new_calc_flag = false;

        strcpy(text_to_display_on_screen, "");
        strcat(text_to_display_on_screen, digit);
    }
}

void process_digit_button(bool*    start_new_calc_flag, 
                          char*       text_to_display_on_screen,
                          char*       text,
                          const char* digit)
{
                strcat(text, digit);
                strcat(text_to_display_on_screen, digit);

                check_if_calculation_is_new(start_new_calc_flag, 
                                            text_to_display_on_screen, 
                                            digit);
}

void factorial_button(bool* start_new_calc_flag, 
                      mpz_t    operand_1m, 
                      char*    text, 
                      char     text_to_display_on_screen[128], 
                      int*     Operation) 
{

        if (*start_new_calc_flag == true)
        {
            *start_new_calc_flag = false;

            strcpy(text_to_display_on_screen, "");
            strcat(text_to_display_on_screen, "ans!");
        }
        else
        {
            mpz_init_set_str(operand_1m, text, 10);
            strcpy(text, "");
            strcat(text_to_display_on_screen, "! ");
        }
        *Operation = FACTORIAL;
}

void equals_button(bool* start_new_calc_flag, 
                   mpz_t    operand_1m, 
                   mpz_t    operand_2m, 
                   char*    text, 
                   char     text_to_display_on_screen[128], 
                   int*     Operation,
                   char*    box_buffer,
                   char*    result_str,
                   size_t*  number_of_digits_to_display)
{
	mpz_t result_m;
    mpz_init(result_m); /* initialize result_m */

    if (*start_new_calc_flag == false)
    {
        mpz_init_set_str (operand_2m, text, 10);
        strcpy(text, "");
        strcat(text_to_display_on_screen, " = ");
    }
    else
    {
        strcpy(text, "");
    }

    switch (*Operation)
    {
        case FACTORIAL:
        {
            printf("%lu! =\n", mpz_get_ui(operand_1m));
            mpz_fac_ui(result_m, mpz_get_ui(operand_1m));
            break;
        }
    }

    *number_of_digits_to_display = mpz_sizeinbase(result_m, 10);  /* check the size of the result in base 10 */
    text       = realloc(text, *number_of_digits_to_display + 2); /* +2 for eventual negative sign and null terminator */
    box_buffer = realloc(box_buffer, *number_of_digits_to_display + 2);
    result_str = realloc(result_str, *number_of_digits_to_display + 2);

    gmp_sprintf(result_str, "%Zd", result_m);
    strcpy(box_buffer, result_str);

    puts("----------");

    mpz_out_str(stdout,10,result_m); 
    printf("\nNumber_of_digits is %lu.\n", *number_of_digits_to_display);

    if (*number_of_digits_to_display > 36)
    {
        sprintf(box_buffer, "Wow %lu digits!", *number_of_digits_to_display);
    }
    mpz_set(operand_1m, result_m);
    mpz_clear(result_m); /* we don't need 'result_m' anymore */
    *start_new_calc_flag = true;
}

void reset_button(char* text, 
                  char  text_to_display_on_screen[128], 
                  char* box_buffer) 
{
    strcpy(text, "");
    strcpy(text_to_display_on_screen, "");
    strcpy(box_buffer, "");
}

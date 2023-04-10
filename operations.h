#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <gmp.h>
#include <stdbool.h>

enum {SUM, 
      SUBTRACT, 
      MULTIPLY, 
      DIVIDE, 
      POW, 
      FACTORIAL, 
      PRIMORIAL, 
      FIBONACCI};


void check_if_calculation_is_new(bool*    start_new_calc_flag, 
                                 char*       text_to_display_on_screen,
                                 const char* digit);


void process_digit_button(bool*    start_new_calc_flag, 
                          char*       text_to_display_on_screen,
                          char*       text,
                          const char* digit);

void factorial_button(bool* start_new_calc_flag, 
                      mpz_t    operand_1m, 
                      char*    text, 
                      char     text_to_display_on_screen[128], 
                      int*     Operation); 


void equals_button(bool* start_new_calc_flag, 
                   mpz_t    operand_1m, 
                   mpz_t    operand_2m, 
                   char*    text, 
                   char     text_to_display_on_screen[128], 
                   int*     Operation,
                   char*    box_buffer,
                   char*    result_str,
                   size_t*  number_of_digits_to_display);

void reset_button(char* text, 
                  char  text_to_display_on_screen[128], 
                  char* box_buffer);

#endif

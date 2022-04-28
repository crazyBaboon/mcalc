/*
 * mcalc - multiple precision calculator
 *
 * Copyright (C)2022 Nuno Ferreira
 *
 * mcalc is free software; you can redistribute/modify it under 
 * the terms of the GNU General Public License version 2.
 * 
 * Compile & run on Unix-like systems:
 * clang main.c -std=c89 -Wall -Wextra -pedantic -g -O2 -o mcalc -lglfw -lGL -lm -lGLU && ./mcalc 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <gmp.h>

#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL2_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl2.h"

#define WINDOW_WIDTH 240
#define WINDOW_HEIGHT 250

enum {SUM, SUBTRACT, MULTIPLY, DIVIDE, POW, FACTORIAL, IS_N_PRIME};

void check_if_calculation_is_new(nk_bool*    start_new_calc_flag, 
                                 char*       text_to_display_on_screen,
                                 const char* digit)
{
    /* Check if this is a new calculation */
    if (*start_new_calc_flag == nk_true)
    {
        *start_new_calc_flag = nk_false;

        strcpy(text_to_display_on_screen, "");
        strcat(text_to_display_on_screen, digit);
    }
}

void process_digit_key(nk_bool*    start_new_calc_flag, 
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

static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}



int main(void)
{
    /* Platform */
    static GLFWwindow *win;
    int width = 0, height = 0;
    struct nk_context *ctx;

    /* Calculator variables */
    nk_bool start_new_calc_flag = nk_true;
    nk_bool is_dividing_by_zero = nk_false;
    int is_number_prime = 0;
    mpz_t operand_1m, operand_2m, result_m;
    static size_t number_of_digits_to_display = 0;
    static char text_to_display_on_screen[128] = "";
    char* text;
    char* result_str;
    static char* box_buffer;
    text       = malloc(3212);
    box_buffer = malloc(3212);
    result_str = malloc(3212);


    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "mcalc", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwGetWindowSize(win, &width, &height);

    /* GUI */
    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);

    {
        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        nk_glfw3_font_stash_end();
    }

    while (!glfwWindowShouldClose(win))
    {
        /* Input */
        glfwPollEvents();
        nk_glfw3_new_frame();

        /* GUI */
        if (nk_begin(ctx, "mcalc", nk_rect(0, -20, 240, 290),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            int Operation;

            nk_layout_row_static(ctx, 30, 173, 1);
            nk_label(ctx, text_to_display_on_screen, NK_TEXT_LEFT);


            nk_layout_row_static(ctx, 30, 215, 1);
            if (number_of_digits_to_display > 36)
            {
                nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX, text_to_display_on_screen, 128, nk_filter_default);
            }            
            else
                nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX, box_buffer, number_of_digits_to_display + 2, nk_filter_default);

            nk_layout_row_static(ctx, 30, 40, 5);
            if (nk_button_label(ctx, "7"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "7");
            if (nk_button_label(ctx, "8"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "8");
            if (nk_button_label(ctx, "9"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "9");
            if (nk_button_label(ctx, "x"))
            {
                mpz_init_set_str (operand_1m, text, 10);
                strcpy(text, "");
                strcat(text_to_display_on_screen, " x ");
                Operation = MULTIPLY;
            }   
            if (nk_button_label(ctx, "C"))
            {
                strcpy(text, "");       
                strcpy(text_to_display_on_screen, "");
                strcpy(box_buffer, "");
            }   
  
               
            nk_layout_row_static(ctx, 30, 40, 5);
            if (nk_button_label(ctx, "4"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "4");
            if (nk_button_label(ctx, "5"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "5");
            if (nk_button_label(ctx, "6"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "6");
            if (nk_button_label(ctx, "-"))
            {
                mpz_init_set_str (operand_1m, text, 10);
                strcpy(text, "");
                strcat(text_to_display_on_screen, " - ");
                Operation = SUBTRACT;
            }
            if (nk_button_label(ctx, "^"))
            {
                mpz_init_set_str (operand_1m, text, 10);
                strcpy(text, "");
                strcat(text_to_display_on_screen, " ^ ");
                Operation = POW;
            }       
                           

            nk_layout_row_static(ctx, 30, 40, 5);
            if (nk_button_label(ctx, "1"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "1");
            if (nk_button_label(ctx, "2"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "2");
            if (nk_button_label(ctx, "3"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "3");
            if (nk_button_label(ctx, "+"))
            {
                mpz_init_set_str (operand_1m, text, 10);
                strcpy(text, "");
                strcat(text_to_display_on_screen, " + ");
                Operation = SUM;
            } 
            if (nk_button_label(ctx, "!"))
            {
                mpz_init_set_str (operand_1m, text, 10);
                strcpy(text, "");
                strcat(text_to_display_on_screen, "! ");
                Operation = FACTORIAL;
            }                
           
            nk_layout_row_static(ctx, 30, 40, 5);
            if (nk_button_label(ctx, "0"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "0");
            if (nk_button_label(ctx, ""))
            {
            }

            if (nk_button_label(ctx, "="))
            {
                mpz_init_set_str (operand_2m, text, 10);

                mpz_init(result_m); /* initialize result_m */
                strcpy(text, "");
                strcat(text_to_display_on_screen, " = ");

                if (Operation == MULTIPLY)
                    mpz_mul(result_m, operand_1m, operand_2m);
                else if (Operation == DIVIDE)
                    {
                        if (mpz_cmp_ui(operand_2m, 0) == 0) /* don't divide by 0 */
                            {
                                is_dividing_by_zero = nk_true;
                                strcpy(text_to_display_on_screen, "");
                                strcat(text_to_display_on_screen, "You are dividing by 0! Noooooo");
                                printf("You are dividing by 0! Don't do it.\n");
                            }
                        else
                            {
                                mpz_div(result_m, operand_1m, operand_2m);
                                is_dividing_by_zero = nk_false;
                            }
                    }
                else if (Operation == SUBTRACT)
                    mpz_sub(result_m, operand_1m, operand_2m);
                else if (Operation == POW)
                    mpz_pow_ui(result_m, operand_1m, mpz_get_ui(operand_2m));    
                else if (Operation == SUM)
                    mpz_add(result_m, operand_1m, operand_2m);
                else if (Operation == FACTORIAL)
                    mpz_fac_ui(result_m, mpz_get_ui(operand_1m));
                else if (Operation == IS_N_PRIME)
                    is_number_prime = mpz_probab_prime_p(operand_1m, 20);                    

                number_of_digits_to_display = mpz_sizeinbase(result_m, 10);
                text       = realloc(text, number_of_digits_to_display + 2); /* +2 for eventual negative sign and null terminator */
                box_buffer = realloc(box_buffer, number_of_digits_to_display + 2);
                result_str = realloc(result_str, number_of_digits_to_display + 2);

                gmp_sprintf(result_str, "%Zd", result_m);
                strcpy(box_buffer, result_str);

                puts("----------");

                if (Operation == MULTIPLY)
                    printf("%lu x %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));
                else if (Operation == DIVIDE)
                {
                    if (is_dividing_by_zero == nk_false)
                        printf("%lu / %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));
                }                
                else if (Operation == SUBTRACT)
                    printf("%lu - %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));
                else if (Operation == POW)
                    printf("%lu ^ %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));    
                else if (Operation == SUM)
                    printf("%lu + %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));
                else if (Operation == FACTORIAL)
                    printf("%lu! =\n", mpz_get_ui(operand_1m));
					 else if (Operation == IS_N_PRIME)
					 {
					     if (is_number_prime == 0)
					     {
					         printf("%lu is definately not prime.\n", mpz_get_ui(operand_1m));
					         strcpy(text_to_display_on_screen, "definately NOT prime");
					         strcpy(box_buffer, "");
					     }
					     else if (is_number_prime == 1)
					     {
					         printf("%lu is probably a prime.\n", mpz_get_ui(operand_1m));
					         strcpy(text_to_display_on_screen, "probably prime.");
					         strcpy(box_buffer, "");
					     }					 
					     else if (is_number_prime == 2)
					     {
					     	 printf("%lu is definately prime.\n", mpz_get_ui(operand_1m));
					         strcpy(text_to_display_on_screen, "definately prime.");
					         strcpy(box_buffer, "");
					     }
					 }
                   
                    
                mpz_out_str(stdout,10,result_m); 
                printf("\nNumber_of_digits is %lu.\n", number_of_digits_to_display);

                if (number_of_digits_to_display > 36)
                {
                    sprintf(text_to_display_on_screen, "Wow %lu digits!", number_of_digits_to_display);
                }

                start_new_calc_flag = nk_true;
            }
            if (nk_button_label(ctx, "/"))
            {
                mpz_init_set_str (operand_1m, text, 10);
                strcpy(text, "");
                strcat(text_to_display_on_screen, " / ");
                Operation = DIVIDE;
            }  
            if (nk_button_label(ctx, "p?"))
            {
                mpz_init_set_str (operand_1m, text, 10);
                strcpy(text, "");
                strcat(text_to_display_on_screen, " prime? ");
                Operation = IS_N_PRIME;
            }                     
            
            
        }
        nk_end(ctx);

        /* Draw */
        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.10f, 0.18f, 0.24f, 1.0f);
        nk_glfw3_render(NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(win);
    }
    nk_glfw3_shutdown();
    glfwTerminate();
    return 0;
}

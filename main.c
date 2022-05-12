/*
 * mcalc - multiple precision calculator
 *
 * Copyright (C)2022 Nuno Ferreira
 *
 * mcalc is free software; you can redistribute/modify it under 
 * the terms of the GNU General Public License version 2.
 * 
 * Compile & run on Unix-like systems:
 * clang main.c -std=c99 -Wall -Wextra -pedantic -O2 -o mcalc -lglfw -lGL -lm -lGLU -lgmp && ./mcalc 
 */

#include <stddef.h>
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

#define WINDOW_WIDTH 290
#define WINDOW_HEIGHT 250
#define MAX_DIGITS_DISPLAY 34

enum {SUM, SUBTRACT, MULTIPLY, DIVIDE, POW, FACTORIAL, PRIMORIAL};

void mem_realloc(char* text, char* box_buffer, char* result_str, size_t length)
{
    text       = realloc(text, length + 2); /* +2 for eventual negative sign and null terminator */
    box_buffer = realloc(box_buffer, length + 2);
    result_str = realloc(result_str, length + 2);
}

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
    struct nk_context *ctx;
    int width = 0, height = 0;

    /* Calculator variables */
    nk_bool start_new_calc_flag = nk_true;
    mpz_t operand_1m, operand_2m;
    static size_t number_of_digits_to_display = 0;
    static char text_to_display_on_screen[128] = "";
    char* text;
    int Operation = SUM;
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
        if (nk_begin(ctx, "mcalc", nk_rect(0, -20, WINDOW_WIDTH, WINDOW_HEIGHT + 30),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            nk_layout_row_static(ctx, 30, 200, 1);
            nk_label(ctx, text_to_display_on_screen, NK_TEXT_LEFT);


            nk_layout_row_static(ctx, 30, 260, 1);
            if (number_of_digits_to_display > 36)
            {
                nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX, text_to_display_on_screen, 128, nk_filter_default);
            }            
            else
                nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX, box_buffer, number_of_digits_to_display + 2, nk_filter_default);

            nk_layout_row_static(ctx, 30, 40, 6);
            if (nk_button_label(ctx, "7"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "7");
            if (nk_button_label(ctx, "8"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "8");
            if (nk_button_label(ctx, "9"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "9");
            if (nk_button_label(ctx, "x"))
            {
                if (start_new_calc_flag == nk_true)
                {
                    start_new_calc_flag = nk_false;

                    strcpy(text_to_display_on_screen, "");
                    strcat(text_to_display_on_screen, "ans x ");
                }
                else
                {
                    mpz_init_set_str (operand_1m, text, 10);
                    strcpy(text, "");
                    strcat(text_to_display_on_screen, " x ");
                }
                Operation = MULTIPLY;
            }   
            if (nk_button_label(ctx, "C"))
            {
                strcpy(text, "");       
                strcpy(text_to_display_on_screen, "");
                strcpy(box_buffer, "");
            }   
            if (nk_button_label(ctx, "np"))
            {
                mpz_t result_m;
                mpz_init(result_m); /* initialize result_m */

                if (start_new_calc_flag == nk_true)
                {
                    start_new_calc_flag = nk_false;

                    strcpy(text_to_display_on_screen, "");
                }
                else
                {
                    mpz_init_set_str (operand_1m, text, 10);
                    strcpy(text, "");
                    strcpy(text_to_display_on_screen, "");
                }

                puts("----------");
                printf("Next prime greater than\n");
                mpz_out_str(stdout,10,operand_1m); 
                printf("\nis:\n");
                mpz_nextprime(result_m, operand_1m);

                number_of_digits_to_display = mpz_sizeinbase(result_m, 10);
                mem_realloc(text, box_buffer, result_str, number_of_digits_to_display);

                gmp_sprintf(result_str, "%Zd", result_m);
                strcpy(box_buffer, result_str);

                mpz_out_str(stdout,10,result_m); 
                printf("\nNumber_of_digits is %lu.\n", number_of_digits_to_display);

                if (number_of_digits_to_display > MAX_DIGITS_DISPLAY)
                {
                    sprintf(text_to_display_on_screen, "Wow %lu digits!", number_of_digits_to_display);
                }
                start_new_calc_flag = nk_true;
                mpz_set(operand_1m, result_m);
                mpz_clear(result_m); /* we don't need 'result_m' anymore */
            }   
               
            nk_layout_row_static(ctx, 30, 40, 6);
            if (nk_button_label(ctx, "4"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "4");
            if (nk_button_label(ctx, "5"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "5");
            if (nk_button_label(ctx, "6"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "6");
            if (nk_button_label(ctx, "-"))
            {
                if (start_new_calc_flag == nk_true)
                {
                    start_new_calc_flag = nk_false;

                    strcpy(text_to_display_on_screen, "");
                    strcat(text_to_display_on_screen, "ans - ");
                }
                else
                {
                    mpz_init_set_str (operand_1m, text, 10);
                    strcpy(text, "");
                    strcat(text_to_display_on_screen, " - ");
                }
                Operation = SUBTRACT;
            }
            if (nk_button_label(ctx, "^"))
            {
                if (start_new_calc_flag == nk_true)
                {
                    start_new_calc_flag = nk_false;

                    strcpy(text_to_display_on_screen, "");
                    strcat(text_to_display_on_screen, "ans ^ ");
                }
                else
                {                    
                    mpz_init_set_str (operand_1m, text, 10);
                    strcpy(text, "");
                    strcat(text_to_display_on_screen, " ^ ");
                }
                Operation = POW;
            }
            if (nk_button_label(ctx, "p?"))
            {
                if (start_new_calc_flag == nk_false)
                {
                    mpz_init_set_str (operand_1m, text, 10);
                    strcpy(text, "");
                    strcpy(text_to_display_on_screen, "");
                }

                puts("----------");
                mpz_out_str(stdout,10,operand_1m); 
                printf("\nis:\n");
                switch (mpz_probab_prime_p(operand_1m, 20))
                {
                    case 0:
                    {
                        printf("definately not prime.\n");
                        strcpy(text_to_display_on_screen, "definately NOT prime");
                        strcpy(box_buffer, "");
                        break;
                    }
                    case 1:
                    {
                        printf("is probably a prime.\n");
                        strcpy(text_to_display_on_screen, "probably prime.");
                        strcpy(box_buffer, "");
                        break;
                    }					 
                    case 2:
                    {
                        printf("is definately prime.\n");
                        strcpy(text_to_display_on_screen, "definately prime.");
                        strcpy(box_buffer, "");
                        break;
                    }
                }
                start_new_calc_flag = nk_true; 
            }   

            nk_layout_row_static(ctx, 30, 40, 6);
            if (nk_button_label(ctx, "1"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "1");
            if (nk_button_label(ctx, "2"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "2");
            if (nk_button_label(ctx, "3"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "3");
            if (nk_button_label(ctx, "+"))
            {
                if (start_new_calc_flag == nk_true)
                {
                    start_new_calc_flag = nk_false;

                    strcpy(text_to_display_on_screen, "");
                    strcat(text_to_display_on_screen, "ans + ");
                }
                else
                {
                    mpz_init_set_str (operand_1m, text, 10);
                    strcpy(text, "");
                    strcat(text_to_display_on_screen, " + ");
                }
                Operation = SUM;
            } 
            if (nk_button_label(ctx, "!"))
            {
                if (start_new_calc_flag == nk_true)
                {
                    start_new_calc_flag = nk_false;

                    strcpy(text_to_display_on_screen, "");
                    strcat(text_to_display_on_screen, "ans!");
                }
                else
                {
                    mpz_init_set_str (operand_1m, text, 10);
                    strcpy(text, "");
                    strcat(text_to_display_on_screen, "! ");
                }
                Operation = FACTORIAL;
            }
            if (nk_button_label(ctx, "#"))
            {
                if (start_new_calc_flag == nk_true)
                {
                    start_new_calc_flag = nk_false;

                    strcpy(text_to_display_on_screen, "");
                    strcat(text_to_display_on_screen, "ans#");
                }
                else
                {
                    mpz_init_set_str (operand_1m, text, 10);
                    strcpy(text, "");
                    strcat(text_to_display_on_screen, "# ");
                }
                Operation = PRIMORIAL;
            }              
            nk_layout_row_static(ctx, 30, 40, 5);
            if (nk_button_label(ctx, "0"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "0");
            if (nk_button_label(ctx, ""))
            {
            }

            if (nk_button_label(ctx, "="))
            {
                mpz_t result_m;
                mpz_init(result_m); /* initialize result_m */

                if (start_new_calc_flag == nk_false)
                {
                    mpz_init_set_str (operand_2m, text, 10);
                    strcpy(text, "");
                    strcat(text_to_display_on_screen, " = ");
                }
                else
                {
                    strcpy(text, "");
                }

                switch (Operation)
                {
                    case MULTIPLY:
                    {
                        mpz_mul(result_m, operand_1m, operand_2m);                    
                        printf("%lu x %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));
                        break;
                    }
                    case DIVIDE:
                    {
                        if (mpz_cmp_ui(operand_2m, 0) == 0) /* don't divide by 0 */
                        {
                            strcpy(text_to_display_on_screen, "");
                            strcat(text_to_display_on_screen, "You are dividing by 0! Noooooo");
                            printf("You are dividing by 0! Don't do it.\n");
                        }
                        else
                        {
                            mpz_div(result_m, operand_1m, operand_2m);
                            printf("%lu / %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));
                        }
                        break;
                    }
                    case SUBTRACT:
                    {
                        mpz_sub(result_m, operand_1m, operand_2m);
                        printf("%lu - %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));
                        break;
                    }                
                    case POW:
                    {
                        mpz_pow_ui(result_m, operand_1m, mpz_get_ui(operand_2m));   
                        printf("%lu ^ %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m)); 
                        break;
                    }                
                    case SUM:
                    {
                        mpz_add(result_m, operand_1m, operand_2m);
                        printf("%lu + %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));
                        break;
                    }
                    case FACTORIAL:
                    {
                        printf("%lu! =\n", mpz_get_ui(operand_1m));
                        mpz_fac_ui(result_m, mpz_get_ui(operand_1m));
                        break;
                    }
                    case PRIMORIAL:
                    {
                        printf("%lu! =\n", mpz_get_ui(operand_1m));
                        mpz_primorial_ui(result_m, mpz_get_ui(operand_1m));
                        break;
                    }
                }

                number_of_digits_to_display = mpz_sizeinbase(result_m, 10);
                mem_realloc(text, box_buffer, result_str, number_of_digits_to_display);

                gmp_sprintf(result_str, "%Zd", result_m);
                strcpy(box_buffer, result_str);

                puts("----------");

                mpz_out_str(stdout,10,result_m); 
                printf("\nNumber_of_digits is %lu.\n", number_of_digits_to_display);

                if (number_of_digits_to_display > 36)
                {
                    sprintf(text_to_display_on_screen, "Wow %lu digits!", number_of_digits_to_display);
                }
                mpz_set(operand_1m, result_m);
                mpz_clear(result_m); /* we don't need 'result_m' anymore */
                start_new_calc_flag = nk_true;
            }
            if (nk_button_label(ctx, "/"))
            {
                if (start_new_calc_flag == nk_true)
                {
                    start_new_calc_flag = nk_false;

                    strcpy(text_to_display_on_screen, "");
                    strcat(text_to_display_on_screen, "ans / ");
                }
                else
                {
                    mpz_init_set_str (operand_1m, text, 10);
                    strcpy(text, "");
                    strcat(text_to_display_on_screen, " / ");
                }
                Operation = DIVIDE;
            }  
            if (nk_button_label(ctx, "sq()"))
            {
                mpz_t result_m;
                mpz_init(result_m); /* initialize result_m */

                if (start_new_calc_flag == nk_true)
                {
                    start_new_calc_flag = nk_false;

                    strcpy(text_to_display_on_screen, "");
                }
                else
                {
                    mpz_init_set_str (operand_1m, text, 10);
                    strcpy(text, "");
                    strcpy(text_to_display_on_screen, "");
                }

                puts("----------");
                printf("sqrt() of \n");
                mpz_out_str(stdout,10,operand_1m); 
                printf("\nis:\n");
                mpz_sqrt(result_m, operand_1m);

                number_of_digits_to_display = mpz_sizeinbase(result_m, 10);
                mem_realloc(text, box_buffer, result_str, number_of_digits_to_display);

                gmp_sprintf(result_str, "%Zd", result_m);
                strcpy(box_buffer, result_str);

                mpz_out_str(stdout,10,result_m); 
                printf("\nNumber_of_digits is %lu.\n", number_of_digits_to_display);

                if (number_of_digits_to_display > 36)
                {
                    sprintf(text_to_display_on_screen, "Wow %lu digits!", number_of_digits_to_display);
                }
                start_new_calc_flag = nk_true;
                mpz_set(operand_1m, result_m);
                mpz_clear(result_m); /* we don't need 'result_m' anymore */
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

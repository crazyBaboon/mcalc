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

enum {SUM, SUBTRACT, MULTIPLY, DIVIDE, POW, FACTORIAL};

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
    mpz_t operand_1m, operand_2m, result_m;
    static size_t number_of_digits_to_display = 0;
    char text_to_display_on_screen[128] = "";
    char* text;
    char* result_str;
    static char* box_buffer;
    text = malloc(3212);
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
                char text_tmp[128];
                sprintf(text_tmp, "Wow %lu digits!", number_of_digits_to_display);
                nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX, text_tmp, 128, nk_filter_default);
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
           
            nk_layout_row_static(ctx, 30, 40, 4);
            if (nk_button_label(ctx, "0"))
                process_digit_key(&start_new_calc_flag, text_to_display_on_screen, text, "0");
            if (nk_button_label(ctx, "."))
            {
                strcat(text, ".");
                strcat(text_to_display_on_screen, ".");
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
                    mpz_div(result_m, operand_1m, operand_2m);
                else if (Operation == SUBTRACT)
                    mpz_sub(result_m, operand_1m, operand_2m);
                else if (Operation == POW)
                    mpz_pow_ui(result_m, operand_1m, mpz_get_ui(operand_2m));    
                else if (Operation == SUM)
                    mpz_add(result_m, operand_1m, operand_2m);
                else if (Operation == FACTORIAL)
                    mpz_fac_ui(result_m, mpz_get_ui(operand_1m));

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
                    printf("%lu / %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));
                else if (Operation == SUBTRACT)
                    printf("%lu - %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));
                else if (Operation == POW)
                    printf("%lu ^ %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));    
                else if (Operation == SUM)
                    printf("%lu + %lu =\n", mpz_get_ui(operand_1m), mpz_get_ui(operand_2m));
                else if (Operation == FACTORIAL)
                    printf("%lu! =\n", mpz_get_ui(operand_1m));

                mpz_out_str(stdout,10,result_m); 
                printf(" - Number_of_digits is %lu.\n", number_of_digits_to_display);

                start_new_calc_flag = nk_true;
            }
            if (nk_button_label(ctx, "/"))
            {
                mpz_init_set_str (operand_1m, text, 10);
                strcpy(text, "");
                strcat(text_to_display_on_screen, " / ");
                Operation = DIVIDE;
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


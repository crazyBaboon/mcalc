/*
 * mcalc - multiple precision calculator
 *
 * Copyright (C)2022 Nuno Ferreira
 *
 * mcalc is free software; you can redistribute/modify it under 
 * the terms of the GNU General Public License version 2.
 * 
 * Compile & run on Unix-like systems:
 * clang main.c operations.c -std=c99 -Wall -Wextra -pedantic -O2 -o mcalc -lglfw -lGL -lm -lGLU -lgmp && ./mcalc 
 */

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
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
#include "operations.h"

#define WINDOW_WIDTH 290
#define WINDOW_HEIGHT 250
#define MAX_DIGITS_DISPLAY 34



static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}



int main(void)
{
    /* Platform */
    static GLFWwindow *win;
    struct nk_context *ctx;
    int width = 0, height = 0;

    /* Calculator variables */
    bool start_new_calc_flag = true;
    mpz_t operand_1m, operand_2m;
    static size_t number_of_digits_to_display = 0;
    static char text_to_display_on_screen[128] = "";
    char* text;
    int Operation = FACTORIAL;
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

        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        nk_glfw3_font_stash_end();

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
                nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX, box_buffer, 128, nk_filter_default);
            }            
            else
                nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX, box_buffer, number_of_digits_to_display + 2, nk_filter_default);

            nk_layout_row_static(ctx, 30, 40, 6);
            if (nk_button_label(ctx, "7"))
                process_digit_button(&start_new_calc_flag, text_to_display_on_screen, text, "7");
            if (nk_button_label(ctx, "8"))
                process_digit_button(&start_new_calc_flag, text_to_display_on_screen, text, "8");
            if (nk_button_label(ctx, "9"))
                process_digit_button(&start_new_calc_flag, text_to_display_on_screen, text, "9");
            if (nk_button_label(ctx, "C"))
                reset_button(text, text_to_display_on_screen, box_buffer);
               
            nk_layout_row_static(ctx, 30, 40, 6);
            if (nk_button_label(ctx, "4"))
                process_digit_button(&start_new_calc_flag, text_to_display_on_screen, text, "4");
            if (nk_button_label(ctx, "5"))
                process_digit_button(&start_new_calc_flag, text_to_display_on_screen, text, "5");
            if (nk_button_label(ctx, "6"))
                process_digit_button(&start_new_calc_flag, text_to_display_on_screen, text, "6");
            
            nk_layout_row_static(ctx, 30, 40, 6);
            if (nk_button_label(ctx, "1"))
                process_digit_button(&start_new_calc_flag, text_to_display_on_screen, text, "1");
            if (nk_button_label(ctx, "2"))
                process_digit_button(&start_new_calc_flag, text_to_display_on_screen, text, "2");
            if (nk_button_label(ctx, "3"))
                process_digit_button(&start_new_calc_flag, text_to_display_on_screen, text, "3");

            if (nk_button_label(ctx, "!"))
			    factorial_button(&start_new_calc_flag, operand_1m, text, text_to_display_on_screen, &Operation); 
            if (nk_button_label(ctx, "="))
                equals_button(&start_new_calc_flag, operand_1m, operand_2m, text, text_to_display_on_screen, &Operation, box_buffer, result_str, &number_of_digits_to_display);

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

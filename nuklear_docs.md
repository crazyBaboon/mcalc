/*
/// # Nuklear
/// ![](https://cloud.githubusercontent.com/assets/8057201/11761525/ae06f0ca-a0c6-11e5-819d-5610b25f6ef4.gif)
///
/// ## Contents
/// 1. About section
/// 2. Highlights section
/// 3. Features section
/// 4. Usage section
///     1. Flags section
///     2. Constants section
///     3. Dependencies section
/// 5. Example section
/// 6. API section
///     1. Context section
///     2. Input section
///     3. Drawing section
///     4. Window section
///     5. Layouting section
///     6. Groups section
///     7. Tree section
///     8. Properties section
/// 7. License section
/// 8. Changelog section
/// 9. Gallery section
/// 10. Credits section
///
/// ## About
/// This is a minimal state immediate mode graphical user interface toolkit
/// written in ANSI C and licensed under public domain. It was designed as a simple
/// embeddable user interface for application and does not have any dependencies,
/// a default renderbackend or OS window and input handling but instead provides a very modular
/// library approach by using simple input state for input and draw
/// commands describing primitive shapes as output. So instead of providing a
/// layered library that tries to abstract over a number of platform and
/// render backends it only focuses on the actual UI.
///
/// ## Highlights
/// - Graphical user interface toolkit
/// - Single header library
/// - Written in C89 (a.k.a. ANSI C or ISO C90)
/// - Small codebase (~18kLOC)
/// - Focus on portability, efficiency and simplicity
/// - No dependencies (not even the standard library if not wanted)
/// - Fully skinnable and customizable
/// - Low memory footprint with total memory control if needed or wanted
/// - UTF-8 support
/// - No global or hidden state
/// - Customizable library modules (you can compile and use only what you need)
/// - Optional font baker and vertex buffer output
///
/// ## Features
/// - Absolutely no platform dependent code
/// - Memory management control ranging from/to
///     - Ease of use by allocating everything from standard library
///     - Control every byte of memory inside the library
/// - Font handling control ranging from/to
///     - Use your own font implementation for everything
///     - Use this libraries internal font baking and handling API
/// - Drawing output control ranging from/to
///     - Simple shapes for more high level APIs which already have drawing capabilities
///     - Hardware accessible anti-aliased vertex buffer output
/// - Customizable colors and properties ranging from/to
///     - Simple changes to color by filling a simple color table
///     - Complete control with ability to use skinning to decorate widgets
/// - Bendable UI library with widget ranging from/to
///     - Basic widgets like buttons, checkboxes, slider, ...
///     - Advanced widget like abstract comboboxes, contextual menus,...
/// - Compile time configuration to only compile what you need
///     - Subset which can be used if you do not want to link or use the standard library
/// - Can be easily modified to only update on user input instead of frame updates
///
/// ## Usage
/// This library is self contained in one single header file and can be used either
/// in header only mode or in implementation mode. The header only mode is used
/// by default when included and allows including this header in other headers
/// and does not contain the actual implementation. <br /><br />
///
/// The implementation mode requires to define  the preprocessor macro
/// NK_IMPLEMENTATION in *one* .c/.cpp file before #including this file, e.g.:
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~C
///     #define NK_IMPLEMENTATION
///     #include "nuklear.h"
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Also optionally define the symbols listed in the section "OPTIONAL DEFINES"
/// below in header and implementation mode if you want to use additional functionality
/// or need more control over the library.
///
/// !!! WARNING
///     Every time nuklear is included define the same compiler flags. This very important not doing so could lead to compiler errors or even worse stack corruptions.
///
/// ### Flags
/// Flag                            | Description
/// --------------------------------|------------------------------------------
/// NK_PRIVATE                      | If defined declares all functions as static, so they can only be accessed inside the file that contains the implementation
/// NK_INCLUDE_FIXED_TYPES          | If defined it will include header `<stdint.h>` for fixed sized types otherwise nuklear tries to select the correct type. If that fails it will throw a compiler error and you have to select the correct types yourself.
/// NK_INCLUDE_DEFAULT_ALLOCATOR    | If defined it will include header `<stdlib.h>` and provide additional functions to use this library without caring for memory allocation control and therefore ease memory management.
/// NK_INCLUDE_STANDARD_IO          | If defined it will include header `<stdio.h>` and provide additional functions depending on file loading.
/// NK_INCLUDE_STANDARD_VARARGS     | If defined it will include header <stdarg.h> and provide additional functions depending on file loading.
/// NK_INCLUDE_STANDARD_BOOL        | If defined it will include header `<stdbool.h>` for nk_bool otherwise nuklear defines nk_bool as int.
/// NK_INCLUDE_VERTEX_BUFFER_OUTPUT | Defining this adds a vertex draw command list backend to this library, which allows you to convert queue commands into vertex draw commands. This is mainly if you need a hardware accessible format for OpenGL, DirectX, Vulkan, Metal,...
/// NK_INCLUDE_FONT_BAKING          | Defining this adds `stb_truetype` and `stb_rect_pack` implementation to this library and provides font baking and rendering. If you already have font handling or do not want to use this font handler you don't have to define it.
/// NK_INCLUDE_DEFAULT_FONT         | Defining this adds the default font: ProggyClean.ttf into this library which can be loaded into a font atlas and allows using this library without having a truetype font
/// NK_INCLUDE_COMMAND_USERDATA     | Defining this adds a userdata pointer into each command. Can be useful for example if you want to provide custom shaders depending on the used widget. Can be combined with the style structures.
/// NK_BUTTON_TRIGGER_ON_RELEASE    | Different platforms require button clicks occurring either on buttons being pressed (up to down) or released (down to up). By default this library will react on buttons being pressed, but if you define this it will only trigger if a button is released.
/// NK_ZERO_COMMAND_MEMORY          | Defining this will zero out memory for each drawing command added to a drawing queue (inside nk_command_buffer_push). Zeroing command memory is very useful for fast checking (using memcmp) if command buffers are equal and avoid drawing frames when nothing on screen has changed since previous frame.
/// NK_UINT_DRAW_INDEX              | Defining this will set the size of vertex index elements when using NK_VERTEX_BUFFER_OUTPUT to 32bit instead of the default of 16bit
/// NK_KEYSTATE_BASED_INPUT         | Define this if your backend uses key state for each frame rather than key press/release events
///
/// !!! WARNING
///     The following flags will pull in the standard C library:
///     - NK_INCLUDE_DEFAULT_ALLOCATOR
///     - NK_INCLUDE_STANDARD_IO
///     - NK_INCLUDE_STANDARD_VARARGS
///
/// !!! WARNING
///     The following flags if defined need to be defined for both header and implementation:
///     - NK_INCLUDE_FIXED_TYPES
///     - NK_INCLUDE_DEFAULT_ALLOCATOR
///     - NK_INCLUDE_STANDARD_VARARGS
///     - NK_INCLUDE_STANDARD_BOOL
///     - NK_INCLUDE_VERTEX_BUFFER_OUTPUT
///     - NK_INCLUDE_FONT_BAKING
///     - NK_INCLUDE_DEFAULT_FONT
///     - NK_INCLUDE_STANDARD_VARARGS
///     - NK_INCLUDE_COMMAND_USERDATA
///     - NK_UINT_DRAW_INDEX
///
/// ### Constants
/// Define                          | Description
/// --------------------------------|---------------------------------------
/// NK_BUFFER_DEFAULT_INITIAL_SIZE  | Initial buffer size allocated by all buffers while using the default allocator functions included by defining NK_INCLUDE_DEFAULT_ALLOCATOR. If you don't want to allocate the default 4k memory then redefine it.
/// NK_MAX_NUMBER_BUFFER            | Maximum buffer size for the conversion buffer between float and string Under normal circumstances this should be more than sufficient.
/// NK_INPUT_MAX                    | Defines the max number of bytes which can be added as text input in one frame. Under normal circumstances this should be more than sufficient.
///
/// !!! WARNING
///     The following constants if defined need to be defined for both header and implementation:
///     - NK_MAX_NUMBER_BUFFER
///     - NK_BUFFER_DEFAULT_INITIAL_SIZE
///     - NK_INPUT_MAX
///
/// ### Dependencies
/// Function    | Description
/// ------------|---------------------------------------------------------------
/// NK_ASSERT   | If you don't define this, nuklear will use <assert.h> with assert().
/// NK_MEMSET   | You can define this to 'memset' or your own memset implementation replacement. If not nuklear will use its own version.
/// NK_MEMCPY   | You can define this to 'memcpy' or your own memcpy implementation replacement. If not nuklear will use its own version.
/// NK_INV_SQRT | You can define this to your own inverse sqrt implementation replacement. If not nuklear will use its own slow and not highly accurate version.
/// NK_SIN      | You can define this to 'sinf' or your own sine implementation replacement. If not nuklear will use its own approximation implementation.
/// NK_COS      | You can define this to 'cosf' or your own cosine implementation replacement. If not nuklear will use its own approximation implementation.
/// NK_STRTOD   | You can define this to `strtod` or your own string to double conversion implementation replacement. If not defined nuklear will use its own imprecise and possibly unsafe version (does not handle nan or infinity!).
/// NK_DTOA     | You can define this to `dtoa` or your own double to string conversion implementation replacement. If not defined nuklear will use its own imprecise and possibly unsafe version (does not handle nan or infinity!).
/// NK_VSNPRINTF| If you define `NK_INCLUDE_STANDARD_VARARGS` as well as `NK_INCLUDE_STANDARD_IO` and want to be safe define this to `vsnprintf` on compilers supporting later versions of C or C++. By default nuklear will check for your stdlib version in C as well as compiler version in C++. if `vsnprintf` is available it will define it to `vsnprintf` directly. If not defined and if you have older versions of C or C++ it will be defined to `vsprintf` which is unsafe.
///
/// !!! WARNING
///     The following dependencies will pull in the standard C library if not redefined:
///     - NK_ASSERT
///
/// !!! WARNING
///     The following dependencies if defined need to be defined for both header and implementation:
///     - NK_ASSERT
///
/// !!! WARNING
///     The following dependencies if defined need to be defined only for the implementation part:
///     - NK_MEMSET
///     - NK_MEMCPY
///     - NK_SQRT
///     - NK_SIN
///     - NK_COS
///     - NK_STRTOD
///     - NK_DTOA
///     - NK_VSNPRINTF
///
/// ## Example
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// // init gui state
/// enum {EASY, HARD};
/// static int op = EASY;
/// static float value = 0.6f;
/// static int i =  20;
/// struct nk_context ctx;
///
/// nk_init_fixed(&ctx, calloc(1, MAX_MEMORY), MAX_MEMORY, &font);
/// if (nk_begin(&ctx, "Show", nk_rect(50, 50, 220, 220),
///     NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
///     // fixed widget pixel width
///     nk_layout_row_static(&ctx, 30, 80, 1);
///     if (nk_button_label(&ctx, "button")) {
///         // event handling
///     }
///
///     // fixed widget window ratio width
///     nk_layout_row_dynamic(&ctx, 30, 2);
///     if (nk_option_label(&ctx, "easy", op == EASY)) op = EASY;
///     if (nk_option_label(&ctx, "hard", op == HARD)) op = HARD;
///
///     // custom widget pixel width
///     nk_layout_row_begin(&ctx, NK_STATIC, 30, 2);
///     {
///         nk_layout_row_push(&ctx, 50);
///         nk_label(&ctx, "Volume:", NK_TEXT_LEFT);
///         nk_layout_row_push(&ctx, 110);
///         nk_slider_float(&ctx, 0, &value, 1.0f, 0.1f);
///     }
///     nk_layout_row_end(&ctx);
/// }
/// nk_end(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// ![](https://cloud.githubusercontent.com/assets/8057201/10187981/584ecd68-675c-11e5-897c-822ef534a876.png)
///
/// ## API
///
*/
/* =============================================================================
 *
 *                                  CONTEXT
 *
 * =============================================================================*/

/*/// ### Context
/// Contexts are the main entry point and the majestro of nuklear and contain all required state.
/// They are used for window, memory, input, style, stack, commands and time management and need
/// to be passed into all nuklear GUI specific functions.
///
/// #### Usage
/// To use a context it first has to be initialized which can be achieved by calling
/// one of either `nk_init_default`, `nk_init_fixed`, `nk_init`, `nk_init_custom`.
/// Each takes in a font handle and a specific way of handling memory. Memory control
/// hereby ranges from standard library to just specifying a fixed sized block of memory
/// which nuklear has to manage itself from.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     // [...]
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function            | Description
/// --------------------|-------------------------------------------------------
/// __nk_init_default__ | Initializes context with standard library memory allocation (malloc,free)
/// __nk_init_fixed__   | Initializes context from single fixed size memory block
/// __nk_init__         | Initializes context with memory allocator callbacks for alloc and free
/// __nk_init_custom__  | Initializes context from two buffers. One for draw commands the other for window/panel/table allocations
/// __nk_clear__        | Called at the end of the frame to reset and prepare the context for the next frame
/// __nk_free__         | Shutdown and free all memory allocated inside the context
/// __nk_set_user_data__| Utility function to pass user data to draw command
 */
#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
/*/// #### nk_init_default
/// Initializes a `nk_context` struct with a default standard library allocator.
/// Should be used if you don't want to be bothered with memory management in nuklear.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_init_default(struct nk_context *ctx, const struct nk_user_font *font);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|---------------------------------------------------------------
/// __ctx__     | Must point to an either stack or heap allocated `nk_context` struct
/// __font__    | Must point to a previously initialized font handle for more info look at font documentation
///
/// Returns either `false(0)` on failure or `true(1)` on success.
///
*/
NK_API nk_bool nk_init_default(struct nk_context*, const struct nk_user_font*);
#endif
/*/// #### nk_init_fixed
/// Initializes a `nk_context` struct from single fixed size memory block
/// Should be used if you want complete control over nuklear's memory management.
/// Especially recommended for system with little memory or systems with virtual memory.
/// For the later case you can just allocate for example 16MB of virtual memory
/// and only the required amount of memory will actually be committed.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_init_fixed(struct nk_context *ctx, void *memory, nk_size size, const struct nk_user_font *font);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// !!! Warning
///     make sure the passed memory block is aligned correctly for `nk_draw_commands`.
///
/// Parameter   | Description
/// ------------|--------------------------------------------------------------
/// __ctx__     | Must point to an either stack or heap allocated `nk_context` struct
/// __memory__  | Must point to a previously allocated memory block
/// __size__    | Must contain the total size of __memory__
/// __font__    | Must point to a previously initialized font handle for more info look at font documentation
///
/// Returns either `false(0)` on failure or `true(1)` on success.
*/
NK_API nk_bool nk_init_fixed(struct nk_context*, void *memory, nk_size size, const struct nk_user_font*);
/*/// #### nk_init
/// Initializes a `nk_context` struct with memory allocation callbacks for nuklear to allocate
/// memory from. Used internally for `nk_init_default` and provides a kitchen sink allocation
/// interface to nuklear. Can be useful for cases like monitoring memory consumption.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_init(struct nk_context *ctx, struct nk_allocator *alloc, const struct nk_user_font *font);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|---------------------------------------------------------------
/// __ctx__     | Must point to an either stack or heap allocated `nk_context` struct
/// __alloc__   | Must point to a previously allocated memory allocator
/// __font__    | Must point to a previously initialized font handle for more info look at font documentation
///
/// Returns either `false(0)` on failure or `true(1)` on success.
*/
NK_API nk_bool nk_init(struct nk_context*, struct nk_allocator*, const struct nk_user_font*);
/*/// #### nk_init_custom
/// Initializes a `nk_context` struct from two different either fixed or growing
/// buffers. The first buffer is for allocating draw commands while the second buffer is
/// used for allocating windows, panels and state tables.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_init_custom(struct nk_context *ctx, struct nk_buffer *cmds, struct nk_buffer *pool, const struct nk_user_font *font);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|---------------------------------------------------------------
/// __ctx__     | Must point to an either stack or heap allocated `nk_context` struct
/// __cmds__    | Must point to a previously initialized memory buffer either fixed or dynamic to store draw commands into
/// __pool__    | Must point to a previously initialized memory buffer either fixed or dynamic to store windows, panels and tables
/// __font__    | Must point to a previously initialized font handle for more info look at font documentation
///
/// Returns either `false(0)` on failure or `true(1)` on success.
*/
NK_API nk_bool nk_init_custom(struct nk_context*, struct nk_buffer *cmds, struct nk_buffer *pool, const struct nk_user_font*);
/*/// #### nk_clear
/// Resets the context state at the end of the frame. This includes mostly
/// garbage collector tasks like removing windows or table not called and therefore
/// used anymore.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_clear(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
*/
NK_API void nk_clear(struct nk_context*);
/*/// #### nk_free
/// Frees all memory allocated by nuklear. Not needed if context was
/// initialized with `nk_init_fixed`.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_free(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
*/
NK_API void nk_free(struct nk_context*);
#ifdef NK_INCLUDE_COMMAND_USERDATA
/*/// #### nk_set_user_data
/// Sets the currently passed userdata passed down into each draw command.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_set_user_data(struct nk_context *ctx, nk_handle data);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|--------------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __data__    | Handle with either pointer or index to be passed into every draw commands
*/
NK_API void nk_set_user_data(struct nk_context*, nk_handle handle);
#endif
/* =============================================================================
 *
 *                                  INPUT
 *
 * =============================================================================*/
/*/// ### Input
/// The input API is responsible for holding the current input state composed of
/// mouse, key and text input states.
/// It is worth noting that no direct OS or window handling is done in nuklear.
/// Instead all input state has to be provided by platform specific code. This on one hand
/// expects more work from the user and complicates usage but on the other hand
/// provides simple abstraction over a big number of platforms, libraries and other
/// already provided functionality.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_input_begin(&ctx);
/// while (GetEvent(&evt)) {
///     if (evt.type == MOUSE_MOVE)
///         nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///     else if (evt.type == [...]) {
///         // [...]
///     }
/// } nk_input_end(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Usage
/// Input state needs to be provided to nuklear by first calling `nk_input_begin`
/// which resets internal state like delta mouse position and button transitions.
/// After `nk_input_begin` all current input state needs to be provided. This includes
/// mouse motion, button and key pressed and released, text input and scrolling.
/// Both event- or state-based input handling are supported by this API
/// and should work without problems. Finally after all input state has been
/// mirrored `nk_input_end` needs to be called to finish input process.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     Event evt;
///     nk_input_begin(&ctx);
///     while (GetEvent(&evt)) {
///         if (evt.type == MOUSE_MOVE)
///             nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///         else if (evt.type == [...]) {
///             // [...]
///         }
///     }
///     nk_input_end(&ctx);
///     // [...]
///     nk_clear(&ctx);
/// } nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function            | Description
/// --------------------|-------------------------------------------------------
/// __nk_input_begin__  | Begins the input mirroring process. Needs to be called before all other `nk_input_xxx` calls
/// __nk_input_motion__ | Mirrors mouse cursor position
/// __nk_input_key__    | Mirrors key state with either pressed or released
/// __nk_input_button__ | Mirrors mouse button state with either pressed or released
/// __nk_input_scroll__ | Mirrors mouse scroll values
/// __nk_input_char__   | Adds a single ASCII text character into an internal text buffer
/// __nk_input_glyph__  | Adds a single multi-byte UTF-8 character into an internal text buffer
/// __nk_input_unicode__| Adds a single unicode rune into an internal text buffer
/// __nk_input_end__    | Ends the input mirroring process by calculating state changes. Don't call any `nk_input_xxx` function referenced above after this call
*/
enum nk_keys {
    NK_KEY_NONE,
    NK_KEY_SHIFT,
    NK_KEY_CTRL,
    NK_KEY_DEL,
    NK_KEY_ENTER,
    NK_KEY_TAB,
    NK_KEY_BACKSPACE,
    NK_KEY_COPY,
    NK_KEY_CUT,
    NK_KEY_PASTE,
    NK_KEY_UP,
    NK_KEY_DOWN,
    NK_KEY_LEFT,
    NK_KEY_RIGHT,
    /* Shortcuts: text field */
    NK_KEY_TEXT_INSERT_MODE,
    NK_KEY_TEXT_REPLACE_MODE,
    NK_KEY_TEXT_RESET_MODE,
    NK_KEY_TEXT_LINE_START,
    NK_KEY_TEXT_LINE_END,
    NK_KEY_TEXT_START,
    NK_KEY_TEXT_END,
    NK_KEY_TEXT_UNDO,
    NK_KEY_TEXT_REDO,
    NK_KEY_TEXT_SELECT_ALL,
    NK_KEY_TEXT_WORD_LEFT,
    NK_KEY_TEXT_WORD_RIGHT,
    /* Shortcuts: scrollbar */
    NK_KEY_SCROLL_START,
    NK_KEY_SCROLL_END,
    NK_KEY_SCROLL_DOWN,
    NK_KEY_SCROLL_UP,
    NK_KEY_MAX
};
enum nk_buttons {
    NK_BUTTON_LEFT,
    NK_BUTTON_MIDDLE,
    NK_BUTTON_RIGHT,
    NK_BUTTON_DOUBLE,
    NK_BUTTON_MAX
};
/*/// #### nk_input_begin
/// Begins the input mirroring process by resetting text, scroll
/// mouse, previous mouse position and movement as well as key state transitions,
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_begin(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
*/
NK_API void nk_input_begin(struct nk_context*);
/*/// #### nk_input_motion
/// Mirrors current mouse position to nuklear
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_motion(struct nk_context *ctx, int x, int y);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __x__       | Must hold an integer describing the current mouse cursor x-position
/// __y__       | Must hold an integer describing the current mouse cursor y-position
*/
NK_API void nk_input_motion(struct nk_context*, int x, int y);
/*/// #### nk_input_key
/// Mirrors the state of a specific key to nuklear
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_key(struct nk_context*, enum nk_keys key, nk_bool down);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __key__     | Must be any value specified in enum `nk_keys` that needs to be mirrored
/// __down__    | Must be 0 for key is up and 1 for key is down
*/
NK_API void nk_input_key(struct nk_context*, enum nk_keys, nk_bool down);
/*/// #### nk_input_button
/// Mirrors the state of a specific mouse button to nuklear
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_button(struct nk_context *ctx, enum nk_buttons btn, int x, int y, nk_bool down);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __btn__     | Must be any value specified in enum `nk_buttons` that needs to be mirrored
/// __x__       | Must contain an integer describing mouse cursor x-position on click up/down
/// __y__       | Must contain an integer describing mouse cursor y-position on click up/down
/// __down__    | Must be 0 for key is up and 1 for key is down
*/
NK_API void nk_input_button(struct nk_context*, enum nk_buttons, int x, int y, nk_bool down);
/*/// #### nk_input_scroll
/// Copies the last mouse scroll value to nuklear. Is generally
/// a scroll value. So does not have to come from mouse and could also originate
/// TODO finish this sentence
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_scroll(struct nk_context *ctx, struct nk_vec2 val);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __val__     | vector with both X- as well as Y-scroll value
*/
NK_API void nk_input_scroll(struct nk_context*, struct nk_vec2 val);
/*/// #### nk_input_char
/// Copies a single ASCII character into an internal text buffer
/// This is basically a helper function to quickly push ASCII characters into
/// nuklear.
///
/// !!! Note
///     Stores up to NK_INPUT_MAX bytes between `nk_input_begin` and `nk_input_end`.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_char(struct nk_context *ctx, char c);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __c__       | Must be a single ASCII character preferable one that can be printed
*/
NK_API void nk_input_char(struct nk_context*, char);
/*/// #### nk_input_glyph
/// Converts an encoded unicode rune into UTF-8 and copies the result into an
/// internal text buffer.
///
/// !!! Note
///     Stores up to NK_INPUT_MAX bytes between `nk_input_begin` and `nk_input_end`.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_glyph(struct nk_context *ctx, const nk_glyph g);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __g__       | UTF-32 unicode codepoint
*/
NK_API void nk_input_glyph(struct nk_context*, const nk_glyph);
/*/// #### nk_input_unicode
/// Converts a unicode rune into UTF-8 and copies the result
/// into an internal text buffer.
/// !!! Note
///     Stores up to NK_INPUT_MAX bytes between `nk_input_begin` and `nk_input_end`.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_unicode(struct nk_context*, nk_rune rune);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __rune__    | UTF-32 unicode codepoint
*/
NK_API void nk_input_unicode(struct nk_context*, nk_rune);
/*/// #### nk_input_end
/// End the input mirroring process by resetting mouse grabbing
/// state to ensure the mouse cursor is not grabbed indefinitely.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_end(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
*/
NK_API void nk_input_end(struct nk_context*);

/* =============================================================================
 *
 *                                  DRAWING
 *
 * =============================================================================*/
/*/// ### Drawing
/// This library was designed to be render backend agnostic so it does
/// not draw anything to screen directly. Instead all drawn shapes, widgets
/// are made of, are buffered into memory and make up a command queue.
/// Each frame therefore fills the command buffer with draw commands
/// that then need to be executed by the user and his own render backend.
/// After that the command buffer needs to be cleared and a new frame can be
/// started. It is probably important to note that the command buffer is the main
/// drawing API and the optional vertex buffer API only takes this format and
/// converts it into a hardware accessible format.
///
/// #### Usage
/// To draw all draw commands accumulated over a frame you need your own render
/// backend able to draw a number of 2D primitives. This includes at least
/// filled and stroked rectangles, circles, text, lines, triangles and scissors.
/// As soon as this criterion is met you can iterate over each draw command
/// and execute each draw command in a interpreter like fashion:
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_command *cmd = 0;
/// nk_foreach(cmd, &ctx) {
///     switch (cmd->type) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case //...:
///         //[...]
///     }
/// }
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// In program flow context draw commands need to be executed after input has been
/// gathered and the complete UI with windows and their contained widgets have
/// been executed and before calling `nk_clear` which frees all previously
/// allocated draw commands.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     Event evt;
///     nk_input_begin(&ctx);
///     while (GetEvent(&evt)) {
///         if (evt.type == MOUSE_MOVE)
///             nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///         else if (evt.type == [...]) {
///             [...]
///         }
///     }
///     nk_input_end(&ctx);
///     //
///     // [...]
///     //
///     const struct nk_command *cmd = 0;
///     nk_foreach(cmd, &ctx) {
///     switch (cmd->type) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case ...:
///         // [...]
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// You probably noticed that you have to draw all of the UI each frame which is
/// quite wasteful. While the actual UI updating loop is quite fast rendering
/// without actually needing it is not. So there are multiple things you could do.
///
/// First is only update on input. This of course is only an option if your
/// application only depends on the UI and does not require any outside calculations.
/// If you actually only update on input make sure to update the UI two times each
/// frame and call `nk_clear` directly after the first pass and only draw in
/// the second pass. In addition it is recommended to also add additional timers
/// to make sure the UI is not drawn more than a fixed number of frames per second.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     // [...wait for input ]
///     // [...do two UI passes ...]
///     do_ui(...)
///     nk_clear(&ctx);
///     do_ui(...)
///     //
///     // draw
///     const struct nk_command *cmd = 0;
///     nk_foreach(cmd, &ctx) {
///     switch (cmd->type) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case ...:
///         //[...]
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// The second probably more applicable trick is to only draw if anything changed.
/// It is not really useful for applications with continuous draw loop but
/// quite useful for desktop applications. To actually get nuklear to only
/// draw on changes you first have to define `NK_ZERO_COMMAND_MEMORY` and
/// allocate a memory buffer that will store each unique drawing output.
/// After each frame you compare the draw command memory inside the library
/// with your allocated buffer by memcmp. If memcmp detects differences
/// you have to copy the command buffer into the allocated buffer
/// and then draw like usual (this example uses fixed memory but you could
/// use dynamically allocated memory).
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// //[... other defines ...]
/// #define NK_ZERO_COMMAND_MEMORY
/// #include "nuklear.h"
/// //
/// // setup context
/// struct nk_context ctx;
/// void *last = calloc(1,64*1024);
/// void *buf = calloc(1,64*1024);
/// nk_init_fixed(&ctx, buf, 64*1024);
/// //
/// // loop
/// while (1) {
///     // [...input...]
///     // [...ui...]
///     void *cmds = nk_buffer_memory(&ctx.memory);
///     if (memcmp(cmds, last, ctx.memory.allocated)) {
///         memcpy(last,cmds,ctx.memory.allocated);
///         const struct nk_command *cmd = 0;
///         nk_foreach(cmd, &ctx) {
///             switch (cmd->type) {
///             case NK_COMMAND_LINE:
///                 your_draw_line_function(...)
///                 break;
///             case NK_COMMAND_RECT
///                 your_draw_rect_function(...)
///                 break;
///             case ...:
///                 // [...]
///             }
///         }
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Finally while using draw commands makes sense for higher abstracted platforms like
/// X11 and Win32 or drawing libraries it is often desirable to use graphics
/// hardware directly. Therefore it is possible to just define
/// `NK_INCLUDE_VERTEX_BUFFER_OUTPUT` which includes optional vertex output.
/// To access the vertex output you first have to convert all draw commands into
/// vertexes by calling `nk_convert` which takes in your preferred vertex format.
/// After successfully converting all draw commands just iterate over and execute all
/// vertex draw commands:
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// // fill configuration
/// struct your_vertex
/// {
///     float pos[2]; // important to keep it to 2 floats
///     float uv[2];
///     unsigned char col[4];
/// };
/// struct nk_convert_config cfg = {};
/// static const struct nk_draw_vertex_layout_element vertex_layout[] = {
///     {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct your_vertex, pos)},
///     {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct your_vertex, uv)},
///     {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct your_vertex, col)},
///     {NK_VERTEX_LAYOUT_END}
/// };
/// cfg.shape_AA = NK_ANTI_ALIASING_ON;
/// cfg.line_AA = NK_ANTI_ALIASING_ON;
/// cfg.vertex_layout = vertex_layout;
/// cfg.vertex_size = sizeof(struct your_vertex);
/// cfg.vertex_alignment = NK_ALIGNOF(struct your_vertex);
/// cfg.circle_segment_count = 22;
/// cfg.curve_segment_count = 22;
/// cfg.arc_segment_count = 22;
/// cfg.global_alpha = 1.0f;
/// cfg.null = dev->null;
/// //
/// // setup buffers and convert
/// struct nk_buffer cmds, verts, idx;
/// nk_buffer_init_default(&cmds);
/// nk_buffer_init_default(&verts);
/// nk_buffer_init_default(&idx);
/// nk_convert(&ctx, &cmds, &verts, &idx, &cfg);
/// //
/// // draw
/// nk_draw_foreach(cmd, &ctx, &cmds) {
/// if (!cmd->elem_count) continue;
///     //[...]
/// }
/// nk_buffer_free(&cms);
/// nk_buffer_free(&verts);
/// nk_buffer_free(&idx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function            | Description
/// --------------------|-------------------------------------------------------
/// __nk__begin__       | Returns the first draw command in the context draw command list to be drawn
/// __nk__next__        | Increments the draw command iterator to the next command inside the context draw command list
/// __nk_foreach__      | Iterates over each draw command inside the context draw command list
/// __nk_convert__      | Converts from the abstract draw commands list into a hardware accessible vertex format
/// __nk_draw_begin__   | Returns the first vertex command in the context vertex draw list to be executed
/// __nk__draw_next__   | Increments the vertex command iterator to the next command inside the context vertex command list
/// __nk__draw_end__    | Returns the end of the vertex draw list
/// __nk_draw_foreach__ | Iterates over each vertex draw command inside the vertex draw list
*/

enum nk_anti_aliasing {NK_ANTI_ALIASING_OFF, NK_ANTI_ALIASING_ON};
enum nk_convert_result {
    NK_CONVERT_SUCCESS = 0,
    NK_CONVERT_INVALID_PARAM = 1,
    NK_CONVERT_COMMAND_BUFFER_FULL = NK_FLAG(1),
    NK_CONVERT_VERTEX_BUFFER_FULL = NK_FLAG(2),
    NK_CONVERT_ELEMENT_BUFFER_FULL = NK_FLAG(3)
};
struct nk_draw_null_texture {
    nk_handle texture; /* texture handle to a texture with a white pixel */
    struct nk_vec2 uv; /* coordinates to a white pixel in the texture  */
};
struct nk_convert_config {
    float global_alpha; /* global alpha value */
    enum nk_anti_aliasing line_AA; /* line anti-aliasing flag can be turned off if you are tight on memory */
    enum nk_anti_aliasing shape_AA; /* shape anti-aliasing flag can be turned off if you are tight on memory */
    unsigned circle_segment_count; /* number of segments used for circles: default to 22 */
    unsigned arc_segment_count; /* number of segments used for arcs: default to 22 */
    unsigned curve_segment_count; /* number of segments used for curves: default to 22 */
    struct nk_draw_null_texture null; /* handle to texture with a white pixel for shape drawing */
    const struct nk_draw_vertex_layout_element *vertex_layout; /* describes the vertex output format and packing */
    nk_size vertex_size; /* sizeof one vertex for vertex packing */
    nk_size vertex_alignment; /* vertex alignment: Can be obtained by NK_ALIGNOF */
};
/*/// #### nk__begin
/// Returns a draw command list iterator to iterate all draw
/// commands accumulated over one frame.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_command* nk__begin(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | must point to an previously initialized `nk_context` struct at the end of a frame
///
/// Returns draw command pointer pointing to the first command inside the draw command list
*/
NK_API const struct nk_command* nk__begin(struct nk_context*);
/*/// #### nk__next
/// Returns draw command pointer pointing to the next command inside the draw command list
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_command* nk__next(struct nk_context*, const struct nk_command*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
/// __cmd__     | Must point to an previously a draw command either returned by `nk__begin` or `nk__next`
///
/// Returns draw command pointer pointing to the next command inside the draw command list
*/
NK_API const struct nk_command* nk__next(struct nk_context*, const struct nk_command*);
/*/// #### nk_foreach
/// Iterates over each draw command inside the context draw command list
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_foreach(c, ctx)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
/// __cmd__     | Command pointer initialized to NULL
///
/// Iterates over each draw command inside the context draw command list
*/
#define nk_foreach(c, ctx) for((c) = nk__begin(ctx); (c) != 0; (c) = nk__next(ctx,c))
#ifdef NK_INCLUDE_VERTEX_BUFFER_OUTPUT
/*/// #### nk_convert
/// Converts all internal draw commands into vertex draw commands and fills
/// three buffers with vertexes, vertex draw commands and vertex indices. The vertex format
/// as well as some other configuration values have to be configured by filling out a
/// `nk_convert_config` struct.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_flags nk_convert(struct nk_context *ctx, struct nk_buffer *cmds,
///     struct nk_buffer *vertices, struct nk_buffer *elements, const struct nk_convert_config*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
/// __cmds__    | Must point to a previously initialized buffer to hold converted vertex draw commands
/// __vertices__| Must point to a previously initialized buffer to hold all produced vertices
/// __elements__| Must point to a previously initialized buffer to hold all produced vertex indices
/// __config__  | Must point to a filled out `nk_config` struct to configure the conversion process
///
/// Returns one of enum nk_convert_result error codes
///
/// Parameter                       | Description
/// --------------------------------|-----------------------------------------------------------
/// NK_CONVERT_SUCCESS              | Signals a successful draw command to vertex buffer conversion
/// NK_CONVERT_INVALID_PARAM        | An invalid argument was passed in the function call
/// NK_CONVERT_COMMAND_BUFFER_FULL  | The provided buffer for storing draw commands is full or failed to allocate more memory
/// NK_CONVERT_VERTEX_BUFFER_FULL   | The provided buffer for storing vertices is full or failed to allocate more memory
/// NK_CONVERT_ELEMENT_BUFFER_FULL  | The provided buffer for storing indices is full or failed to allocate more memory
*/
NK_API nk_flags nk_convert(struct nk_context*, struct nk_buffer *cmds, struct nk_buffer *vertices, struct nk_buffer *elements, const struct nk_convert_config*);
/*/// #### nk__draw_begin
/// Returns a draw vertex command buffer iterator to iterate over the vertex draw command buffer
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_draw_command* nk__draw_begin(const struct nk_context*, const struct nk_buffer*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
/// __buf__     | Must point to an previously by `nk_convert` filled out vertex draw command buffer
///
/// Returns vertex draw command pointer pointing to the first command inside the vertex draw command buffer
*/
NK_API const struct nk_draw_command* nk__draw_begin(const struct nk_context*, const struct nk_buffer*);
/*/// #### nk__draw_end
/// Returns the vertex draw command at the end of the vertex draw command buffer
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_draw_command* nk__draw_end(const struct nk_context *ctx, const struct nk_buffer *buf);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
/// __buf__     | Must point to an previously by `nk_convert` filled out vertex draw command buffer
///
/// Returns vertex draw command pointer pointing to the end of the last vertex draw command inside the vertex draw command buffer
*/
NK_API const struct nk_draw_command* nk__draw_end(const struct nk_context*, const struct nk_buffer*);
/*/// #### nk__draw_next
/// Increments the vertex draw command buffer iterator
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_draw_command* nk__draw_next(const struct nk_draw_command*, const struct nk_buffer*, const struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __cmd__     | Must point to an previously either by `nk__draw_begin` or `nk__draw_next` returned vertex draw command
/// __buf__     | Must point to an previously by `nk_convert` filled out vertex draw command buffer
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
///
/// Returns vertex draw command pointer pointing to the end of the last vertex draw command inside the vertex draw command buffer
*/
NK_API const struct nk_draw_command* nk__draw_next(const struct nk_draw_command*, const struct nk_buffer*, const struct nk_context*);
/*/// #### nk_draw_foreach
/// Iterates over each vertex draw command inside a vertex draw command buffer
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_draw_foreach(cmd,ctx, b)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __cmd__     | `nk_draw_command`iterator set to NULL
/// __buf__     | Must point to an previously by `nk_convert` filled out vertex draw command buffer
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
*/
#define nk_draw_foreach(cmd,ctx, b) for((cmd)=nk__draw_begin(ctx, b); (cmd)!=0; (cmd)=nk__draw_next(cmd, b, ctx))
#endif

/* =============================================================================
 *
 *                                  WINDOW
 *
 * =============================================================================
/// ### Window
/// Windows are the main persistent state used inside nuklear and are life time
/// controlled by simply "retouching" (i.e. calling) each window each frame.
/// All widgets inside nuklear can only be added inside the function pair `nk_begin_xxx`
/// and `nk_end`. Calling any widgets outside these two functions will result in an
/// assert in debug or no state change in release mode.<br /><br />
///
/// Each window holds frame persistent state like position, size, flags, state tables,
/// and some garbage collected internal persistent widget state. Each window
/// is linked into a window stack list which determines the drawing and overlapping
/// order. The topmost window thereby is the currently active window.<br /><br />
///
/// To change window position inside the stack occurs either automatically by
/// user input by being clicked on or programmatically by calling `nk_window_focus`.
/// Windows by default are visible unless explicitly being defined with flag
/// `NK_WINDOW_HIDDEN`, the user clicked the close button on windows with flag
/// `NK_WINDOW_CLOSABLE` or if a window was explicitly hidden by calling
/// `nk_window_show`. To explicitly close and destroy a window call `nk_window_close`.<br /><br />
///
/// #### Usage
/// To create and keep a window you have to call one of the two `nk_begin_xxx`
/// functions to start window declarations and `nk_end` at the end. Furthermore it
/// is recommended to check the return value of `nk_begin_xxx` and only process
/// widgets inside the window if the value is not 0. Either way you have to call
/// `nk_end` at the end of window declarations. Furthermore, do not attempt to
/// nest `nk_begin_xxx` calls which will hopefully result in an assert or if not
/// in a segmentation fault.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// if (nk_begin_xxx(...) {
///     // [... widgets ...]
/// }
/// nk_end(ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// In the grand concept window and widget declarations need to occur after input
/// handling and before drawing to screen. Not doing so can result in higher
/// latency or at worst invalid behavior. Furthermore make sure that `nk_clear`
/// is called at the end of the frame. While nuklear's default platform backends
/// already call `nk_clear` for you if you write your own backend not calling
/// `nk_clear` can cause asserts or even worse undefined behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     Event evt;
///     nk_input_begin(&ctx);
///     while (GetEvent(&evt)) {
///         if (evt.type == MOUSE_MOVE)
///             nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///         else if (evt.type == [...]) {
///             nk_input_xxx(...);
///         }
///     }
///     nk_input_end(&ctx);
///
///     if (nk_begin_xxx(...) {
///         //[...]
///     }
///     nk_end(ctx);
///
///     const struct nk_command *cmd = 0;
///     nk_foreach(cmd, &ctx) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case //...:
///         //[...]
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function                            | Description
/// ------------------------------------|----------------------------------------
/// nk_begin                            | Starts a new window; needs to be called every frame for every window (unless hidden) or otherwise the window gets removed
/// nk_begin_titled                     | Extended window start with separated title and identifier to allow multiple windows with same name but not title
/// nk_end                              | Needs to be called at the end of the window building process to process scaling, scrollbars and general cleanup
//
/// nk_window_find                      | Finds and returns the window with give name
/// nk_window_get_bounds                | Returns a rectangle with screen position and size of the currently processed window.
/// nk_window_get_position              | Returns the position of the currently processed window
/// nk_window_get_size                  | Returns the size with width and height of the currently processed window
/// nk_window_get_width                 | Returns the width of the currently processed window
/// nk_window_get_height                | Returns the height of the currently processed window
/// nk_window_get_panel                 | Returns the underlying panel which contains all processing state of the current window
/// nk_window_get_content_region        | Returns the position and size of the currently visible and non-clipped space inside the currently processed window
/// nk_window_get_content_region_min    | Returns the upper rectangle position of the currently visible and non-clipped space inside the currently processed window
/// nk_window_get_content_region_max    | Returns the upper rectangle position of the currently visible and non-clipped space inside the currently processed window
/// nk_window_get_content_region_size   | Returns the size of the currently visible and non-clipped space inside the currently processed window
/// nk_window_get_canvas                | Returns the draw command buffer. Can be used to draw custom widgets
/// nk_window_get_scroll                | Gets the scroll offset of the current window
/// nk_window_has_focus                 | Returns if the currently processed window is currently active
/// nk_window_is_collapsed              | Returns if the window with given name is currently minimized/collapsed
/// nk_window_is_closed                 | Returns if the currently processed window was closed
/// nk_window_is_hidden                 | Returns if the currently processed window was hidden
/// nk_window_is_active                 | Same as nk_window_has_focus for some reason
/// nk_window_is_hovered                | Returns if the currently processed window is currently being hovered by mouse
/// nk_window_is_any_hovered            | Return if any window currently hovered
/// nk_item_is_any_active               | Returns if any window or widgets is currently hovered or active
//
/// nk_window_set_bounds                | Updates position and size of the currently processed window
/// nk_window_set_position              | Updates position of the currently process window
/// nk_window_set_size                  | Updates the size of the currently processed window
/// nk_window_set_focus                 | Set the currently processed window as active window
/// nk_window_set_scroll                | Sets the scroll offset of the current window
//
/// nk_window_close                     | Closes the window with given window name which deletes the window at the end of the frame
/// nk_window_collapse                  | Collapses the window with given window name
/// nk_window_collapse_if               | Collapses the window with given window name if the given condition was met
/// nk_window_show                      | Hides a visible or reshows a hidden window
/// nk_window_show_if                   | Hides/shows a window depending on condition
*/
/*
/// #### nk_panel_flags
/// Flag                        | Description
/// ----------------------------|----------------------------------------
/// NK_WINDOW_BORDER            | Draws a border around the window to visually separate window from the background
/// NK_WINDOW_MOVABLE           | The movable flag indicates that a window can be moved by user input or by dragging the window header
/// NK_WINDOW_SCALABLE          | The scalable flag indicates that a window can be scaled by user input by dragging a scaler icon at the button of the window
/// NK_WINDOW_CLOSABLE          | Adds a closable icon into the header
/// NK_WINDOW_MINIMIZABLE       | Adds a minimize icon into the header
/// NK_WINDOW_NO_SCROLLBAR      | Removes the scrollbar from the window
/// NK_WINDOW_TITLE             | Forces a header at the top at the window showing the title
/// NK_WINDOW_SCROLL_AUTO_HIDE  | Automatically hides the window scrollbar if no user interaction: also requires delta time in `nk_context` to be set each frame
/// NK_WINDOW_BACKGROUND        | Always keep window in the background
/// NK_WINDOW_SCALE_LEFT        | Puts window scaler in the left-bottom corner instead right-bottom
/// NK_WINDOW_NO_INPUT          | Prevents window of scaling, moving or getting focus
///
/// #### nk_collapse_states
/// State           | Description
/// ----------------|-----------------------------------------------------------
/// __NK_MINIMIZED__| UI section is collased and not visible until maximized
/// __NK_MAXIMIZED__| UI section is extended and visible until minimized
/// <br /><br />
*/

enum nk_panel_flags {
    NK_WINDOW_BORDER            = NK_FLAG(0),
    NK_WINDOW_MOVABLE           = NK_FLAG(1),
    NK_WINDOW_SCALABLE          = NK_FLAG(2),
    NK_WINDOW_CLOSABLE          = NK_FLAG(3),
    NK_WINDOW_MINIMIZABLE       = NK_FLAG(4),
    NK_WINDOW_NO_SCROLLBAR      = NK_FLAG(5),
    NK_WINDOW_TITLE             = NK_FLAG(6),
    NK_WINDOW_SCROLL_AUTO_HIDE  = NK_FLAG(7),
    NK_WINDOW_BACKGROUND        = NK_FLAG(8),
    NK_WINDOW_SCALE_LEFT        = NK_FLAG(9),
    NK_WINDOW_NO_INPUT          = NK_FLAG(10)
};
/*/// #### nk_begin
/// Starts a new window; needs to be called every frame for every
/// window (unless hidden) or otherwise the window gets removed
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_begin(struct nk_context *ctx, const char *title, struct nk_rect bounds, nk_flags flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __title__   | Window title and identifier. Needs to be persistent over frames to identify the window
/// __bounds__  | Initial position and window size. However if you do not define `NK_WINDOW_SCALABLE` or `NK_WINDOW_MOVABLE` you can set window position and size every frame
/// __flags__   | Window flags defined in the nk_panel_flags section with a number of different window behaviors
///
/// Returns `true(1)` if the window can be filled up with widgets from this point
/// until `nk_end` or `false(0)` otherwise for example if minimized
*/
NK_API nk_bool nk_begin(struct nk_context *ctx, const char *title, struct nk_rect bounds, nk_flags flags);
/*/// #### nk_begin_titled
/// Extended window start with separated title and identifier to allow multiple
/// windows with same title but not name
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_begin_titled(struct nk_context *ctx, const char *name, const char *title, struct nk_rect bounds, nk_flags flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Window identifier. Needs to be persistent over frames to identify the window
/// __title__   | Window title displayed inside header if flag `NK_WINDOW_TITLE` or either `NK_WINDOW_CLOSABLE` or `NK_WINDOW_MINIMIZED` was set
/// __bounds__  | Initial position and window size. However if you do not define `NK_WINDOW_SCALABLE` or `NK_WINDOW_MOVABLE` you can set window position and size every frame
/// __flags__   | Window flags defined in the nk_panel_flags section with a number of different window behaviors
///
/// Returns `true(1)` if the window can be filled up with widgets from this point
/// until `nk_end` or `false(0)` otherwise for example if minimized
*/
NK_API nk_bool nk_begin_titled(struct nk_context *ctx, const char *name, const char *title, struct nk_rect bounds, nk_flags flags);
/*/// #### nk_end
/// Needs to be called at the end of the window building process to process scaling, scrollbars and general cleanup.
/// All widget calls after this functions will result in asserts or no state changes
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_end(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
*/
NK_API void nk_end(struct nk_context *ctx);
/*/// #### nk_window_find
/// Finds and returns a window from passed name
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_window *nk_window_find(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Window identifier
///
/// Returns a `nk_window` struct pointing to the identified window or NULL if
/// no window with the given name was found
*/
NK_API struct nk_window *nk_window_find(struct nk_context *ctx, const char *name);
/*/// #### nk_window_get_bounds
/// Returns a rectangle with screen position and size of the currently processed window
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_window_get_bounds(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns a `nk_rect` struct with window upper left window position and size
*/
NK_API struct nk_rect nk_window_get_bounds(const struct nk_context *ctx);
/*/// #### nk_window_get_position
/// Returns the position of the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_window_get_position(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns a `nk_vec2` struct with window upper left position
*/
NK_API struct nk_vec2 nk_window_get_position(const struct nk_context *ctx);
/*/// #### nk_window_get_size
/// Returns the size with width and height of the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_window_get_size(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns a `nk_vec2` struct with window width and height
*/
NK_API struct nk_vec2 nk_window_get_size(const struct nk_context*);
/*/// #### nk_window_get_width
/// Returns the width of the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// float nk_window_get_width(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns the current window width
*/
NK_API float nk_window_get_width(const struct nk_context*);
/*/// #### nk_window_get_height
/// Returns the height of the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// float nk_window_get_height(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns the current window height
*/
NK_API float nk_window_get_height(const struct nk_context*);
/*/// #### nk_window_get_panel
/// Returns the underlying panel which contains all processing state of the current window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// !!! WARNING
///     Do not keep the returned panel pointer around, it is only valid until `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_panel* nk_window_get_panel(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns a pointer to window internal `nk_panel` state.
*/
NK_API struct nk_panel* nk_window_get_panel(struct nk_context*);
/*/// #### nk_window_get_content_region
/// Returns the position and size of the currently visible and non-clipped space
/// inside the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_window_get_content_region(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `nk_rect` struct with screen position and size (no scrollbar offset)
/// of the visible space inside the current window
*/
NK_API struct nk_rect nk_window_get_content_region(struct nk_context*);
/*/// #### nk_window_get_content_region_min
/// Returns the upper left position of the currently visible and non-clipped
/// space inside the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_window_get_content_region_min(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// returns `nk_vec2` struct with  upper left screen position (no scrollbar offset)
/// of the visible space inside the current window
*/
NK_API struct nk_vec2 nk_window_get_content_region_min(struct nk_context*);
/*/// #### nk_window_get_content_region_max
/// Returns the lower right screen position of the currently visible and
/// non-clipped space inside the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_window_get_content_region_max(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `nk_vec2` struct with lower right screen position (no scrollbar offset)
/// of the visible space inside the current window
*/
NK_API struct nk_vec2 nk_window_get_content_region_max(struct nk_context*);
/*/// #### nk_window_get_content_region_size
/// Returns the size of the currently visible and non-clipped space inside the
/// currently processed window
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_window_get_content_region_size(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `nk_vec2` struct with size the visible space inside the current window
*/
NK_API struct nk_vec2 nk_window_get_content_region_size(struct nk_context*);
/*/// #### nk_window_get_canvas
/// Returns the draw command buffer. Can be used to draw custom widgets
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// !!! WARNING
///     Do not keep the returned command buffer pointer around it is only valid until `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_command_buffer* nk_window_get_canvas(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns a pointer to window internal `nk_command_buffer` struct used as
/// drawing canvas. Can be used to do custom drawing.
*/
NK_API struct nk_command_buffer* nk_window_get_canvas(struct nk_context*);
/*/// #### nk_window_get_scroll
/// Gets the scroll offset for the current window
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_get_scroll(struct nk_context *ctx, nk_uint *offset_x, nk_uint *offset_y);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter    | Description
/// -------------|-----------------------------------------------------------
/// __ctx__      | Must point to an previously initialized `nk_context` struct
/// __offset_x__ | A pointer to the x offset output (or NULL to ignore)
/// __offset_y__ | A pointer to the y offset output (or NULL to ignore)
*/
NK_API void nk_window_get_scroll(struct nk_context*, nk_uint *offset_x, nk_uint *offset_y);
/*/// #### nk_window_has_focus
/// Returns if the currently processed window is currently active
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_has_focus(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `false(0)` if current window is not active or `true(1)` if it is
*/
NK_API nk_bool nk_window_has_focus(const struct nk_context*);
/*/// #### nk_window_is_hovered
/// Return if the current window is being hovered
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_hovered(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `true(1)` if current window is hovered or `false(0)` otherwise
*/
NK_API nk_bool nk_window_is_hovered(struct nk_context*);
/*/// #### nk_window_is_collapsed
/// Returns if the window with given name is currently minimized/collapsed
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_collapsed(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of window you want to check if it is collapsed
///
/// Returns `true(1)` if current window is minimized and `false(0)` if window not
/// found or is not minimized
*/
NK_API nk_bool nk_window_is_collapsed(struct nk_context *ctx, const char *name);
/*/// #### nk_window_is_closed
/// Returns if the window with given name was closed by calling `nk_close`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_closed(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of window you want to check if it is closed
///
/// Returns `true(1)` if current window was closed or `false(0)` window not found or not closed
*/
NK_API nk_bool nk_window_is_closed(struct nk_context*, const char*);
/*/// #### nk_window_is_hidden
/// Returns if the window with given name is hidden
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_hidden(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of window you want to check if it is hidden
///
/// Returns `true(1)` if current window is hidden or `false(0)` window not found or visible
*/
NK_API nk_bool nk_window_is_hidden(struct nk_context*, const char*);
/*/// #### nk_window_is_active
/// Same as nk_window_has_focus for some reason
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_active(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of window you want to check if it is active
///
/// Returns `true(1)` if current window is active or `false(0)` window not found or not active
*/
NK_API nk_bool nk_window_is_active(struct nk_context*, const char*);
/*/// #### nk_window_is_any_hovered
/// Returns if the any window is being hovered
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_any_hovered(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `true(1)` if any window is hovered or `false(0)` otherwise
*/
NK_API nk_bool nk_window_is_any_hovered(struct nk_context*);
/*/// #### nk_item_is_any_active
/// Returns if the any window is being hovered or any widget is currently active.
/// Can be used to decide if input should be processed by UI or your specific input handling.
/// Example could be UI and 3D camera to move inside a 3D space.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_item_is_any_active(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `true(1)` if any window is hovered or any item is active or `false(0)` otherwise
*/
NK_API nk_bool nk_item_is_any_active(struct nk_context*);
/*/// #### nk_window_set_bounds
/// Updates position and size of window with passed in name
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_set_bounds(struct nk_context*, const char *name, struct nk_rect bounds);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to modify both position and size
/// __bounds__  | Must point to a `nk_rect` struct with the new position and size
*/
NK_API void nk_window_set_bounds(struct nk_context*, const char *name, struct nk_rect bounds);
/*/// #### nk_window_set_position
/// Updates position of window with passed name
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_set_position(struct nk_context*, const char *name, struct nk_vec2 pos);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to modify both position
/// __pos__     | Must point to a `nk_vec2` struct with the new position
*/
NK_API void nk_window_set_position(struct nk_context*, const char *name, struct nk_vec2 pos);
/*/// #### nk_window_set_size
/// Updates size of window with passed in name
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_set_size(struct nk_context*, const char *name, struct nk_vec2);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to modify both window size
/// __size__    | Must point to a `nk_vec2` struct with new window size
*/
NK_API void nk_window_set_size(struct nk_context*, const char *name, struct nk_vec2);
/*/// #### nk_window_set_focus
/// Sets the window with given name as active
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_set_focus(struct nk_context*, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to set focus on
*/
NK_API void nk_window_set_focus(struct nk_context*, const char *name);
/*/// #### nk_window_set_scroll
/// Sets the scroll offset for the current window
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_set_scroll(struct nk_context *ctx, nk_uint offset_x, nk_uint offset_y);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter    | Description
/// -------------|-----------------------------------------------------------
/// __ctx__      | Must point to an previously initialized `nk_context` struct
/// __offset_x__ | The x offset to scroll to
/// __offset_y__ | The y offset to scroll to
*/
NK_API void nk_window_set_scroll(struct nk_context*, nk_uint offset_x, nk_uint offset_y);
/*/// #### nk_window_close
/// Closes a window and marks it for being freed at the end of the frame
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_close(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to close
*/
NK_API void nk_window_close(struct nk_context *ctx, const char *name);
/*/// #### nk_window_collapse
/// Updates collapse state of a window with given name
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_collapse(struct nk_context*, const char *name, enum nk_collapse_states state);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to close
/// __state__   | value out of nk_collapse_states section
*/
NK_API void nk_window_collapse(struct nk_context*, const char *name, enum nk_collapse_states state);
/*/// #### nk_window_collapse_if
/// Updates collapse state of a window with given name if given condition is met
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_collapse_if(struct nk_context*, const char *name, enum nk_collapse_states, int cond);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to either collapse or maximize
/// __state__   | value out of nk_collapse_states section the window should be put into
/// __cond__    | condition that has to be met to actually commit the collapse state change
*/
NK_API void nk_window_collapse_if(struct nk_context*, const char *name, enum nk_collapse_states, int cond);
/*/// #### nk_window_show
/// updates visibility state of a window with given name
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_show(struct nk_context*, const char *name, enum nk_show_states);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to either collapse or maximize
/// __state__   | state with either visible or hidden to modify the window with
*/
NK_API void nk_window_show(struct nk_context*, const char *name, enum nk_show_states);
/*/// #### nk_window_show_if
/// Updates visibility state of a window with given name if a given condition is met
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_show_if(struct nk_context*, const char *name, enum nk_show_states, int cond);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to either hide or show
/// __state__   | state with either visible or hidden to modify the window with
/// __cond__    | condition that has to be met to actually commit the visbility state change
*/
NK_API void nk_window_show_if(struct nk_context*, const char *name, enum nk_show_states, int cond);

/* =============================================================================
 *
 *                                  LAYOUT
 *
 * =============================================================================
/// ### Layouting
/// Layouting in general describes placing widget inside a window with position and size.
/// While in this particular implementation there are five different APIs for layouting
/// each with different trade offs between control and ease of use. <br /><br />
///
/// All layouting methods in this library are based around the concept of a row.
/// A row has a height the window content grows by and a number of columns and each
/// layouting method specifies how each widget is placed inside the row.
/// After a row has been allocated by calling a layouting functions and then
/// filled with widgets will advance an internal pointer over the allocated row. <br /><br />
///
/// To actually define a layout you just call the appropriate layouting function
/// and each subsequent widget call will place the widget as specified. Important
/// here is that if you define more widgets then columns defined inside the layout
/// functions it will allocate the next row without you having to make another layouting <br /><br />
/// call.
///
/// Biggest limitation with using all these APIs outside the `nk_layout_space_xxx` API
/// is that you have to define the row height for each. However the row height
/// often depends on the height of the font. <br /><br />
///
/// To fix that internally nuklear uses a minimum row height that is set to the
/// height plus padding of currently active font and overwrites the row height
/// value if zero. <br /><br />
///
/// If you manually want to change the minimum row height then
/// use nk_layout_set_min_row_height, and use nk_layout_reset_min_row_height to
/// reset it back to be derived from font height. <br /><br />
///
/// Also if you change the font in nuklear it will automatically change the minimum
/// row height for you and. This means if you change the font but still want
/// a minimum row height smaller than the font you have to repush your value. <br /><br />
///
/// For actually more advanced UI I would even recommend using the `nk_layout_space_xxx`
/// layouting method in combination with a cassowary constraint solver (there are
/// some versions on github with permissive license model) to take over all control over widget
/// layouting yourself. However for quick and dirty layouting using all the other layouting
/// functions should be fine.
///
/// #### Usage
/// 1.  __nk_layout_row_dynamic__<br /><br />
///     The easiest layouting function is `nk_layout_row_dynamic`. It provides each
///     widgets with same horizontal space inside the row and dynamically grows
///     if the owning window grows in width. So the number of columns dictates
///     the size of each widget dynamically by formula:
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     widget_width = (window_width - padding - spacing) * (1/colum_count)
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
///     Just like all other layouting APIs if you define more widget than columns this
///     library will allocate a new row and keep all layouting parameters previously
///     defined.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // first row with height: 30 composed of two widgets
///         nk_layout_row_dynamic(&ctx, 30, 2);
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // second row with same parameter as defined above
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // third row uses 0 for height which will use auto layouting
///         nk_layout_row_dynamic(&ctx, 0, 2);
///         nk_widget(...);
///         nk_widget(...);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// 2.  __nk_layout_row_static__<br /><br />
///     Another easy layouting function is `nk_layout_row_static`. It provides each
///     widget with same horizontal pixel width inside the row and does not grow
///     if the owning window scales smaller or bigger.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // first row with height: 30 composed of two widgets with width: 80
///         nk_layout_row_static(&ctx, 30, 80, 2);
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // second row with same parameter as defined above
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // third row uses 0 for height which will use auto layouting
///         nk_layout_row_static(&ctx, 0, 80, 2);
///         nk_widget(...);
///         nk_widget(...);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// 3.  __nk_layout_row_xxx__<br /><br />
///     A little bit more advanced layouting API are functions `nk_layout_row_begin`,
///     `nk_layout_row_push` and `nk_layout_row_end`. They allow to directly
///     specify each column pixel or window ratio in a row. It supports either
///     directly setting per column pixel width or widget window ratio but not
///     both. Furthermore it is a immediate mode API so each value is directly
///     pushed before calling a widget. Therefore the layout is not automatically
///     repeating like the last two layouting functions.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // first row with height: 25 composed of two widgets with width 60 and 40
///         nk_layout_row_begin(ctx, NK_STATIC, 25, 2);
///         nk_layout_row_push(ctx, 60);
///         nk_widget(...);
///         nk_layout_row_push(ctx, 40);
///         nk_widget(...);
///         nk_layout_row_end(ctx);
///         //
///         // second row with height: 25 composed of two widgets with window ratio 0.25 and 0.75
///         nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 2);
///         nk_layout_row_push(ctx, 0.25f);
///         nk_widget(...);
///         nk_layout_row_push(ctx, 0.75f);
///         nk_widget(...);
///         nk_layout_row_end(ctx);
///         //
///         // third row with auto generated height: composed of two widgets with window ratio 0.25 and 0.75
///         nk_layout_row_begin(ctx, NK_DYNAMIC, 0, 2);
///         nk_layout_row_push(ctx, 0.25f);
///         nk_widget(...);
///         nk_layout_row_push(ctx, 0.75f);
///         nk_widget(...);
///         nk_layout_row_end(ctx);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// 4.  __nk_layout_row__<br /><br />
///     The array counterpart to API nk_layout_row_xxx is the single nk_layout_row
///     functions. Instead of pushing either pixel or window ratio for every widget
///     it allows to define it by array. The trade of for less control is that
///     `nk_layout_row` is automatically repeating. Otherwise the behavior is the
///     same.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // two rows with height: 30 composed of two widgets with width 60 and 40
///         const float size[] = {60,40};
///         nk_layout_row(ctx, NK_STATIC, 30, 2, ratio);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // two rows with height: 30 composed of two widgets with window ratio 0.25 and 0.75
///         const float ratio[] = {0.25, 0.75};
///         nk_layout_row(ctx, NK_DYNAMIC, 30, 2, ratio);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // two rows with auto generated height composed of two widgets with window ratio 0.25 and 0.75
///         const float ratio[] = {0.25, 0.75};
///         nk_layout_row(ctx, NK_DYNAMIC, 30, 2, ratio);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// 5.  __nk_layout_row_template_xxx__<br /><br />
///     The most complex and second most flexible API is a simplified flexbox version without
///     line wrapping and weights for dynamic widgets. It is an immediate mode API but
///     unlike `nk_layout_row_xxx` it has auto repeat behavior and needs to be called
///     before calling the templated widgets.
///     The row template layout has three different per widget size specifier. The first
///     one is the `nk_layout_row_template_push_static`  with fixed widget pixel width.
///     They do not grow if the row grows and will always stay the same.
///     The second size specifier is `nk_layout_row_template_push_variable`
///     which defines a minimum widget size but it also can grow if more space is available
///     not taken by other widgets.
///     Finally there are dynamic widgets with `nk_layout_row_template_push_dynamic`
///     which are completely flexible and unlike variable widgets can even shrink
///     to zero if not enough space is provided.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // two rows with height: 30 composed of three widgets
///         nk_layout_row_template_begin(ctx, 30);
///         nk_layout_row_template_push_dynamic(ctx);
///         nk_layout_row_template_push_variable(ctx, 80);
///         nk_layout_row_template_push_static(ctx, 80);
///         nk_layout_row_template_end(ctx);
///         //
///         // first row
///         nk_widget(...); // dynamic widget can go to zero if not enough space
///         nk_widget(...); // variable widget with min 80 pixel but can grow bigger if enough space
///         nk_widget(...); // static widget with fixed 80 pixel width
///         //
///         // second row same layout
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// 6.  __nk_layout_space_xxx__<br /><br />
///     Finally the most flexible API directly allows you to place widgets inside the
///     window. The space layout API is an immediate mode API which does not support
///     row auto repeat and directly sets position and size of a widget. Position
///     and size hereby can be either specified as ratio of allocated space or
///     allocated space local position and pixel size. Since this API is quite
///     powerful there are a number of utility functions to get the available space
///     and convert between local allocated space and screen space.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // static row with height: 500 (you can set column count to INT_MAX if you don't want to be bothered)
///         nk_layout_space_begin(ctx, NK_STATIC, 500, INT_MAX);
///         nk_layout_space_push(ctx, nk_rect(0,0,150,200));
///         nk_widget(...);
///         nk_layout_space_push(ctx, nk_rect(200,200,100,200));
///         nk_widget(...);
///         nk_layout_space_end(ctx);
///         //
///         // dynamic row with height: 500 (you can set column count to INT_MAX if you don't want to be bothered)
///         nk_layout_space_begin(ctx, NK_DYNAMIC, 500, INT_MAX);
///         nk_layout_space_push(ctx, nk_rect(0.5,0.5,0.1,0.1));
///         nk_widget(...);
///         nk_layout_space_push(ctx, nk_rect(0.7,0.6,0.1,0.1));
///         nk_widget(...);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function                                | Description
/// ----------------------------------------|------------------------------------
/// nk_layout_set_min_row_height            | Set the currently used minimum row height to a specified value
/// nk_layout_reset_min_row_height          | Resets the currently used minimum row height to font height
/// nk_layout_widget_bounds                 | Calculates current width a static layout row can fit inside a window
/// nk_layout_ratio_from_pixel              | Utility functions to calculate window ratio from pixel size
//
/// nk_layout_row_dynamic                   | Current layout is divided into n same sized growing columns
/// nk_layout_row_static                    | Current layout is divided into n same fixed sized columns
/// nk_layout_row_begin                     | Starts a new row with given height and number of columns
/// nk_layout_row_push                      | Pushes another column with given size or window ratio
/// nk_layout_row_end                       | Finished previously started row
/// nk_layout_row                           | Specifies row columns in array as either window ratio or size
//
/// nk_layout_row_template_begin            | Begins the row template declaration
/// nk_layout_row_template_push_dynamic     | Adds a dynamic column that dynamically grows and can go to zero if not enough space
/// nk_layout_row_template_push_variable    | Adds a variable column that dynamically grows but does not shrink below specified pixel width
/// nk_layout_row_template_push_static      | Adds a static column that does not grow and will always have the same size
/// nk_layout_row_template_end              | Marks the end of the row template
//
/// nk_layout_space_begin                   | Begins a new layouting space that allows to specify each widgets position and size
/// nk_layout_space_push                    | Pushes position and size of the next widget in own coordinate space either as pixel or ratio
/// nk_layout_space_end                     | Marks the end of the layouting space
//
/// nk_layout_space_bounds                  | Callable after nk_layout_space_begin and returns total space allocated
/// nk_layout_space_to_screen               | Converts vector from nk_layout_space coordinate space into screen space
/// nk_layout_space_to_local                | Converts vector from screen space into nk_layout_space coordinates
/// nk_layout_space_rect_to_screen          | Converts rectangle from nk_layout_space coordinate space into screen space
/// nk_layout_space_rect_to_local           | Converts rectangle from screen space into nk_layout_space coordinates
*/
/*/// #### nk_layout_set_min_row_height
/// Sets the currently used minimum row height.
/// !!! WARNING
///     The passed height needs to include both your preferred row height
///     as well as padding. No internal padding is added.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_set_min_row_height(struct nk_context*, float height);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __height__  | New minimum row height to be used for auto generating the row height
*/

NK_API void nk_layout_set_min_row_height(struct nk_context*, float height);
/*/// #### nk_layout_reset_min_row_height
/// Reset the currently used minimum row height back to `font_height + text_padding + padding`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_reset_min_row_height(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
*/
NK_API void nk_layout_reset_min_row_height(struct nk_context*);
/*/// #### nk_layout_widget_bounds
/// Returns the width of the next row allocate by one of the layouting functions
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_layout_widget_bounds(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
///
/// Return `nk_rect` with both position and size of the next row
*/
NK_API struct nk_rect nk_layout_widget_bounds(struct nk_context*);
/*/// #### nk_layout_ratio_from_pixel
/// Utility functions to calculate window ratio from pixel size
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// float nk_layout_ratio_from_pixel(struct nk_context*, float pixel_width);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __pixel__   | Pixel_width to convert to window ratio
///
/// Returns `nk_rect` with both position and size of the next row
*/
NK_API float nk_layout_ratio_from_pixel(struct nk_context*, float pixel_width);
/*/// #### nk_layout_row_dynamic
/// Sets current row layout to share horizontal space
/// between @cols number of widgets evenly. Once called all subsequent widget
/// calls greater than @cols will allocate a new row with same layout.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_dynamic(struct nk_context *ctx, float height, int cols);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __height__  | Holds height of each widget in row or zero for auto layouting
/// __columns__ | Number of widget inside row
*/
NK_API void nk_layout_row_dynamic(struct nk_context *ctx, float height, int cols);
/*/// #### nk_layout_row_static
/// Sets current row layout to fill @cols number of widgets
/// in row with same @item_width horizontal size. Once called all subsequent widget
/// calls greater than @cols will allocate a new row with same layout.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_static(struct nk_context *ctx, float height, int item_width, int cols);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __height__  | Holds height of each widget in row or zero for auto layouting
/// __width__   | Holds pixel width of each widget in the row
/// __columns__ | Number of widget inside row
*/
NK_API void nk_layout_row_static(struct nk_context *ctx, float height, int item_width, int cols);
/*/// #### nk_layout_row_begin
/// Starts a new dynamic or fixed row with given height and columns.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_begin(struct nk_context *ctx, enum nk_layout_format fmt, float row_height, int cols);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __fmt__     | either `NK_DYNAMIC` for window ratio or `NK_STATIC` for fixed size columns
/// __height__  | holds height of each widget in row or zero for auto layouting
/// __columns__ | Number of widget inside row
*/
NK_API void nk_layout_row_begin(struct nk_context *ctx, enum nk_layout_format fmt, float row_height, int cols);
/*/// #### nk_layout_row_push
/// Specifies either window ratio or width of a single column
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_push(struct nk_context*, float value);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __value__   | either a window ratio or fixed width depending on @fmt in previous `nk_layout_row_begin` call
*/
NK_API void nk_layout_row_push(struct nk_context*, float value);
/*/// #### nk_layout_row_end
/// Finished previously started row
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_end(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
*/
NK_API void nk_layout_row_end(struct nk_context*);
/*/// #### nk_layout_row
/// Specifies row columns in array as either window ratio or size
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row(struct nk_context*, enum nk_layout_format, float height, int cols, const float *ratio);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __fmt__     | Either `NK_DYNAMIC` for window ratio or `NK_STATIC` for fixed size columns
/// __height__  | Holds height of each widget in row or zero for auto layouting
/// __columns__ | Number of widget inside row
*/
NK_API void nk_layout_row(struct nk_context*, enum nk_layout_format, float height, int cols, const float *ratio);
/*/// #### nk_layout_row_template_begin
/// Begins the row template declaration
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_template_begin(struct nk_context*, float row_height);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __height__  | Holds height of each widget in row or zero for auto layouting
*/
NK_API void nk_layout_row_template_begin(struct nk_context*, float row_height);
/*/// #### nk_layout_row_template_push_dynamic
/// Adds a dynamic column that dynamically grows and can go to zero if not enough space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_template_push_dynamic(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __height__  | Holds height of each widget in row or zero for auto layouting
*/
NK_API void nk_layout_row_template_push_dynamic(struct nk_context*);
/*/// #### nk_layout_row_template_push_variable
/// Adds a variable column that dynamically grows but does not shrink below specified pixel width
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_template_push_variable(struct nk_context*, float min_width);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __width__   | Holds the minimum pixel width the next column must always be
*/
NK_API void nk_layout_row_template_push_variable(struct nk_context*, float min_width);
/*/// #### nk_layout_row_template_push_static
/// Adds a static column that does not grow and will always have the same size
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_template_push_static(struct nk_context*, float width);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __width__   | Holds the absolute pixel width value the next column must be
*/
NK_API void nk_layout_row_template_push_static(struct nk_context*, float width);
/*/// #### nk_layout_row_template_end
/// Marks the end of the row template
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_template_end(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
*/
NK_API void nk_layout_row_template_end(struct nk_context*);
/*/// #### nk_layout_space_begin
/// Begins a new layouting space that allows to specify each widgets position and size.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_space_begin(struct nk_context*, enum nk_layout_format, float height, int widget_count);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __fmt__     | Either `NK_DYNAMIC` for window ratio or `NK_STATIC` for fixed size columns
/// __height__  | Holds height of each widget in row or zero for auto layouting
/// __columns__ | Number of widgets inside row
*/
NK_API void nk_layout_space_begin(struct nk_context*, enum nk_layout_format, float height, int widget_count);
/*/// #### nk_layout_space_push
/// Pushes position and size of the next widget in own coordinate space either as pixel or ratio
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_space_push(struct nk_context *ctx, struct nk_rect bounds);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
/// __bounds__  | Position and size in laoyut space local coordinates
*/
NK_API void nk_layout_space_push(struct nk_context*, struct nk_rect bounds);
/*/// #### nk_layout_space_end
/// Marks the end of the layout space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_space_end(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
*/
NK_API void nk_layout_space_end(struct nk_context*);
/*/// #### nk_layout_space_bounds
/// Utility function to calculate total space allocated for `nk_layout_space`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_layout_space_bounds(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
///
/// Returns `nk_rect` holding the total space allocated
*/
NK_API struct nk_rect nk_layout_space_bounds(struct nk_context*);
/*/// #### nk_layout_space_to_screen
/// Converts vector from nk_layout_space coordinate space into screen space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_layout_space_to_screen(struct nk_context*, struct nk_vec2);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
/// __vec__     | Position to convert from layout space into screen coordinate space
///
/// Returns transformed `nk_vec2` in screen space coordinates
*/
NK_API struct nk_vec2 nk_layout_space_to_screen(struct nk_context*, struct nk_vec2);
/*/// #### nk_layout_space_to_local
/// Converts vector from layout space into screen space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_layout_space_to_local(struct nk_context*, struct nk_vec2);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
/// __vec__     | Position to convert from screen space into layout coordinate space
///
/// Returns transformed `nk_vec2` in layout space coordinates
*/
NK_API struct nk_vec2 nk_layout_space_to_local(struct nk_context*, struct nk_vec2);
/*/// #### nk_layout_space_rect_to_screen
/// Converts rectangle from screen space into layout space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_layout_space_rect_to_screen(struct nk_context*, struct nk_rect);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
/// __bounds__  | Rectangle to convert from layout space into screen space
///
/// Returns transformed `nk_rect` in screen space coordinates
*/
NK_API struct nk_rect nk_layout_space_rect_to_screen(struct nk_context*, struct nk_rect);
/*/// #### nk_layout_space_rect_to_local
/// Converts rectangle from layout space into screen space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_layout_space_rect_to_local(struct nk_context*, struct nk_rect);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
/// __bounds__  | Rectangle to convert from layout space into screen space
///
/// Returns transformed `nk_rect` in layout space coordinates
*/
NK_API struct nk_rect nk_layout_space_rect_to_local(struct nk_context*, struct nk_rect);

/*/// #### nk_spacer
/// Spacer is a dummy widget that consumes space as usual but doesn't draw anything
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_spacer(struct nk_context* );
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
///
*/
NK_API void nk_spacer(struct nk_context* );


/* =============================================================================
 *
 *                                  GROUP
 *
 * =============================================================================
/// ### Groups
/// Groups are basically windows inside windows. They allow to subdivide space
/// in a window to layout widgets as a group. Almost all more complex widget
/// layouting requirements can be solved using groups and basic layouting
/// fuctionality. Groups just like windows are identified by an unique name and
/// internally keep track of scrollbar offsets by default. However additional
/// versions are provided to directly manage the scrollbar.
///
/// #### Usage
/// To create a group you have to call one of the three `nk_group_begin_xxx`
/// functions to start group declarations and `nk_group_end` at the end. Furthermore it
/// is required to check the return value of `nk_group_begin_xxx` and only process
/// widgets inside the window if the value is not 0.
/// Nesting groups is possible and even encouraged since many layouting schemes
/// can only be achieved by nesting. Groups, unlike windows, need `nk_group_end`
/// to be only called if the corresponding `nk_group_begin_xxx` call does not return 0:
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// if (nk_group_begin_xxx(ctx, ...) {
///     // [... widgets ...]
///     nk_group_end(ctx);
/// }
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// In the grand concept groups can be called after starting a window
/// with `nk_begin_xxx` and before calling `nk_end`:
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     // Input
///     Event evt;
///     nk_input_begin(&ctx);
///     while (GetEvent(&evt)) {
///         if (evt.type == MOUSE_MOVE)
///             nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///         else if (evt.type == [...]) {
///             nk_input_xxx(...);
///         }
///     }
///     nk_input_end(&ctx);
///     //
///     // Window
///     if (nk_begin_xxx(...) {
///         // [...widgets...]
///         nk_layout_row_dynamic(...);
///         if (nk_group_begin_xxx(ctx, ...) {
///             //[... widgets ...]
///             nk_group_end(ctx);
///         }
///     }
///     nk_end(ctx);
///     //
///     // Draw
///     const struct nk_command *cmd = 0;
///     nk_foreach(cmd, &ctx) {
///     switch (cmd->type) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case ...:
///         // [...]
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// #### Reference
/// Function                        | Description
/// --------------------------------|-------------------------------------------
/// nk_group_begin                  | Start a new group with internal scrollbar handling
/// nk_group_begin_titled           | Start a new group with separated name and title and internal scrollbar handling
/// nk_group_end                    | Ends a group. Should only be called if nk_group_begin returned non-zero
/// nk_group_scrolled_offset_begin  | Start a new group with manual separated handling of scrollbar x- and y-offset
/// nk_group_scrolled_begin         | Start a new group with manual scrollbar handling
/// nk_group_scrolled_end           | Ends a group with manual scrollbar handling. Should only be called if nk_group_begin returned non-zero
/// nk_group_get_scroll             | Gets the scroll offset for the given group
/// nk_group_set_scroll             | Sets the scroll offset for the given group
*/
/*/// #### nk_group_begin
/// Starts a new widget group. Requires a previous layouting function to specify a pos/size.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_group_begin(struct nk_context*, const char *title, nk_flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __title__   | Must be an unique identifier for this group that is also used for the group header
/// __flags__   | Window flags defined in the nk_panel_flags section with a number of different group behaviors
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_group_begin(struct nk_context*, const char *title, nk_flags);
/*/// #### nk_group_begin_titled
/// Starts a new widget group. Requires a previous layouting function to specify a pos/size.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_group_begin_titled(struct nk_context*, const char *name, const char *title, nk_flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __id__      | Must be an unique identifier for this group
/// __title__   | Group header title
/// __flags__   | Window flags defined in the nk_panel_flags section with a number of different group behaviors
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_group_begin_titled(struct nk_context*, const char *name, const char *title, nk_flags);
/*/// #### nk_group_end
/// Ends a widget group
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_group_end(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
*/
NK_API void nk_group_end(struct nk_context*);
/*/// #### nk_group_scrolled_offset_begin
/// starts a new widget group. requires a previous layouting function to specify
/// a size. Does not keep track of scrollbar.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_group_scrolled_offset_begin(struct nk_context*, nk_uint *x_offset, nk_uint *y_offset, const char *title, nk_flags flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __x_offset__| Scrollbar x-offset to offset all widgets inside the group horizontally.
/// __y_offset__| Scrollbar y-offset to offset all widgets inside the group vertically
/// __title__   | Window unique group title used to both identify and display in the group header
/// __flags__   | Window flags from the nk_panel_flags section
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_group_scrolled_offset_begin(struct nk_context*, nk_uint *x_offset, nk_uint *y_offset, const char *title, nk_flags flags);
/*/// #### nk_group_scrolled_begin
/// Starts a new widget group. requires a previous
/// layouting function to specify a size. Does not keep track of scrollbar.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_group_scrolled_begin(struct nk_context*, struct nk_scroll *off, const char *title, nk_flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __off__     | Both x- and y- scroll offset. Allows for manual scrollbar control
/// __title__   | Window unique group title used to both identify and display in the group header
/// __flags__   | Window flags from nk_panel_flags section
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_group_scrolled_begin(struct nk_context*, struct nk_scroll *off, const char *title, nk_flags);
/*/// #### nk_group_scrolled_end
/// Ends a widget group after calling nk_group_scrolled_offset_begin or nk_group_scrolled_begin.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_group_scrolled_end(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
*/
NK_API void nk_group_scrolled_end(struct nk_context*);
/*/// #### nk_group_get_scroll
/// Gets the scroll position of the given group.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_group_get_scroll(struct nk_context*, const char *id, nk_uint *x_offset, nk_uint *y_offset);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter    | Description
/// -------------|-----------------------------------------------------------
/// __ctx__      | Must point to an previously initialized `nk_context` struct
/// __id__       | The id of the group to get the scroll position of
/// __x_offset__ | A pointer to the x offset output (or NULL to ignore)
/// __y_offset__ | A pointer to the y offset output (or NULL to ignore)
*/
NK_API void nk_group_get_scroll(struct nk_context*, const char *id, nk_uint *x_offset, nk_uint *y_offset);
/*/// #### nk_group_set_scroll
/// Sets the scroll position of the given group.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_group_set_scroll(struct nk_context*, const char *id, nk_uint x_offset, nk_uint y_offset);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter    | Description
/// -------------|-----------------------------------------------------------
/// __ctx__      | Must point to an previously initialized `nk_context` struct
/// __id__       | The id of the group to scroll
/// __x_offset__ | The x offset to scroll to
/// __y_offset__ | The y offset to scroll to
*/

NK_API void nk_group_set_scroll(struct nk_context*, const char *id, nk_uint x_offset, nk_uint y_offset);

/* =============================================================================
 *
 *                                  TREE
 *
 * =============================================================================
/// ### Tree
/// Trees represent two different concept. First the concept of a collapsible
/// UI section that can be either in a hidden or visible state. They allow the UI
/// user to selectively minimize the current set of visible UI to comprehend.
/// The second concept are tree widgets for visual UI representation of trees.<br /><br />
///
/// Trees thereby can be nested for tree representations and multiple nested
/// collapsible UI sections. All trees are started by calling of the
/// `nk_tree_xxx_push_tree` functions and ended by calling one of the
/// `nk_tree_xxx_pop_xxx()` functions. Each starting functions takes a title label
/// and optionally an image to be displayed and the initial collapse state from
/// the nk_collapse_states section.<br /><br />
///
/// The runtime state of the tree is either stored outside the library by the caller
/// or inside which requires a unique ID. The unique ID can either be generated
/// automatically from `__FILE__` and `__LINE__` with function `nk_tree_push`,
/// by `__FILE__` and a user provided ID generated for example by loop index with
/// function `nk_tree_push_id` or completely provided from outside by user with
/// function `nk_tree_push_hashed`.
///
/// #### Usage
/// To create a tree you have to call one of the seven `nk_tree_xxx_push_xxx`
/// functions to start a collapsible UI section and `nk_tree_xxx_pop` to mark the
/// end.
/// Each starting function will either return `false(0)` if the tree is collapsed
/// or hidden and therefore does not need to be filled with content or `true(1)`
/// if visible and required to be filled.
///
/// !!! Note
///     The tree header does not require and layouting function and instead
///     calculates a auto height based on the currently used font size
///
/// The tree ending functions only need to be called if the tree content is
/// actually visible. So make sure the tree push function is guarded by `if`
/// and the pop call is only taken if the tree is visible.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// if (nk_tree_push(ctx, NK_TREE_TAB, "Tree", NK_MINIMIZED)) {
///     nk_layout_row_dynamic(...);
///     nk_widget(...);
///     nk_tree_pop(ctx);
/// }
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function                    | Description
/// ----------------------------|-------------------------------------------
/// nk_tree_push                | Start a collapsible UI section with internal state management
/// nk_tree_push_id             | Start a collapsible UI section with internal state management callable in a look
/// nk_tree_push_hashed         | Start a collapsible UI section with internal state management with full control over internal unique ID use to store state
/// nk_tree_image_push          | Start a collapsible UI section with image and label header
/// nk_tree_image_push_id       | Start a collapsible UI section with image and label header and internal state management callable in a look
/// nk_tree_image_push_hashed   | Start a collapsible UI section with image and label header and internal state management with full control over internal unique ID use to store state
/// nk_tree_pop                 | Ends a collapsible UI section
//
/// nk_tree_state_push          | Start a collapsible UI section with external state management
/// nk_tree_state_image_push    | Start a collapsible UI section with image and label header and external state management
/// nk_tree_state_pop           | Ends a collapsabale UI section
///
/// #### nk_tree_type
/// Flag            | Description
/// ----------------|----------------------------------------
/// NK_TREE_NODE    | Highlighted tree header to mark a collapsible UI section
/// NK_TREE_TAB     | Non-highlighted tree header closer to tree representations
*/
/*/// #### nk_tree_push
/// Starts a collapsible UI section with internal state management
/// !!! WARNING
///     To keep track of the runtime tree collapsible state this function uses
///     defines `__FILE__` and `__LINE__` to generate a unique ID. If you want
///     to call this function in a loop please use `nk_tree_push_id` or
///     `nk_tree_push_hashed` instead.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_tree_push(ctx, type, title, state)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
#define nk_tree_push(ctx, type, title, state) nk_tree_push_hashed(ctx, type, title, state, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),__LINE__)
/*/// #### nk_tree_push_id
/// Starts a collapsible UI section with internal state management callable in a look
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_tree_push_id(ctx, type, title, state, id)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
/// __id__      | Loop counter index if this function is called in a loop
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
#define nk_tree_push_id(ctx, type, title, state, id) nk_tree_push_hashed(ctx, type, title, state, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),id)
/*/// #### nk_tree_push_hashed
/// Start a collapsible UI section with internal state management with full
/// control over internal unique ID used to store state
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_tree_push_hashed(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states initial_state, const char *hash, int len,int seed);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
/// __hash__    | Memory block or string to generate the ID from
/// __len__     | Size of passed memory block or string in __hash__
/// __seed__    | Seeding value if this function is called in a loop or default to `0`
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
/*/// #### nk_tree_push_hashed
/// Start a collapsible UI section with internal state management with full
/// control over internal unique ID used to store state
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_tree_push_hashed(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states initial_state, const char *hash, int len,int seed);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
/// __hash__    | Memory block or string to generate the ID from
/// __len__     | Size of passed memory block or string in __hash__
/// __seed__    | Seeding value if this function is called in a loop or default to `0`
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_tree_push_hashed(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states initial_state, const char *hash, int len,int seed);
/*/// #### nk_tree_image_push
/// Start a collapsible UI section with image and label header
/// !!! WARNING
///     To keep track of the runtime tree collapsible state this function uses
///     defines `__FILE__` and `__LINE__` to generate a unique ID. If you want
///     to call this function in a loop please use `nk_tree_image_push_id` or
///     `nk_tree_image_push_hashed` instead.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_tree_image_push(ctx, type, img, title, state)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __img__     | Image to display inside the header on the left of the label
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
#define nk_tree_image_push(ctx, type, img, title, state) nk_tree_image_push_hashed(ctx, type, img, title, state, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),__LINE__)
/*/// #### nk_tree_image_push_id
/// Start a collapsible UI section with image and label header and internal state
/// management callable in a look
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_tree_image_push_id(ctx, type, img, title, state, id)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __img__     | Image to display inside the header on the left of the label
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
/// __id__      | Loop counter index if this function is called in a loop
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
#define nk_tree_image_push_id(ctx, type, img, title, state, id) nk_tree_image_push_hashed(ctx, type, img, title, state, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),id)
/*/// #### nk_tree_image_push_hashed
/// Start a collapsible UI section with internal state management with full
/// control over internal unique ID used to store state
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_tree_image_push_hashed(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states initial_state, const char *hash, int len,int seed);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __img__     | Image to display inside the header on the left of the label
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
/// __hash__    | Memory block or string to generate the ID from
/// __len__     | Size of passed memory block or string in __hash__
/// __seed__    | Seeding value if this function is called in a loop or default to `0`
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_tree_image_push_hashed(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states initial_state, const char *hash, int len,int seed);
/*/// #### nk_tree_pop
/// Ends a collapsabale UI section
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_tree_pop(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after calling `nk_tree_xxx_push_xxx`
*/
NK_API void nk_tree_pop(struct nk_context*);
/*/// #### nk_tree_state_push
/// Start a collapsible UI section with external state management
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_tree_state_push(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states *state);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after calling `nk_tree_xxx_push_xxx`
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __title__   | Label printed in the tree header
/// __state__   | Persistent state to update
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_tree_state_push(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states *state);
/*/// #### nk_tree_state_image_push
/// Start a collapsible UI section with image and label header and external state management
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_tree_state_image_push(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states *state);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after calling `nk_tree_xxx_push_xxx`
/// __img__     | Image to display inside the header on the left of the label
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __title__   | Label printed in the tree header
/// __state__   | Persistent state to update
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_tree_state_image_push(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states *state);
/*/// #### nk_tree_state_pop
/// Ends a collapsabale UI section
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_tree_state_pop(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after calling `nk_tree_xxx_push_xxx`
*/
NK_API void nk_tree_state_pop(struct nk_context*);

#define nk_tree_element_push(ctx, type, title, state, sel) nk_tree_element_push_hashed(ctx, type, title, state, sel, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),__LINE__)
#define nk_tree_element_push_id(ctx, type, title, state, sel, id) nk_tree_element_push_hashed(ctx, type, title, state, sel, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),id)
NK_API nk_bool nk_tree_element_push_hashed(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states initial_state, nk_bool *selected, const char *hash, int len, int seed);
NK_API nk_bool nk_tree_element_image_push_hashed(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states initial_state, nk_bool *selected, const char *hash, int len,int seed);
NK_API void nk_tree_element_pop(struct nk_context*);

/* =============================================================================
 *
 *                                  PROPERTIES
 *
 * =============================================================================
/// ### Properties
/// Properties are the main value modification widgets in Nuklear. Changing a value
/// can be achieved by dragging, adding/removing incremental steps on button click
/// or by directly typing a number.
///
/// #### Usage
/// Each property requires a unique name for identification that is also used for
/// displaying a label. If you want to use the same name multiple times make sure
/// add a '#' before your name. The '#' will not be shown but will generate a
/// unique ID. Each property also takes in a minimum and maximum value. If you want
/// to make use of the complete number range of a type just use the provided
/// type limits from `limits.h`. For example `INT_MIN` and `INT_MAX` for
/// `nk_property_int` and `nk_propertyi`. In additional each property takes in
/// a increment value that will be added or subtracted if either the increment
/// decrement button is clicked. Finally there is a value for increment per pixel
/// dragged that is added or subtracted from the value.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// int value = 0;
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     // Input
///     Event evt;
///     nk_input_begin(&ctx);
///     while (GetEvent(&evt)) {
///         if (evt.type == MOUSE_MOVE)
///             nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///         else if (evt.type == [...]) {
///             nk_input_xxx(...);
///         }
///     }
///     nk_input_end(&ctx);
///     //
///     // Window
///     if (nk_begin_xxx(...) {
///         // Property
///         nk_layout_row_dynamic(...);
///         nk_property_int(ctx, "ID", INT_MIN, &value, INT_MAX, 1, 1);
///     }
///     nk_end(ctx);
///     //
///     // Draw
///     const struct nk_command *cmd = 0;
///     nk_foreach(cmd, &ctx) {
///     switch (cmd->type) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case ...:
///         // [...]
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function            | Description
/// --------------------|-------------------------------------------
/// nk_property_int     | Integer property directly modifying a passed in value
/// nk_property_float   | Float property directly modifying a passed in value
/// nk_property_double  | Double property directly modifying a passed in value
/// nk_propertyi        | Integer property returning the modified int value
/// nk_propertyf        | Float property returning the modified float value
/// nk_propertyd        | Double property returning the modified double value
///
*/
/*/// #### nk_property_int
/// Integer property directly modifying a passed in value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_property_int(struct nk_context *ctx, const char *name, int min, int *val, int max, int step, float inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Integer pointer to be modified
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
*/
NK_API void nk_property_int(struct nk_context*, const char *name, int min, int *val, int max, int step, float inc_per_pixel);
/*/// #### nk_property_float
/// Float property directly modifying a passed in value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_property_float(struct nk_context *ctx, const char *name, float min, float *val, float max, float step, float inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Float pointer to be modified
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
*/
NK_API void nk_property_float(struct nk_context*, const char *name, float min, float *val, float max, float step, float inc_per_pixel);
/*/// #### nk_property_double
/// Double property directly modifying a passed in value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_property_double(struct nk_context *ctx, const char *name, double min, double *val, double max, double step, double inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Double pointer to be modified
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
*/
NK_API void nk_property_double(struct nk_context*, const char *name, double min, double *val, double max, double step, float inc_per_pixel);
/*/// #### nk_propertyi
/// Integer property modifying a passed in value and returning the new value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// int nk_propertyi(struct nk_context *ctx, const char *name, int min, int val, int max, int step, float inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Current integer value to be modified and returned
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
///
/// Returns the new modified integer value
*/
NK_API int nk_propertyi(struct nk_context*, const char *name, int min, int val, int max, int step, float inc_per_pixel);
/*/// #### nk_propertyf
/// Float property modifying a passed in value and returning the new value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// float nk_propertyf(struct nk_context *ctx, const char *name, float min, float val, float max, float step, float inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Current float value to be modified and returned
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
///
/// Returns the new modified float value
*/
NK_API float nk_propertyf(struct nk_context*, const char *name, float min, float val, float max, float step, float inc_per_pixel);
/*/// #### nk_propertyd
/// Float property modifying a passed in value and returning the new value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// float nk_propertyd(struct nk_context *ctx, const char *name, double min, double val, double max, double step, double inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Current double value to be modified and returned
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
///
/// Returns the new modified double value
*/
NK_API double nk_propertyd(struct nk_context*, const char *name, double min, double val, double max, double step, float inc_per_pixel);

/* ===============================================================
 *
 *                          FONT
 *
 * ===============================================================*/
/*  Font handling in this library was designed to be quite customizable and lets
    you decide what you want to use and what you want to provide. There are three
    different ways to use the font atlas. The first two will use your font
    handling scheme and only requires essential data to run nuklear. The next
    slightly more advanced features is font handling with vertex buffer output.
    Finally the most complex API wise is using nuklear's font baking API.

    1.) Using your own implementation without vertex buffer output
    --------------------------------------------------------------
    So first up the easiest way to do font handling is by just providing a
    `nk_user_font` struct which only requires the height in pixel of the used
    font and a callback to calculate the width of a string. This way of handling
    fonts is best fitted for using the normal draw shape command API where you
    do all the text drawing yourself and the library does not require any kind
    of deeper knowledge about which font handling mechanism you use.
    IMPORTANT: the `nk_user_font` pointer provided to nuklear has to persist
    over the complete life time! I know this sucks but it is currently the only
    way to switch between fonts.

        float your_text_width_calculation(nk_handle handle, float height, const char *text, int len)
        {
            your_font_type *type = handle.ptr;
            float text_width = ...;
            return text_width;
        }

        struct nk_user_font font;
        font.userdata.ptr = &your_font_class_or_struct;
        font.height = your_font_height;
        font.width = your_text_width_calculation;

        struct nk_context ctx;
        nk_init_default(&ctx, &font);

    2.) Using your own implementation with vertex buffer output
    --------------------------------------------------------------
    While the first approach works fine if you don't want to use the optional
    vertex buffer output it is not enough if you do. To get font handling working
    for these cases you have to provide two additional parameters inside the
    `nk_user_font`. First a texture atlas handle used to draw text as subimages
    of a bigger font atlas texture and a callback to query a character's glyph
    information (offset, size, ...). So it is still possible to provide your own
    font and use the vertex buffer output.

        float your_text_width_calculation(nk_handle handle, float height, const char *text, int len)
        {
            your_font_type *type = handle.ptr;
            float text_width = ...;
            return text_width;
        }
        void query_your_font_glyph(nk_handle handle, float font_height, struct nk_user_font_glyph *glyph, nk_rune codepoint, nk_rune next_codepoint)
        {
            your_font_type *type = handle.ptr;
            glyph.width = ...;
            glyph.height = ...;
            glyph.xadvance = ...;
            glyph.uv[0].x = ...;
            glyph.uv[0].y = ...;
            glyph.uv[1].x = ...;
            glyph.uv[1].y = ...;
            glyph.offset.x = ...;
            glyph.offset.y = ...;
        }

        struct nk_user_font font;
        font.userdata.ptr = &your_font_class_or_struct;
        font.height = your_font_height;
        font.width = your_text_width_calculation;
        font.query = query_your_font_glyph;
        font.texture.id = your_font_texture;

        struct nk_context ctx;
        nk_init_default(&ctx, &font);

    3.) Nuklear font baker
    ------------------------------------
    The final approach if you do not have a font handling functionality or don't
    want to use it in this library is by using the optional font baker.
    The font baker APIs can be used to create a font plus font atlas texture
    and can be used with or without the vertex buffer output.

    It still uses the `nk_user_font` struct and the two different approaches
    previously stated still work. The font baker is not located inside
    `nk_context` like all other systems since it can be understood as more of
    an extension to nuklear and does not really depend on any `nk_context` state.

    Font baker need to be initialized first by one of the nk_font_atlas_init_xxx
    functions. If you don't care about memory just call the default version
    `nk_font_atlas_init_default` which will allocate all memory from the standard library.
    If you want to control memory allocation but you don't care if the allocated
    memory is temporary and therefore can be freed directly after the baking process
    is over or permanent you can call `nk_font_atlas_init`.

    After successfully initializing the font baker you can add Truetype(.ttf) fonts from
    different sources like memory or from file by calling one of the `nk_font_atlas_add_xxx`.
    functions. Adding font will permanently store each font, font config and ttf memory block(!)
    inside the font atlas and allows to reuse the font atlas. If you don't want to reuse
    the font baker by for example adding additional fonts you can call
    `nk_font_atlas_cleanup` after the baking process is over (after calling nk_font_atlas_end).

    As soon as you added all fonts you wanted you can now start the baking process
    for every selected glyph to image by calling `nk_font_atlas_bake`.
    The baking process returns image memory, width and height which can be used to
    either create your own image object or upload it to any graphics library.
    No matter which case you finally have to call `nk_font_atlas_end` which
    will free all temporary memory including the font atlas image so make sure
    you created our texture beforehand. `nk_font_atlas_end` requires a handle
    to your font texture or object and optionally fills a `struct nk_draw_null_texture`
    which can be used for the optional vertex output. If you don't want it just
    set the argument to `NULL`.

    At this point you are done and if you don't want to reuse the font atlas you
    can call `nk_font_atlas_cleanup` to free all truetype blobs and configuration
    memory. Finally if you don't use the font atlas and any of it's fonts anymore
    you need to call `nk_font_atlas_clear` to free all memory still being used.

        struct nk_font_atlas atlas;
        nk_font_atlas_init_default(&atlas);
        nk_font_atlas_begin(&atlas);
        nk_font *font = nk_font_atlas_add_from_file(&atlas, "Path/To/Your/TTF_Font.ttf", 13, 0);
        nk_font *font2 = nk_font_atlas_add_from_file(&atlas, "Path/To/Your/TTF_Font2.ttf", 16, 0);
        const void* img = nk_font_atlas_bake(&atlas, &img_width, &img_height, NK_FONT_ATLAS_RGBA32);
        nk_font_atlas_end(&atlas, nk_handle_id(texture), 0);

        struct nk_context ctx;
        nk_init_default(&ctx, &font->handle);
        while (1) {

        }
        nk_font_atlas_clear(&atlas);

    The font baker API is probably the most complex API inside this library and
    I would suggest reading some of my examples `example/` to get a grip on how
    to use the font atlas. There are a number of details I left out. For example
    how to merge fonts, configure a font with `nk_font_config` to use other languages,
    use another texture coordinate format and a lot more:

        struct nk_font_config cfg = nk_font_config(font_pixel_height);
        cfg.merge_mode = nk_false or nk_true;
        cfg.range = nk_font_korean_glyph_ranges();
        cfg.coord_type = NK_COORD_PIXEL;
        nk_font *font = nk_font_atlas_add_from_file(&atlas, "Path/To/Your/TTF_Font.ttf", 13, &cfg);

*/
struct nk_user_font_glyph;
typedef float(*nk_text_width_f)(nk_handle, float h, const char*, int len);
typedef void(*nk_query_font_glyph_f)(nk_handle handle, float font_height,
                                    struct nk_user_font_glyph *glyph,
                                    nk_rune codepoint, nk_rune next_codepoint);

#if defined(NK_INCLUDE_VERTEX_BUFFER_OUTPUT) || defined(NK_INCLUDE_SOFTWARE_FONT)
struct nk_user_font_glyph {
    struct nk_vec2 uv[2];
    /* texture coordinates */
    struct nk_vec2 offset;
    /* offset between top left and glyph */
    float width, height;
    /* size of the glyph  */
    float xadvance;
    /* offset to the next glyph */
};
#endif

struct nk_user_font {
    nk_handle userdata;
    /* user provided font handle */
    float height;
    /* max height of the font */
    nk_text_width_f width;
    /* font string width in pixel callback */
#ifdef NK_INCLUDE_VERTEX_BUFFER_OUTPUT
    nk_query_font_glyph_f query;
    /* font glyph callback to query drawing info */
    nk_handle texture;
    /* texture handle to the used font atlas or texture */
#endif
};

#ifdef NK_INCLUDE_FONT_BAKING
enum nk_font_coord_type {
    NK_COORD_UV, /* texture coordinates inside font glyphs are clamped between 0-1 */
    NK_COORD_PIXEL /* texture coordinates inside font glyphs are in absolute pixel */
};

struct nk_font;
struct nk_baked_font {
    float height;
    /* height of the font  */
    float ascent, descent;
    /* font glyphs ascent and descent  */
    nk_rune glyph_offset;
    /* glyph array offset inside the font glyph baking output array  */
    nk_rune glyph_count;
    /* number of glyphs of this font inside the glyph baking array output */
    const nk_rune *ranges;
    /* font codepoint ranges as pairs of (from/to) and 0 as last element */
};

struct nk_font_config {
    struct nk_font_config *next;
    /* NOTE: only used internally */
    void *ttf_blob;
    /* pointer to loaded TTF file memory block.
     * NOTE: not needed for nk_font_atlas_add_from_memory and nk_font_atlas_add_from_file. */
    nk_size ttf_size;
    /* size of the loaded TTF file memory block
     * NOTE: not needed for nk_font_atlas_add_from_memory and nk_font_atlas_add_from_file. */

    unsigned char ttf_data_owned_by_atlas;
    /* used inside font atlas: default to: 0*/
    unsigned char merge_mode;
    /* merges this font into the last font */
    unsigned char pixel_snap;
    /* align every character to pixel boundary (if true set oversample (1,1)) */
    unsigned char oversample_v, oversample_h;
    /* rasterize at high quality for sub-pixel position */
    unsigned char padding[3];

    float size;
    /* baked pixel height of the font */
    enum nk_font_coord_type coord_type;
    /* texture coordinate format with either pixel or UV coordinates */
    struct nk_vec2 spacing;
    /* extra pixel spacing between glyphs  */
    const nk_rune *range;
    /* list of unicode ranges (2 values per range, zero terminated) */
    struct nk_baked_font *font;
    /* font to setup in the baking process: NOTE: not needed for font atlas */
    nk_rune fallback_glyph;
    /* fallback glyph to use if a given rune is not found */
    struct nk_font_config *n;
    struct nk_font_config *p;
};

struct nk_font_glyph {
    nk_rune codepoint;
    float xadvance;
    float x0, y0, x1, y1, w, h;
    float u0, v0, u1, v1;
};

struct nk_font {
    struct nk_font *next;
    struct nk_user_font handle;
    struct nk_baked_font info;
    float scale;
    struct nk_font_glyph *glyphs;
    const struct nk_font_glyph *fallback;
    nk_rune fallback_codepoint;
    nk_handle texture;
    struct nk_font_config *config;
};

enum nk_font_atlas_format {
    NK_FONT_ATLAS_ALPHA8,
    NK_FONT_ATLAS_RGBA32
};

struct nk_font_atlas {
    void *pixel;
    int tex_width;
    int tex_height;

    struct nk_allocator permanent;
    struct nk_allocator temporary;

    struct nk_recti custom;
    struct nk_cursor cursors[NK_CURSOR_COUNT];

    int glyph_count;
    struct nk_font_glyph *glyphs;
    struct nk_font *default_font;
    struct nk_font *fonts;
    struct nk_font_config *config;
    int font_num;
};

/* some language glyph codepoint ranges */
NK_API const nk_rune *nk_font_default_glyph_ranges(void);
NK_API const nk_rune *nk_font_chinese_glyph_ranges(void);
NK_API const nk_rune *nk_font_cyrillic_glyph_ranges(void);
NK_API const nk_rune *nk_font_korean_glyph_ranges(void);

#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
NK_API void nk_font_atlas_init_default(struct nk_font_atlas*);
#endif
NK_API void nk_font_atlas_init(struct nk_font_atlas*, struct nk_allocator*);
NK_API void nk_font_atlas_init_custom(struct nk_font_atlas*, struct nk_allocator *persistent, struct nk_allocator *transient);
NK_API void nk_font_atlas_begin(struct nk_font_atlas*);
NK_API struct nk_font_config nk_font_config(float pixel_height);
NK_API struct nk_font *nk_font_atlas_add(struct nk_font_atlas*, const struct nk_font_config*);
#ifdef NK_INCLUDE_DEFAULT_FONT
NK_API struct nk_font* nk_font_atlas_add_default(struct nk_font_atlas*, float height, const struct nk_font_config*);
#endif
NK_API struct nk_font* nk_font_atlas_add_from_memory(struct nk_font_atlas *atlas, void *memory, nk_size size, float height, const struct nk_font_config *config);
#ifdef NK_INCLUDE_STANDARD_IO
NK_API struct nk_font* nk_font_atlas_add_from_file(struct nk_font_atlas *atlas, const char *file_path, float height, const struct nk_font_config*);
#endif
NK_API struct nk_font *nk_font_atlas_add_compressed(struct nk_font_atlas*, void *memory, nk_size size, float height, const struct nk_font_config*);
NK_API struct nk_font* nk_font_atlas_add_compressed_base85(struct nk_font_atlas*, const char *data, float height, const struct nk_font_config *config);
NK_API const void* nk_font_atlas_bake(struct nk_font_atlas*, int *width, int *height, enum nk_font_atlas_format);
NK_API void nk_font_atlas_end(struct nk_font_atlas*, nk_handle tex, struct nk_draw_null_texture*);
NK_API const struct nk_font_glyph* nk_font_find_glyph(struct nk_font*, nk_rune unicode);
NK_API void nk_font_atlas_cleanup(struct nk_font_atlas *atlas);
NK_API void nk_font_atlas_clear(struct nk_font_atlas*);

#endif
/* ==============================================================
 *
 *                          MEMORY BUFFER
 *
 * ===============================================================*/
/*  A basic (double)-buffer with linear allocation and resetting as only
    freeing policy. The buffer's main purpose is to control all memory management
    inside the GUI toolkit and still leave memory control as much as possible in
    the hand of the user while also making sure the library is easy to use if
    not as much control is needed.
    In general all memory inside this library can be provided from the user in
    three different ways.

    The first way and the one providing most control is by just passing a fixed
    size memory block. In this case all control lies in the hand of the user
    since he can exactly control where the memory comes from and how much memory
    the library should consume. Of course using the fixed size API removes the
    ability to automatically resize a buffer if not enough memory is provided so
    you have to take over the resizing. While being a fixed sized buffer sounds
    quite limiting, it is very effective in this library since the actual memory
    consumption is quite stable and has a fixed upper bound for a lot of cases.

    If you don't want to think about how much memory the library should allocate
    at all time or have a very dynamic UI with unpredictable memory consumption
    habits but still want control over memory allocation you can use the dynamic
    allocator based API. The allocator consists of two callbacks for allocating
    and freeing memory and optional userdata so you can plugin your own allocator.

    The final and easiest way can be used by defining
    NK_INCLUDE_DEFAULT_ALLOCATOR which uses the standard library memory
    allocation functions malloc and free and takes over complete control over
    memory in this library.
*/

/*===============================================================
 *
 *                      TEXT EDITOR
 *
 * ===============================================================*/
/* Editing text in this library is handled by either `nk_edit_string` or
 * `nk_edit_buffer`. But like almost everything in this library there are multiple
 * ways of doing it and a balance between control and ease of use with memory
 * as well as functionality controlled by flags.
 *
 * This library generally allows three different levels of memory control:
 * First of is the most basic way of just providing a simple char array with
 * string length. This method is probably the easiest way of handling simple
 * user text input. Main upside is complete control over memory while the biggest
 * downside in comparison with the other two approaches is missing undo/redo.
 *
 * For UIs that require undo/redo the second way was created. It is based on
 * a fixed size nk_text_edit struct, which has an internal undo/redo stack.
 * This is mainly useful if you want something more like a text editor but don't want
 * to have a dynamically growing buffer.
 *
 * The final way is using a dynamically growing nk_text_edit struct, which
 * has both a default version if you don't care where memory comes from and an
 * allocator version if you do. While the text editor is quite powerful for its
 * complexity I would not recommend editing gigabytes of data with it.
 * It is rather designed for uses cases which make sense for a GUI library not for
 * an full blown text editor.
 */

/* ===============================================================
 *
 *                          DRAWING
 *
 * ===============================================================*/
/*  This library was designed to be render backend agnostic so it does
    not draw anything to screen. Instead all drawn shapes, widgets
    are made of, are buffered into memory and make up a command queue.
    Each frame therefore fills the command buffer with draw commands
    that then need to be executed by the user and his own render backend.
    After that the command buffer needs to be cleared and a new frame can be
    started. It is probably important to note that the command buffer is the main
    drawing API and the optional vertex buffer API only takes this format and
    converts it into a hardware accessible format.

    To use the command queue to draw your own widgets you can access the
    command buffer of each window by calling `nk_window_get_canvas` after
    previously having called `nk_begin`:

        void draw_red_rectangle_widget(struct nk_context *ctx)
        {
            struct nk_command_buffer *canvas;
            struct nk_input *input = &ctx->input;
            canvas = nk_window_get_canvas(ctx);

            struct nk_rect space;
            enum nk_widget_layout_states state;
            state = nk_widget(&space, ctx);
            if (!state) return;

            if (state != NK_WIDGET_ROM)
                update_your_widget_by_user_input(...);
            nk_fill_rect(canvas, space, 0, nk_rgb(255,0,0));
        }

        if (nk_begin(...)) {
            nk_layout_row_dynamic(ctx, 25, 1);
            draw_red_rectangle_widget(ctx);
        }
        nk_end(..)

    Important to know if you want to create your own widgets is the `nk_widget`
    call. It allocates space on the panel reserved for this widget to be used,
    but also returns the state of the widget space. If your widget is not seen and does
    not have to be updated it is '0' and you can just return. If it only has
    to be drawn the state will be `NK_WIDGET_ROM` otherwise you can do both
    update and draw your widget. The reason for separating is to only draw and
    update what is actually necessary which is crucial for performance.
*/

/*==============================================================
 *                          STACK
 * =============================================================*/
/* The style modifier stack can be used to temporarily change a
 * property inside `nk_style`. For example if you want a special
 * red button you can temporarily push the old button color onto a stack
 * draw the button with a red color and then you just pop the old color
 * back from the stack:
 *
 *      nk_style_push_style_item(ctx, &ctx->style.button.normal, nk_style_item_color(nk_rgb(255,0,0)));
 *      nk_style_push_style_item(ctx, &ctx->style.button.hover, nk_style_item_color(nk_rgb(255,0,0)));
 *      nk_style_push_style_item(ctx, &ctx->style.button.active, nk_style_item_color(nk_rgb(255,0,0)));
 *      nk_style_push_vec2(ctx, &cx->style.button.padding, nk_vec2(2,2));
 *
 *      nk_button(...);
 *
 *      nk_style_pop_style_item(ctx);
 *      nk_style_pop_style_item(ctx);
 *      nk_style_pop_style_item(ctx);
 *      nk_style_pop_vec2(ctx);
 *
 * Nuklear has a stack for style_items, float properties, vector properties,
 * flags, colors, fonts and for button_behavior. Each has it's own fixed size stack
 * which can be changed at compile time.
 */

/* ===============================================================
 *
 *                              MATH
 *
 * ===============================================================*/
/*  Since nuklear is supposed to work on all systems providing floating point
    math without any dependencies I also had to implement my own math functions
    for sqrt, sin and cos. Since the actual highly accurate implementations for
    the standard library functions are quite complex and I do not need high
    precision for my use cases I use approximations.

    Sqrt
    ----
    For square root nuklear uses the famous fast inverse square root:
    https://en.wikipedia.org/wiki/Fast_inverse_square_root with
    slightly tweaked magic constant. While on today's hardware it is
    probably not faster it is still fast and accurate enough for
    nuklear's use cases. IMPORTANT: this requires float format IEEE 754

    Sine/Cosine
    -----------
    All constants inside both function are generated Remez's minimax
    approximations for value range 0...2*PI. The reason why I decided to
    approximate exactly that range is that nuklear only needs sine and
    cosine to generate circles which only requires that exact range.
    In addition I used Remez instead of Taylor for additional precision:
    www.lolengine.net/blog/2011/12/21/better-function-approximations.

    The tool I used to generate constants for both sine and cosine
    (it can actually approximate a lot more functions) can be
    found here: www.lolengine.net/wiki/oss/lolremez
*/


/*  stb_truetype.h - v1.26 - public domain */
/*  authored from 2009-2021 by Sean Barrett / RAD Game Tools */
/*  */
/*  ======================================================================= */
/*  */
/*     NO SECURITY GUARANTEE -- DO NOT USE THIS ON UNTRUSTED FONT FILES */
/*  */
/*  This library does no range checking of the offsets found in the file, */
/*  meaning an attacker can use it to read arbitrary memory. */
/*  */
/*  ======================================================================= */
/*  */
/*    This library processes TrueType files: */
/*         parse files */
/*         extract glyph metrics */
/*         extract glyph shapes */
/*         render glyphs to one-channel bitmaps with antialiasing (box filter) */
/*         render glyphs to one-channel SDF bitmaps (signed-distance field/function) */
/*  */
/*    Todo: */
/*         non-MS cmaps */
/*         crashproof on bad data */
/*         hinting? (no longer patented) */
/*         cleartype-style AA? */
/*         optimize: use simple memory allocator for intermediates */
/*         optimize: build edge-list directly from curves */
/*         optimize: rasterize directly from curves? */
/*  */
/*  ADDITIONAL CONTRIBUTORS */
/*  */
/*    Mikko Mononen: compound shape support, more cmap formats */
/*    Tor Andersson: kerning, subpixel rendering */
/*    Dougall Johnson: OpenType / Type 2 font handling */
/*    Daniel Ribeiro Maciel: basic GPOS-based kerning */
/*  */
/*    Misc other: */
/*        Ryan Gordon */
/*        Simon Glass */
/*        github:IntellectualKitty */
/*        Imanol Celaya */
/*        Daniel Ribeiro Maciel */
/*  */
/*    Bug/warning reports/fixes: */
/*        "Zer" on mollyrocket       Fabian "ryg" Giesen   github:NiLuJe */
/*        Cass Everitt               Martins Mozeiko       github:aloucks */
/*        stoiko (Haemimont Games)   Cap Petschulat        github:oyvindjam */
/*        Brian Hook                 Omar Cornut           github:vassvik */
/*        Walter van Niftrik         Ryan Griege */
/*        David Gow                  Peter LaValle */
/*        David Given                Sergey Popov */
/*        Ivan-Assen Ivanov          Giumo X. Clanjor */
/*        Anthony Pesch              Higor Euripedes */
/*        Johan Duparc               Thomas Fields */
/*        Hou Qiming                 Derek Vinyard */
/*        Rob Loach                  Cort Stratton */
/*        Kenney Phillis Jr.         Brian Costabile */
/*        Ken Voskuil (kaesve) */
/*  */
/*  VERSION HISTORY */
/*  */
/*    1.26 (2021-08-28) fix broken rasterizer */
/*    1.25 (2021-07-11) many fixes */
/*    1.24 (2020-02-05) fix warning */
/*    1.23 (2020-02-02) query SVG data for glyphs; query whole kerning table (but only kern not GPOS) */
/*    1.22 (2019-08-11) minimize missing-glyph duplication; fix kerning if both 'GPOS' and 'kern' are defined */
/*    1.21 (2019-02-25) fix warning */
/*    1.20 (2019-02-07) PackFontRange skips missing codepoints; GetScaleFontVMetrics() */
/*    1.19 (2018-02-11) GPOS kerning, STBTT_fmod */
/*    1.18 (2018-01-29) add missing function */
/*    1.17 (2017-07-23) make more arguments const; doc fix */
/*    1.16 (2017-07-12) SDF support */
/*    1.15 (2017-03-03) make more arguments const */
/*    1.14 (2017-01-16) num-fonts-in-TTC function */
/*    1.13 (2017-01-02) support OpenType fonts, certain Apple fonts */
/*    1.12 (2016-10-25) suppress warnings about casting away const with -Wcast-qual */
/*    1.11 (2016-04-02) fix unused-variable warning */
/*    1.10 (2016-04-02) user-defined fabs(); rare memory leak; remove duplicate typedef */
/*    1.09 (2016-01-16) warning fix; avoid crash on outofmem; use allocation userdata properly */
/*    1.08 (2015-09-13) document stbtt_Rasterize(); fixes for vertical & horizontal edges */
/*    1.07 (2015-08-01) allow PackFontRanges to accept arrays of sparse codepoints; */
/*                      variant PackFontRanges to pack and render in separate phases; */
/*                      fix stbtt_GetFontOFfsetForIndex (never worked for non-0 input?); */
/*                      fixed an assert() bug in the new rasterizer */
/*                      replace assert() with STBTT_assert() in new rasterizer */
/*  */
/*    Full history can be found at the end of this file. */
/*  */
/*  LICENSE */
/*  */
/*    See end of file for license information. */
/*  */
/*  USAGE */
/*  */
/*    Include this file in whatever places need to refer to it. In ONE C/C++ */
/*    file, write: */
/*       #define STB_TRUETYPE_IMPLEMENTATION */
/*    before the #include of this file. This expands out the actual */
/*    implementation into that C/C++ file. */
/*  */
/*    To make the implementation private to the file that generates the implementation, */
/*       #define STBTT_STATIC */
/*  */
/*    Simple 3D API (don't ship this, but it's fine for tools and quick start) */
/*            stbtt_BakeFontBitmap()               -- bake a font to a bitmap for use as texture */
/*            stbtt_GetBakedQuad()                 -- compute quad to draw for a given char */
/*  */
/*    Improved 3D API (more shippable): */
/*            #include "stb_rect_pack.h"           -- optional, but you really want it */
/*            stbtt_PackBegin() */
/*            stbtt_PackSetOversampling()          -- for improved quality on small fonts */
/*            stbtt_PackFontRanges()               -- pack and renders */
/*            stbtt_PackEnd() */
/*            stbtt_GetPackedQuad() */
/*  */
/*    "Load" a font file from a memory buffer (you have to keep the buffer loaded) */
/*            stbtt_InitFont() */
/*            stbtt_GetFontOffsetForIndex()        -- indexing for TTC font collections */
/*            stbtt_GetNumberOfFonts()             -- number of fonts for TTC font collections */
/*  */
/*    Render a unicode codepoint to a bitmap */
/*            stbtt_GetCodepointBitmap()           -- allocates and returns a bitmap */
/*            stbtt_MakeCodepointBitmap()          -- renders into bitmap you provide */
/*            stbtt_GetCodepointBitmapBox()        -- how big the bitmap must be */
/*  */
/*    Character advance/positioning */
/*            stbtt_GetCodepointHMetrics() */
/*            stbtt_GetFontVMetrics() */
/*            stbtt_GetFontVMetricsOS2() */
/*            stbtt_GetCodepointKernAdvance() */
/*  */
/*    Starting with version 1.06, the rasterizer was replaced with a new, */
/*    faster and generally-more-precise rasterizer. The new rasterizer more */
/*    accurately measures pixel coverage for anti-aliasing, except in the case */
/*    where multiple shapes overlap, in which case it overestimates the AA pixel */
/*    coverage. Thus, anti-aliasing of intersecting shapes may look wrong. If */
/*    this turns out to be a problem, you can re-enable the old rasterizer with */
/*         #define STBTT_RASTERIZER_VERSION 1 */
/*    which will incur about a 15% speed hit. */
/*  */
/*  ADDITIONAL DOCUMENTATION */
/*  */
/*    Immediately after this block comment are a series of sample programs. */
/*  */
/*    After the sample programs is the "header file" section. This section */
/*    includes documentation for each API function. */
/*  */
/*    Some important concepts to understand to use this library: */
/*  */
/*       Codepoint */
/*          Characters are defined by unicode codepoints, e.g. 65 is */
/*          uppercase A, 231 is lowercase c with a cedilla, 0x7e30 is */
/*          the hiragana for "ma". */
/*  */
/*       Glyph */
/*          A visual character shape (every codepoint is rendered as */
/*          some glyph) */
/*  */
/*       Glyph index */
/*          A font-specific integer ID representing a glyph */
/*  */
/*       Baseline */
/*          Glyph shapes are defined relative to a baseline, which is the */
/*          bottom of uppercase characters. Characters extend both above */
/*          and below the baseline. */
/*  */
/*       Current Point */
/*          As you draw text to the screen, you keep track of a "current point" */
/*          which is the origin of each character. The current point's vertical */
/*          position is the baseline. Even "baked fonts" use this model. */
/*  */
/*       Vertical Font Metrics */
/*          The vertical qualities of the font, used to vertically position */
/*          and space the characters. See docs for stbtt_GetFontVMetrics. */
/*  */
/*       Font Size in Pixels or Points */
/*          The preferred interface for specifying font sizes in stb_truetype */
/*          is to specify how tall the font's vertical extent should be in pixels. */
/*          If that sounds good enough, skip the next paragraph. */
/*  */
/*          Most font APIs instead use "points", which are a common typographic */
/*          measurement for describing font size, defined as 72 points per inch. */
/*          stb_truetype provides a point API for compatibility. However, true */
/*          "per inch" conventions don't make much sense on computer displays */
/*          since different monitors have different number of pixels per */
/*          inch. For example, Windows traditionally uses a convention that */
/*          there are 96 pixels per inch, thus making 'inch' measurements have */
/*          nothing to do with inches, and thus effectively defining a point to */
/*          be 1.333 pixels. Additionally, the TrueType font data provides */
/*          an explicit scale factor to scale a given font's glyphs to points, */
/*          but the author has observed that this scale factor is often wrong */
/*          for non-commercial fonts, thus making fonts scaled in points */
/*          according to the TrueType spec incoherently sized in practice. */
/*  */
/*  DETAILED USAGE: */
/*  */
/*   Scale: */
/*     Select how high you want the font to be, in points or pixels. */
/*     Call ScaleForPixelHeight or ScaleForMappingEmToPixels to compute */
/*     a scale factor SF that will be used by all other functions. */
/*  */
/*   Baseline: */
/*     You need to select a y-coordinate that is the baseline of where */
/*     your text will appear. Call GetFontBoundingBox to get the baseline-relative */
/*     bounding box for all characters. SF*-y0 will be the distance in pixels */
/*     that the worst-case character could extend above the baseline, so if */
/*     you want the top edge of characters to appear at the top of the */
/*     screen where y=0, then you would set the baseline to SF*-y0. */
/*  */
/*   Current point: */
/*     Set the current point where the first character will appear. The */
/*     first character could extend left of the current point; this is font */
/*     dependent. You can either choose a current point that is the leftmost */
/*     point and hope, or add some padding, or check the bounding box or */
/*     left-side-bearing of the first character to be displayed and set */
/*     the current point based on that. */
/*  */
/*   Displaying a character: */
/*     Compute the bounding box of the character. It will contain signed values */
/*     relative to <current_point, baseline>. I.e. if it returns x0,y0,x1,y1, */
/*     then the character should be displayed in the rectangle from */
/*     <current_point+SF*x0, baseline+SF*y0> to <current_point+SF*x1,baseline+SF*y1). */
/*  */
/*   Advancing for the next character: */
/*     Call GlyphHMetrics, and compute 'current_point += SF * advance'. */
/*  */
/*  */
/*  ADVANCED USAGE */
/*  */
/*    Quality: */
/*  */
/*     - Use the functions with Subpixel at the end to allow your characters */
/*       to have subpixel positioning. Since the font is anti-aliased, not */
/*       hinted, this is very import for quality. (This is not possible with */
/*       baked fonts.) */
/*  */
/*     - Kerning is now supported, and if you're supporting subpixel rendering */
/*       then kerning is worth using to give your text a polished look. */
/*  */
/*    Performance: */
/*  */
/*     - Convert Unicode codepoints to glyph indexes and operate on the glyphs; */
/*       if you don't do this, stb_truetype is forced to do the conversion on */
/*       every call. */
/*  */
/*     - There are a lot of memory allocations. We should modify it to take */
/*       a temp buffer and allocate from the temp buffer (without freeing), */
/*       should help performance a lot. */
/*  */
/*  NOTES */
/*  */
/*    The system uses the raw data found in the .ttf file without changing it */
/*    and without building auxiliary data structures. This is a bit inefficient */
/*    on little-endian systems (the data is big-endian), but assuming you're */
/*    caching the bitmaps or glyph shapes this shouldn't be a big deal. */
/*  */
/*    It appears to be very hard to programmatically determine what font a */
/*    given file is in a general way. I provide an API for this, but I don't */
/*    recommend it. */
/*  */
/*  */
/*  PERFORMANCE MEASUREMENTS FOR 1.06: */
/*  */
/*                       32-bit     64-bit */
/*    Previous release:  8.83 s     7.68 s */
/*    Pool allocations:  7.72 s     6.34 s */
/*    Inline sort     :  6.54 s     5.65 s */
/*    New rasterizer  :  5.63 s     5.00 s */

/* //////////////////////////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////////////////////////// */
/* // */
/* //  SAMPLE PROGRAMS */
/* // */
/*  */
/*   Incomplete text-in-3d-api example, which draws quads properly aligned to be lossless. */
/*   See "tests/truetype_demo_win32.c" for a complete version. */
#if 0
#define STB_TRUETYPE_IMPLEMENTATION  /*  force following include to generate implementation */
#include "stb_truetype.h"

unsigned char ttf_buffer[1<<20];
unsigned char temp_bitmap[512*512];

stbtt_bakedchar cdata[96]; /*  ASCII 32..126 is 95 glyphs */
GLuint ftex;

void my_stbtt_initfont(void)
{
   fread(ttf_buffer, 1, 1<<20, fopen("c:/windows/fonts/times.ttf", "rb"));
   stbtt_BakeFontBitmap(ttf_buffer,0, 32.0, temp_bitmap,512,512, 32,96, cdata); /*  no guarantee this fits! */
   /*  can free ttf_buffer at this point */
   glGenTextures(1, &ftex);
   glBindTexture(GL_TEXTURE_2D, ftex);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
   /*  can free temp_bitmap at this point */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void my_stbtt_print(float x, float y, char *text)
{
   /*  assume orthographic projection with units = screen pixels, origin at top left */
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, ftex);
   glBegin(GL_QUADS);
   while (*text) {
      if (*text >= 32 && *text < 128) {
         stbtt_aligned_quad q;
         stbtt_GetBakedQuad(cdata, 512,512, *text-32, &x,&y,&q,1);/* 1=opengl & d3d10+,0=d3d9 */
         glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
         glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
         glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
         glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
      }
      ++text;
   }
   glEnd();
}
#endif
/*  */
/*  */
/* //////////////////////////////////////////////////////////////////////////// */
/*  */
/*  Complete program (this compiles): get a single bitmap, print as ASCII art */
/*  */
#if 0
#include <stdio.h>
#define STB_TRUETYPE_IMPLEMENTATION  /*  force following include to generate implementation */
#include "stb_truetype.h"

char ttf_buffer[1<<25];

int main(int argc, char **argv)
{
   stbtt_fontinfo font;
   unsigned char *bitmap;
   int w,h,i,j,c = (argc > 1 ? atoi(argv[1]) : 'a'), s = (argc > 2 ? atoi(argv[2]) : 20);

   fread(ttf_buffer, 1, 1<<25, fopen(argc > 3 ? argv[3] : "c:/windows/fonts/arialbd.ttf", "rb"));

   stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer,0));
   bitmap = stbtt_GetCodepointBitmap(&font, 0,stbtt_ScaleForPixelHeight(&font, s), c, &w, &h, 0,0);

   for (j=0; j < h; ++j) {
      for (i=0; i < w; ++i)
         putchar(" .:ioVM@"[bitmap[j*w+i]>>5]);
      putchar('\n');
   }
   return 0;
}
#endif
/*  */
/*  Output: */
/*  */
/*      .ii. */
/*     @@@@@@. */
/*    V@Mio@@o */
/*    :i.  V@V */
/*      :oM@@M */
/*    :@@@MM@M */
/*    @@o  o@M */
/*   :@@.  M@M */
/*    @@@o@@@@ */
/*    :M@@V:@@. */
/*  */
/* //////////////////////////////////////////////////////////////////////////// */
/*  */
/*  Complete program: print "Hello World!" banner, with bugs */
/*  */
#if 0
char buffer[24<<20];
unsigned char screen[20][79];

int main(int arg, char **argv)
{
   stbtt_fontinfo font;
   int i,j,ascent,baseline,ch=0;
   float scale, xpos=2; /*  leave a little padding in case the character extends left */
   char *text = "Heljo World!"; /*  intentionally misspelled to show 'lj' brokenness */

   fread(buffer, 1, 1000000, fopen("c:/windows/fonts/arialbd.ttf", "rb"));
   stbtt_InitFont(&font, buffer, 0);

   scale = stbtt_ScaleForPixelHeight(&font, 15);
   stbtt_GetFontVMetrics(&font, &ascent,0,0);
   baseline = (int) (ascent*scale);

   while (text[ch]) {
      int advance,lsb,x0,y0,x1,y1;
      float x_shift = xpos - (float) floor(xpos);
      stbtt_GetCodepointHMetrics(&font, text[ch], &advance, &lsb);
      stbtt_GetCodepointBitmapBoxSubpixel(&font, text[ch], scale,scale,x_shift,0, &x0,&y0,&x1,&y1);
      stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int) xpos + x0], x1-x0,y1-y0, 79, scale,scale,x_shift,0, text[ch]);
      /*  note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong */
      /*  because this API is really for baking character bitmaps into textures. if you want to render */
      /*  a sequence of characters, you really need to render each bitmap to a temp buffer, then */
      /*  "alpha blend" that into the working buffer */
      xpos += (advance * scale);
      if (text[ch+1])
         xpos += scale*stbtt_GetCodepointKernAdvance(&font, text[ch],text[ch+1]);
      ++ch;
   }

   for (j=0; j < 20; ++j) {
      for (i=0; i < 78; ++i)
         putchar(" .:ioVM@"[screen[j][i]>>5]);
      putchar('\n');
   }

   return 0;
}
#endif


/* //////////////////////////////////////////////////////////////////////////// */

/* //////////////////////////////////////////////////////////////////////////// */
/* // */
/* //   INTEGRATION WITH YOUR CODEBASE */
/* // */
/* //   The following sections allow you to supply alternate definitions */
/* //   of C library functions used by stb_truetype, e.g. if you don't */
/* //   link with the C runtime library. */


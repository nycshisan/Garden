//
//  Misc.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/29.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Misc_h
#define Misc_h

typedef float data_t;

#define ALWAYS_INLINE __attribute__((always_inline))

#define fatalError(MSG) do {\
    printf(MSG);\
    assert(0);\
} while(0)

// C++ standards restrain the unwinding of macro arguments used with `##`
#define CONCAT_LITERAL(LEFT, RIGHT) LEFT ## RIGHT
#define CONCAT(LEFT, RIGHT) CONCAT_LITERAL(LEFT, RIGHT)

/*
 * The Varying class definition helper macro is defined here.
 * Cuz C++ does not support a flexible reflection machansim and some kind of preprocess-time
 * meta-programing, and I do not want to introduce an external build system to do this,
 * I have to abuse the preprocessor to make the helper.
 * Some ideas could be referenced at https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms
 */

/*
 * When the macro `__PROBE` accepts some arguments, it will not pick the
 * `__PROBE_NIL` macro, and therefore will not generate the placeholders
 * for `__CHECK` macro.
 */
#define __PROBE_NIL PLACEHOLDER, SECOND
#define __PROBE(A, B) __PROBE_NIL ## A ## B

/*
 * If the `__PROBE_NIL` macro is not expanded, which means `A` and `B` are either
 * not nil, there will be no placeholders generated. The `FIRST` will be the
 * second parameter of the macro `__CHECK_INTERNAL` and be choosed.
 */
#define __CHECK_INTERNAL(PLACEHOLDER, CHOICE, ...) CHOICE
#define __CHECK(OPTION) __CHECK_INTERNAL(OPTION, FIRST)

// The macro `__SELECTOR` will pick one of these two macros to simulate a selector.
#define __SELECTOR_FIRST(FIRST, SECOND) FIRST
#define __SELECTOR_SECOND(FIRST, SECOND) SECOND
#define __SELECTOR_INTERNAL(A, B) CONCAT(__SELECTOR_, __CHECK(__PROBE(A, B)))

// Get the first arguments for checking.
// GCC allows a macro defined like `MACRO(A, ...)` accept zero arguments.
#define __PICK_FIRST(FIRST, ...) FIRST
#define __SELECTOR(A, ...) __SELECTOR_INTERNAL(A, __PICK_FIRST(__VA_ARGS__))


// Macros for Deferringly expanding to bypass the recurse suppression of the preprocessor
#define __EMPTY()
#define __DEFER(id) id __EMPTY()
#define __OBSTRUCT(id) id __DEFER(__EMPTY)()

// Macros for expanding the deferred macros.
#define __EVAL(...)  __EVAL_1(__EVAL_1(__EVAL_1(__VA_ARGS__)))
#define __EVAL_1(...) __EVAL_2(__EVAL_2(__EVAL_2(__VA_ARGS__)))
#define __EVAL_2(...) __EVAL_3(__EVAL_3(__EVAL_3(__VA_ARGS__)))
#define __EVAL_3(...) __EVAL_4(__EVAL_4(__EVAL_4(__VA_ARGS__)))
#define __EVAL_4(...) __EVAL_5(__EVAL_5(__EVAL_5(__VA_ARGS__)))
#define __EVAL_5(...) __VA_ARGS__


#define _VARYING_DATA_MEMBERS_DEF_HELPER() VARYING_DATA_MEMBERS_DEF_HELPER
#define VARYING_DATA_MEMBERS_DEF_HELPER(TYPE, NAME, ...) TYPE NAME; __SELECTOR(__VA_ARGS__)(__OBSTRUCT(_VARYING_DATA_MEMBERS_DEF_HELPER)()(__VA_ARGS__), __EMPTY())

#define _VARYING_ADD_MODIFIERS_DEF_HELPER() VARYING_ADD_MODIFIERS_DEF_HELPER
#define VARYING_ADD_MODIFIERS_DEF_HELPER(TYPE, NAME, ...) NAME += addend.NAME; __SELECTOR(__VA_ARGS__)(__OBSTRUCT(_VARYING_ADD_MODIFIERS_DEF_HELPER)()(__VA_ARGS__), __EMPTY())

#define _VARYING_SUB_MODIFIERS_DEF_HELPER() VARYING_SUB_MODIFIERS_DEF_HELPER
#define VARYING_SUB_MODIFIERS_DEF_HELPER(TYPE, NAME, ...) NAME -= subtrahend.NAME; __SELECTOR(__VA_ARGS__)(__OBSTRUCT(_VARYING_SUB_MODIFIERS_DEF_HELPER)()(__VA_ARGS__), __EMPTY())

#define _VARYING_MULTIPLY_MODIFIERS_DEF_HELPER() VARYING_MULTIPLY_MODIFIERS_DEF_HELPER
#define VARYING_MULTIPLY_MODIFIERS_DEF_HELPER(TYPE, NAME, ...) NAME *= factor; __SELECTOR(__VA_ARGS__)(__OBSTRUCT(_VARYING_MULTIPLY_MODIFIERS_DEF_HELPER)()(__VA_ARGS__), __EMPTY())

#define VARYING_DEF_HELPER(CLASS_NAME, ...) \
class CLASS_NAME { \
public: \
    __EVAL(VARYING_DATA_MEMBERS_DEF_HELPER(__VA_ARGS__)) \
    ALWAYS_INLINE void add(CLASS_NAME &addend) { \
        __EVAL(VARYING_ADD_MODIFIERS_DEF_HELPER(__VA_ARGS__)) \
    } \
    ALWAYS_INLINE void subtract(CLASS_NAME &subtrahend) { \
        __EVAL(VARYING_SUB_MODIFIERS_DEF_HELPER(__VA_ARGS__)) \
    } \
    ALWAYS_INLINE void multiply(data_t factor) { \
        __EVAL(VARYING_MULTIPLY_MODIFIERS_DEF_HELPER(__VA_ARGS__)) \
    } \
}

// The macro `VARYING_DEF_HELPER` must accept more than one auguments.
// Pipelines which do not use `Varying` could be specialized by the NullVarying class.
class NullVarying {
public:
    ALWAYS_INLINE void add(NullVarying &addend) {}
    ALWAYS_INLINE void subtract(NullVarying &subtrahend) {}
    ALWAYS_INLINE void multiply(data_t factor) {}
};


#endif /* Misc_h */

/**
 * Macros for metaprogramming
 * ExtendedC
 *
 * Copyright (C) 2012 Justin Spahr-Summers
 * Released under the MIT license
 */

#ifndef BJL_EXTC_METAMACROS_H
#define BJL_EXTC_METAMACROS_H


/**
 * Executes one or more expressions (which may have a void type, such as a call
 * to a function that returns no value) and always returns true.
 */
#define bjl_metamacro_exprify(...) \
    ((__VA_ARGS__), true)

/**
 * Returns a string representation of VALUE after full macro expansion.
 */
#define bjl_metamacro_stringify(VALUE) \
        bjl_metamacro_stringify_(VALUE)

/**
 * Returns A and B concatenated after full macro expansion.
 */
#define bjl_metamacro_concat(A, B) \
        bjl_metamacro_concat_(A, B)

/**
 * Returns the Nth variadic argument (starting from zero). At least
 * N + 1 variadic arguments must be given. N must be between zero and twenty,
 * inclusive.
 */
#define bjl_metamacro_at(N, ...) \
        bjl_metamacro_concat(bjl_metamacro_at, N)(__VA_ARGS__)

/**
 * Returns the number of arguments (up to twenty) provided to the macro. At
 * least one argument must be provided.
 *
 * Inspired by P99: http://p99.gforge.inria.fr
 */
#define bjl_metamacro_argcount(...) \
        bjl_metamacro_at(20, __VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

/**
 * Identical to #metamacro_foreach_cxt, except that no CONTEXT argument is
 * given. Only the index and current argument will thus be passed to MACRO.
 */
#define bjl_metamacro_foreach(MACRO, SEP, ...) \
        bjl_metamacro_foreach_cxt(bjl_metamacro_foreach_iter, SEP, MACRO, __VA_ARGS__)

/**
 * For each consecutive variadic argument (up to twenty), MACRO is passed the
 * zero-based index of the current argument, CONTEXT, and then the argument
 * itself. The results of adjoining invocations of MACRO are then separated by
 * SEP.
 *
 * Inspired by P99: http://p99.gforge.inria.fr
 */
#define bjl_metamacro_foreach_cxt(MACRO, SEP, CONTEXT, ...) \
        bjl_metamacro_concat(bjl_metamacro_foreach_cxt, bjl_metamacro_argcount(__VA_ARGS__))(MACRO, SEP, CONTEXT, __VA_ARGS__)

/**
 * Identical to #metamacro_foreach_cxt. This can be used when the former would
 * fail due to recursive macro expansion.
 */
#define bjl_metamacro_foreach_cxt_recursive(MACRO, SEP, CONTEXT, ...) \
        bjl_metamacro_concat(bjl_metamacro_foreach_cxt_recursive, bjl_metamacro_argcount(__VA_ARGS__))(MACRO, SEP, CONTEXT, __VA_ARGS__)

/**
 * In consecutive order, appends each variadic argument (up to twenty) onto
 * BASE. The resulting concatenations are then separated by SEP.
 *
 * This is primarily useful to manipulate a list of macro invocations into instead
 * invoking a different, possibly related macro.
 */
#define bjl_metamacro_foreach_concat(BASE, SEP, ...) \
        bjl_metamacro_foreach_cxt(bjl_metamacro_foreach_concat_iter, SEP, BASE, __VA_ARGS__)

/**
 * Iterates COUNT times, each time invoking MACRO with the current index
 * (starting at zero) and CONTEXT. The results of adjoining invocations of MACRO
 * are then separated by SEP.
 *
 * COUNT must be an integer between zero and twenty, inclusive.
 */
#define bjl_metamacro_for_cxt(COUNT, MACRO, SEP, CONTEXT) \
        bjl_metamacro_concat(bjl_metamacro_for_cxt, COUNT)(MACRO, SEP, CONTEXT)

/**
 * Returns the first argument given. At least one argument must be provided.
 *
 * This is useful when implementing a variadic macro, where you may have only
 * one variadic argument, but no way to retrieve it (for example, because \c ...
 * always needs to match at least one argument).
 *
 * @code

#define varmacro(...) \
    metamacro_head(__VA_ARGS__)

 * @endcode
 */
#define bjl_metamacro_head(...) \
        bjl_metamacro_head_(__VA_ARGS__, 0)

/**
 * Returns every argument except the first. At least two arguments must be
 * provided.
 */
#define bjl_metamacro_tail(...) \
        bjl_metamacro_tail_(__VA_ARGS__)

/**
 * Returns the first N (up to twenty) variadic arguments as a new argument list.
 * At least N variadic arguments must be provided.
 */
#define bjl_metamacro_take(N, ...) \
        bjl_metamacro_concat(bjl_metamacro_take, N)(__VA_ARGS__)

/**
 * Removes the first N (up to twenty) variadic arguments from the given argument
 * list. At least N variadic arguments must be provided.
 */
#define bjl_metamacro_drop(N, ...) \
        bjl_metamacro_concat(bjl_metamacro_drop, N)(__VA_ARGS__)

/**
 * Decrements VAL, which must be a number between zero and twenty, inclusive.
 *
 * This is primarily useful when dealing with indexes and counts in
 * metaprogramming.
 */
#define bjl_metamacro_dec(VAL) \
        bjl_metamacro_at(VAL, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19)

/**
 * Increments VAL, which must be a number between zero and twenty, inclusive.
 *
 * This is primarily useful when dealing with indexes and counts in
 * metaprogramming.
 */
#define bjl_metamacro_inc(VAL) \
        bjl_metamacro_at(VAL, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21)

/**
 * If A is equal to B, the next argument list is expanded; otherwise, the
 * argument list after that is expanded. A and B must be numbers between zero
 * and twenty, inclusive. Additionally, B must be greater than or equal to A.
 *
 * @code

// expands to true
metamacro_if_eq(0, 0)(true)(false)

// expands to false
metamacro_if_eq(0, 1)(true)(false)

 * @endcode
 *
 * This is primarily useful when dealing with indexes and counts in
 * metaprogramming.
 */
#define bjl_metamacro_if_eq(A, B) \
        bjl_metamacro_concat(bjl_metamacro_if_eq, A)(B)

/**
 * Identical to #metamacro_if_eq. This can be used when the former would fail
 * due to recursive macro expansion.
 */
#define bjl_metamacro_if_eq_recursive(A, B) \
        bjl_metamacro_concat(bjl_metamacro_if_eq_recursive, A)(B)

/**
 * Returns 1 if N is an even number, or 0 otherwise. N must be between zero and
 * twenty, inclusive.
 *
 * For the purposes of this test, zero is considered even.
 */
#define bjl_metamacro_is_even(N) \
        bjl_metamacro_at(N, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1)

/**
 * Returns the logical NOT of B, which must be the number zero or one.
 */
#define bjl_metamacro_not(B) \
        bjl_metamacro_at(B, 1, 0)

// IMPLEMENTATION DETAILS FOLLOW!
// Do not write code that depends on anything below this line.
#define bjl_metamacro_stringify_(VALUE) # VALUE
#define bjl_metamacro_concat_(A, B) A ## B
#define bjl_metamacro_foreach_iter(INDEX, MACRO, ARG) MACRO(INDEX, ARG)
#define bjl_metamacro_head_(FIRST, ...) FIRST
#define bjl_metamacro_tail_(FIRST, ...) __VA_ARGS__
#define bjl_metamacro_consume_(...)
#define bjl_metamacro_expand_(...) __VA_ARGS__

// implemented from scratch so that metamacro_concat() doesn't end up nesting
#define bjl_metamacro_foreach_concat_iter(INDEX, BASE, ARG) bjl_metamacro_foreach_concat_iter_(BASE, ARG)
#define bjl_metamacro_foreach_concat_iter_(BASE, ARG) BASE ## ARG

// metamacro_at expansions
#define bjl_metamacro_at0(...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at1(_0, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at2(_0, _1, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at3(_0, _1, _2, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at4(_0, _1, _2, _3, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at5(_0, _1, _2, _3, _4, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at6(_0, _1, _2, _3, _4, _5, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at7(_0, _1, _2, _3, _4, _5, _6, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at8(_0, _1, _2, _3, _4, _5, _6, _7, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at9(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at17(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at18(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at19(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, ...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_at20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, ...) bjl_metamacro_head(__VA_ARGS__)

// metamacro_foreach_cxt expansions
#define bjl_metamacro_foreach_cxt0(MACRO, SEP, CONTEXT)
#define bjl_metamacro_foreach_cxt1(MACRO, SEP, CONTEXT, _0) MACRO(0, CONTEXT, _0)

#define bjl_metamacro_foreach_cxt2(MACRO, SEP, CONTEXT, _0, _1) \
    bjl_metamacro_foreach_cxt1(MACRO, SEP, CONTEXT, _0) \
    SEP \
    MACRO(1, CONTEXT, _1)

#define bjl_metamacro_foreach_cxt3(MACRO, SEP, CONTEXT, _0, _1, _2) \
    bjl_metamacro_foreach_cxt2(MACRO, SEP, CONTEXT, _0, _1) \
    SEP \
    MACRO(2, CONTEXT, _2)

#define bjl_metamacro_foreach_cxt4(MACRO, SEP, CONTEXT, _0, _1, _2, _3) \
    bjl_metamacro_foreach_cxt3(MACRO, SEP, CONTEXT, _0, _1, _2) \
    SEP \
    MACRO(3, CONTEXT, _3)

#define bjl_metamacro_foreach_cxt5(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4) \
    bjl_metamacro_foreach_cxt4(MACRO, SEP, CONTEXT, _0, _1, _2, _3) \
    SEP \
    MACRO(4, CONTEXT, _4)

#define bjl_metamacro_foreach_cxt6(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5) \
    bjl_metamacro_foreach_cxt5(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4) \
    SEP \
    MACRO(5, CONTEXT, _5)

#define bjl_metamacro_foreach_cxt7(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
    bjl_metamacro_foreach_cxt6(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5) \
    SEP \
    MACRO(6, CONTEXT, _6)

#define bjl_metamacro_foreach_cxt8(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
    bjl_metamacro_foreach_cxt7(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
    SEP \
    MACRO(7, CONTEXT, _7)

#define bjl_metamacro_foreach_cxt9(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
    bjl_metamacro_foreach_cxt8(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
    SEP \
    MACRO(8, CONTEXT, _8)

#define bjl_metamacro_foreach_cxt10(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    bjl_metamacro_foreach_cxt9(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
    SEP \
    MACRO(9, CONTEXT, _9)

#define bjl_metamacro_foreach_cxt11(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    bjl_metamacro_foreach_cxt10(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    SEP \
    MACRO(10, CONTEXT, _10)

#define bjl_metamacro_foreach_cxt12(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
    bjl_metamacro_foreach_cxt11(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    SEP \
    MACRO(11, CONTEXT, _11)

#define bjl_metamacro_foreach_cxt13(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
    bjl_metamacro_foreach_cxt12(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
    SEP \
    MACRO(12, CONTEXT, _12)

#define bjl_metamacro_foreach_cxt14(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
    bjl_metamacro_foreach_cxt13(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
    SEP \
    MACRO(13, CONTEXT, _13)

#define bjl_metamacro_foreach_cxt15(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
    bjl_metamacro_foreach_cxt14(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
    SEP \
    MACRO(14, CONTEXT, _14)

#define bjl_metamacro_foreach_cxt16(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
    bjl_metamacro_foreach_cxt15(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
    SEP \
    MACRO(15, CONTEXT, _15)

#define bjl_metamacro_foreach_cxt17(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
    bjl_metamacro_foreach_cxt16(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
    SEP \
    MACRO(16, CONTEXT, _16)

#define bjl_metamacro_foreach_cxt18(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
    bjl_metamacro_foreach_cxt17(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
    SEP \
    MACRO(17, CONTEXT, _17)

#define bjl_metamacro_foreach_cxt19(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
    bjl_metamacro_foreach_cxt18(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
    SEP \
    MACRO(18, CONTEXT, _18)

#define bjl_metamacro_foreach_cxt20(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) \
    bjl_metamacro_foreach_cxt19(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
    SEP \
    MACRO(19, CONTEXT, _19)

// metamacro_foreach_cxt_recursive expansions
#define bjl_metamacro_foreach_cxt_recursive0(MACRO, SEP, CONTEXT)
#define bjl_metamacro_foreach_cxt_recursive1(MACRO, SEP, CONTEXT, _0) MACRO(0, CONTEXT, _0)

#define bjl_metamacro_foreach_cxt_recursive2(MACRO, SEP, CONTEXT, _0, _1) \
    bjl_metamacro_foreach_cxt_recursive1(MACRO, SEP, CONTEXT, _0) \
    SEP \
    MACRO(1, CONTEXT, _1)

#define bjl_metamacro_foreach_cxt_recursive3(MACRO, SEP, CONTEXT, _0, _1, _2) \
    bjl_metamacro_foreach_cxt_recursive2(MACRO, SEP, CONTEXT, _0, _1) \
    SEP \
    MACRO(2, CONTEXT, _2)

#define bjl_metamacro_foreach_cxt_recursive4(MACRO, SEP, CONTEXT, _0, _1, _2, _3) \
    bjl_metamacro_foreach_cxt_recursive3(MACRO, SEP, CONTEXT, _0, _1, _2) \
    SEP \
    MACRO(3, CONTEXT, _3)

#define bjl_metamacro_foreach_cxt_recursive5(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4) \
    bjl_metamacro_foreach_cxt_recursive4(MACRO, SEP, CONTEXT, _0, _1, _2, _3) \
    SEP \
    MACRO(4, CONTEXT, _4)

#define bjl_metamacro_foreach_cxt_recursive6(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5) \
    bjl_metamacro_foreach_cxt_recursive5(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4) \
    SEP \
    MACRO(5, CONTEXT, _5)

#define bjl_metamacro_foreach_cxt_recursive7(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
    bjl_metamacro_foreach_cxt_recursive6(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5) \
    SEP \
    MACRO(6, CONTEXT, _6)

#define bjl_metamacro_foreach_cxt_recursive8(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
    bjl_metamacro_foreach_cxt_recursive7(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
    SEP \
    MACRO(7, CONTEXT, _7)

#define bjl_metamacro_foreach_cxt_recursive9(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
    bjl_metamacro_foreach_cxt_recursive8(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
    SEP \
    MACRO(8, CONTEXT, _8)

#define bjl_metamacro_foreach_cxt_recursive10(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    bjl_metamacro_foreach_cxt_recursive9(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
    SEP \
    MACRO(9, CONTEXT, _9)

#define bjl_metamacro_foreach_cxt_recursive11(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    bjl_metamacro_foreach_cxt_recursive10(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    SEP \
    MACRO(10, CONTEXT, _10)

#define bjl_metamacro_foreach_cxt_recursive12(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
    bjl_metamacro_foreach_cxt_recursive11(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    SEP \
    MACRO(11, CONTEXT, _11)

#define bjl_metamacro_foreach_cxt_recursive13(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
    bjl_metamacro_foreach_cxt_recursive12(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
    SEP \
    MACRO(12, CONTEXT, _12)

#define bjl_metamacro_foreach_cxt_recursive14(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
    bjl_metamacro_foreach_cxt_recursive13(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
    SEP \
    MACRO(13, CONTEXT, _13)

#define bjl_metamacro_foreach_cxt_recursive15(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
    bjl_metamacro_foreach_cxt_recursive14(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
    SEP \
    MACRO(14, CONTEXT, _14)

#define bjl_metamacro_foreach_cxt_recursive16(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
    bjl_metamacro_foreach_cxt_recursive15(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
    SEP \
    MACRO(15, CONTEXT, _15)

#define bjl_metamacro_foreach_cxt_recursive17(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
    bjl_metamacro_foreach_cxt_recursive16(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
    SEP \
    MACRO(16, CONTEXT, _16)

#define bjl_metamacro_foreach_cxt_recursive18(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
    bjl_metamacro_foreach_cxt_recursive17(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
    SEP \
    MACRO(17, CONTEXT, _17)

#define bjl_metamacro_foreach_cxt_recursive19(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
    bjl_metamacro_foreach_cxt_recursive18(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
    SEP \
    MACRO(18, CONTEXT, _18)

#define bjl_metamacro_foreach_cxt_recursive20(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) \
    bjl_metamacro_foreach_cxt_recursive19(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
    SEP \
    MACRO(19, CONTEXT, _19)

// metamacro_for_cxt expansions
#define bjl_metamacro_for_cxt0(MACRO, SEP, CONTEXT)
#define bjl_metamacro_for_cxt1(MACRO, SEP, CONTEXT) MACRO(0, CONTEXT)

#define bjl_metamacro_for_cxt2(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt1(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(1, CONTEXT)

#define bjl_metamacro_for_cxt3(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt2(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(2, CONTEXT)

#define bjl_metamacro_for_cxt4(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt3(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(3, CONTEXT)

#define bjl_metamacro_for_cxt5(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt4(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(4, CONTEXT)

#define bjl_metamacro_for_cxt6(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt5(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(5, CONTEXT)

#define bjl_metamacro_for_cxt7(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt6(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(6, CONTEXT)

#define bjl_metamacro_for_cxt8(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt7(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(7, CONTEXT)

#define bjl_metamacro_for_cxt9(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt8(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(8, CONTEXT)

#define bjl_metamacro_for_cxt10(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt9(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(9, CONTEXT)

#define bjl_metamacro_for_cxt11(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt10(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(10, CONTEXT)

#define bjl_metamacro_for_cxt12(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt11(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(11, CONTEXT)

#define bjl_metamacro_for_cxt13(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt12(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(12, CONTEXT)

#define bjl_metamacro_for_cxt14(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt13(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(13, CONTEXT)

#define bjl_metamacro_for_cxt15(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt14(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(14, CONTEXT)

#define bjl_metamacro_for_cxt16(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt15(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(15, CONTEXT)

#define bjl_metamacro_for_cxt17(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt16(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(16, CONTEXT)

#define bjl_metamacro_for_cxt18(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt17(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(17, CONTEXT)

#define bjl_metamacro_for_cxt19(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt18(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(18, CONTEXT)

#define bjl_metamacro_for_cxt20(MACRO, SEP, CONTEXT) \
    bjl_metamacro_for_cxt19(MACRO, SEP, CONTEXT) \
    SEP \
    MACRO(19, CONTEXT)

// metamacro_if_eq expansions
#define bjl_metamacro_if_eq0(VALUE) \
    bjl_metamacro_concat(bjl_metamacro_if_eq0_, VALUE)

#define bjl_metamacro_if_eq0_0(...) __VA_ARGS__ bjl_metamacro_consume_
#define bjl_metamacro_if_eq0_1(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_2(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_3(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_4(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_5(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_6(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_7(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_8(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_9(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_10(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_11(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_12(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_13(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_14(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_15(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_16(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_17(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_18(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_19(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq0_20(...) bjl_metamacro_expand_

#define bjl_metamacro_if_eq1(VALUE) bjl_metamacro_if_eq0(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq2(VALUE) bjl_metamacro_if_eq1(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq3(VALUE) bjl_metamacro_if_eq2(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq4(VALUE) bjl_metamacro_if_eq3(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq5(VALUE) bjl_metamacro_if_eq4(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq6(VALUE) bjl_metamacro_if_eq5(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq7(VALUE) bjl_metamacro_if_eq6(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq8(VALUE) bjl_metamacro_if_eq7(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq9(VALUE) bjl_metamacro_if_eq8(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq10(VALUE) bjl_metamacro_if_eq9(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq11(VALUE) bjl_metamacro_if_eq10(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq12(VALUE) bjl_metamacro_if_eq11(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq13(VALUE) bjl_metamacro_if_eq12(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq14(VALUE) bjl_metamacro_if_eq13(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq15(VALUE) bjl_metamacro_if_eq14(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq16(VALUE) bjl_metamacro_if_eq15(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq17(VALUE) bjl_metamacro_if_eq16(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq18(VALUE) bjl_metamacro_if_eq17(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq19(VALUE) bjl_metamacro_if_eq18(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq20(VALUE) bjl_metamacro_if_eq19(bjl_metamacro_dec(VALUE))

// metamacro_if_eq_recursive expansions
#define bjl_metamacro_if_eq_recursive0(VALUE) \
    bjl_metamacro_concat(bjl_metamacro_if_eq_recursive0_, VALUE)

#define bjl_metamacro_if_eq_recursive0_0(...) __VA_ARGS__ bjl_metamacro_consume_
#define bjl_metamacro_if_eq_recursive0_1(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_2(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_3(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_4(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_5(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_6(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_7(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_8(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_9(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_10(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_11(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_12(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_13(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_14(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_15(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_16(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_17(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_18(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_19(...) bjl_metamacro_expand_
#define bjl_metamacro_if_eq_recursive0_20(...) bjl_metamacro_expand_

#define bjl_metamacro_if_eq_recursive1(VALUE) bjl_metamacro_if_eq_recursive0(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive2(VALUE) bjl_metamacro_if_eq_recursive1(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive3(VALUE) bjl_metamacro_if_eq_recursive2(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive4(VALUE) bjl_metamacro_if_eq_recursive3(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive5(VALUE) bjl_metamacro_if_eq_recursive4(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive6(VALUE) bjl_metamacro_if_eq_recursive5(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive7(VALUE) bjl_metamacro_if_eq_recursive6(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive8(VALUE) bjl_metamacro_if_eq_recursive7(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive9(VALUE) bjl_metamacro_if_eq_recursive8(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive10(VALUE) bjl_metamacro_if_eq_recursive9(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive11(VALUE) bjl_metamacro_if_eq_recursive10(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive12(VALUE) bjl_metamacro_if_eq_recursive11(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive13(VALUE) bjl_metamacro_if_eq_recursive12(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive14(VALUE) bjl_metamacro_if_eq_recursive13(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive15(VALUE) bjl_metamacro_if_eq_recursive14(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive16(VALUE) bjl_metamacro_if_eq_recursive15(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive17(VALUE) bjl_metamacro_if_eq_recursive16(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive18(VALUE) bjl_metamacro_if_eq_recursive17(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive19(VALUE) bjl_metamacro_if_eq_recursive18(bjl_metamacro_dec(VALUE))
#define bjl_metamacro_if_eq_recursive20(VALUE) bjl_metamacro_if_eq_recursive19(bjl_metamacro_dec(VALUE))

// metamacro_take expansions
#define bjl_metamacro_take0(...)
#define bjl_metamacro_take1(...) bjl_metamacro_head(__VA_ARGS__)
#define bjl_metamacro_take2(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take1(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take3(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take2(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take4(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take3(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take5(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take4(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take6(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take5(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take7(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take6(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take8(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take7(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take9(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take8(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take10(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take9(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take11(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take10(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take12(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take11(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take13(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take12(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take14(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take13(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take15(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take14(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take16(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take15(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take17(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take16(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take18(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take17(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take19(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take18(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_take20(...) bjl_metamacro_head(__VA_ARGS__), bjl_metamacro_take19(bjl_metamacro_tail(__VA_ARGS__))

// metamacro_drop expansions
#define bjl_metamacro_drop0(...) __VA_ARGS__
#define bjl_metamacro_drop1(...) bjl_metamacro_tail(__VA_ARGS__)
#define bjl_metamacro_drop2(...) bjl_metamacro_drop1(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop3(...) bjl_metamacro_drop2(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop4(...) bjl_metamacro_drop3(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop5(...) bjl_metamacro_drop4(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop6(...) bjl_metamacro_drop5(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop7(...) bjl_metamacro_drop6(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop8(...) bjl_metamacro_drop7(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop9(...) bjl_metamacro_drop8(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop10(...) bjl_metamacro_drop9(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop11(...) bjl_metamacro_drop10(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop12(...) bjl_metamacro_drop11(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop13(...) bjl_metamacro_drop12(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop14(...) bjl_metamacro_drop13(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop15(...) bjl_metamacro_drop14(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop16(...) bjl_metamacro_drop15(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop17(...) bjl_metamacro_drop16(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop18(...) bjl_metamacro_drop17(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop19(...) bjl_metamacro_drop18(bjl_metamacro_tail(__VA_ARGS__))
#define bjl_metamacro_drop20(...) bjl_metamacro_drop19(bjl_metamacro_tail(__VA_ARGS__))

#endif

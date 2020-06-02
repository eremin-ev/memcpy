#pragma once

/* log2 magic */

/* The tgmath real type for T, where E is 0 if T is an integer type
   and 1 for a floating type.  If T has a complex type, it is
   unspecified whether the return type is real or complex (but it has
   the correct corresponding real type).  */
#  define __tgmath_real_type_sub(T, E) \
  __typeof__ (*(0 ? (__typeof__ (0 ? (double *) 0 : (void *) (E))) 0	      \
		  : (__typeof__ (0 ? (T *) 0 : (void *) (!(E)))) 0))

/* The tgmath real type of EXPR.  */
#  define __tgmath_real_type(expr) \
  __tgmath_real_type_sub (__typeof__ ((__typeof__ (+(expr))) 0),	      \
			  __floating_type (__typeof__ (+(expr))))

#if __HAVE_BUILTIN_TGMATH

# define __TGMATH_UNARY_REAL_ONLY(Val, Fct) __TGMATH_1 (Fct, (Val))

#else /* !__HAVE_BUILTIN_TGMATH.  */

# define __TGMATH_UNARY_REAL_ONLY(Val, Fct)				\
  (__extension__ ((sizeof (+(Val)) == sizeof (double)			      \
		      || __builtin_classify_type (Val) != 8)		      \
		     ? (__tgmath_real_type (Val)) Fct (Val)		      \
		     : (sizeof (+(Val)) == sizeof (float))		      \
		     ? (__tgmath_real_type (Val)) Fct##f (Val)		      \
		     : __TGMATH_F128 ((Val), (__tgmath_real_type (Val)) Fct,  \
				      (Val))				      \
		     (__tgmath_real_type (Val)) __tgml(Fct) (Val)))

#endif /* !__HAVE_BUILTIN_TGMATH.  */

/* Compute base-2 logarithm of X.  */
#define log2(Val) __TGMATH_UNARY_REAL_ONLY (Val, log2)

#define ASM_SIZE_DIRECTIVE(name) .size name,.-name

/* The log2 of the minimum alignment required for an address that
   is the target of a computed branch (i.e. a "bx" instruction).
   A more-specific arm-features.h file may define this to set a more
   stringent requirement.

   Using this only makes sense for code in ARM mode (where instructions
   always have a fixed size of four bytes), or for Thumb-mode code that is
   specifically aligning all the related branch targets to match (since
   Thumb instructions might be either two or four bytes).  */
#ifndef ARM_BX_ALIGN_LOG2
# define ARM_BX_ALIGN_LOG2	2
#endif

/* Define a macro we can use to construct the asm name for a C symbol.  */
# define C_LABEL(name)	name##:

# define C_SYMBOL_NAME(name) name

# define cfi_startproc			.cfi_startproc
# define cfi_endproc			.cfi_endproc
# define cfi_def_cfa(reg, off)		.cfi_def_cfa reg, off
# define cfi_def_cfa_register(reg)	.cfi_def_cfa_register reg
# define cfi_def_cfa_offset(off)	.cfi_def_cfa_offset off
# define cfi_adjust_cfa_offset(off)	.cfi_adjust_cfa_offset off
# define cfi_offset(reg, off)		.cfi_offset reg, off
# define cfi_rel_offset(reg, off)	.cfi_rel_offset reg, off
# define cfi_register(r1, r2)		.cfi_register r1, r2
# define cfi_return_column(reg)	.cfi_return_column reg
# define cfi_restore(reg)		.cfi_restore reg
# define cfi_same_value(reg)		.cfi_same_value reg
# define cfi_undefined(reg)		.cfi_undefined reg
# define cfi_remember_state		.cfi_remember_state
# define cfi_restore_state		.cfi_restore_state
# define cfi_window_save		.cfi_window_save
# define cfi_personality(enc, exp)	.cfi_personality enc, exp
# define cfi_lsda(enc, exp)		.cfi_lsda enc, exp

#define CFI_SECTIONS					\
	.cfi_sections .debug_frame

/* If compiled for profiling, call `mcount' at the start of each function.  */
/* If not, Do nothing.  */
#define CALL_MCOUNT

#define ALIGNARG(log2) log2

/* Define an entry point visible from C.  */
#define	ENTRY(name)					\
	.globl	C_SYMBOL_NAME(name);			\
	.type	C_SYMBOL_NAME(name),%function;		\
	.align	ALIGNARG(4);				\
  C_LABEL(name)						\
	CFI_SECTIONS;					\
	cfi_startproc;					\
	CALL_MCOUNT

#undef	END
#define END(name)					\
	cfi_endproc;					\
	ASM_SIZE_DIRECTIVE(name)

#define libc_hidden_builtin_def(entry)

#define IS_IN(arg)	0

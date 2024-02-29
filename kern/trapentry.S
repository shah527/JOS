/* See COPYRIGHT for copyright information. */

#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/trap.h>



###################################################################
# exceptions/interrupts
###################################################################

/* TRAPHANDLER defines a globally-visible function for handling a trap.
 * It pushes a trap number onto the stack, then jumps to _alltraps.
 * Use TRAPHANDLER for traps where the CPU automatically pushes an error code.
 *
 * You shouldn't call a TRAPHANDLER function from C, but you may
 * need to _declare_ one in C (for instance, to get a function pointer
 * during IDT setup).  You can declare the function with
 *   void NAME();
 * where NAME is the argument passed to TRAPHANDLER.
 */
#define TRAPHANDLER(name, num)						\
	.globl name;		/* define global symbol for 'name' */	\
	.type name, @function;	/* symbol type is function */		\
	.align 2;		/* align function definition */		\
	name:			/* function starts here */		\
	pushl $(num);							\
	jmp _alltraps

/* Use TRAPHANDLER_NOEC for traps where the CPU doesn't push an error code.
 * It pushes a 0 in place of the error code, so the trap frame has the same
 * format in either case.
 */
#define TRAPHANDLER_NOEC(name, num)					\
	.globl name;							\
	.type name, @function;						\
	.align 2;							\
	name:								\
	pushl $0;							\
	pushl $(num);							\
	jmp _alltraps

.text

/*
 * Lab 3: Your code here for generating entry points for the different traps.
 */

// HINT 1 : TRAPHANDLER_NOEC(t_divide, T_DIVIDE);
//          Do something like this if there is no error code for the trap
// HINT 2 : TRAPHANDLER(t_dblflt, T_DBLFLT);
//          Do something like this if the trap includes an error code..
// HINT 3 : READ Intel's manual to check if the trap includes an error code
//          or not...

/*
 * Lab 3: Your code here for _alltraps
 */


TRAPHANDLER_NOEC(t_divide, T_DIVIDE);
TRAPHANDLER_NOEC(t_debug, T_DEBUG);
TRAPHANDLER_NOEC(t_nmi, T_NMI);
TRAPHANDLER_NOEC(t_brkpt, T_BRKPT);
TRAPHANDLER_NOEC(t_oflow, T_OFLOW);
TRAPHANDLER_NOEC(t_bound, T_BOUND);
TRAPHANDLER_NOEC(t_illop, T_ILLOP);
TRAPHANDLER_NOEC(t_device, T_DEVICE);
TRAPHANDLER(t_dblflt, T_DBLFLT);
TRAPHANDLER_NOEC(t_tss, T_TSS);
TRAPHANDLER(t_segnp, T_SEGNP);
TRAPHANDLER(t_stack, T_STACK);
TRAPHANDLER(t_gpflt, T_GPFLT);
TRAPHANDLER(t_pgflt, T_PGFLT);
TRAPHANDLER_NOEC(t_fperr, T_FPERR);
TRAPHANDLER(t_align, T_ALIGN);
TRAPHANDLER_NOEC(t_mchk, T_MCHK);
TRAPHANDLER_NOEC(t_simderr, T_SIMDERR);
TRAPHANDLER_NOEC(t_syscall, T_SYSCALL);
.globl _alltraps
_alltraps:
    pushl %ds
    pushl %es
    pushal
    movw $GD_KD, %bx
    movw %bx, %ds
    movw %bx, %es
    pushl %esp
    call trap


/* LLMPROMPT:  how to save registers and set up a stack frame trapentry do not give code explain me*/
/* LLMPROMPT: alltraps not working how to fix */
/* LLMPROMPT: this is my output where  could it be wrong how is the fix what explain concepts only
AssertionError: ...
         check_page_installed_pgdir() succeeded!
         [00000000] new env 00001000
    GOOD Incoming TRAP frame at 0xefffffc4
         TRAP frame at 0xefffffc4
           edi  0x00000000
    ...
           cs   0x----dfc0
           flag 0x00000023
    GOOD [00001000] free env 00001000
         Destroyed the only environment - nothing more to do!
         Welcome to the JOS kernel monitor!
         Type 'help' for a list of commands.
         qemu-system-i386: terminating on signal 15 from pid 1546179 (make)
    MISSING 'TRAP frame at 0xf.......'
    MISSING '  trap 0x00000000 Divide error'
    MISSING '  eip  0x008.....'
    MISSING '  ss   0x----0023'
    
    QEMU output saved to jos.out.divzero
softint: FAIL (1.0s) 
    AssertionError: ...
           esp  0x00000023
           ss   0x----ff53
    GOOD [00001000] free env 00001000
         Destroyed the only environment - nothing more to do!
    GOOD Welcome to the JOS kernel monitor!
         Type 'help' for a list of commands.
         qemu-system-i386: terminating on signal 15 from pid 1548218 (make)
    MISSING 'Incoming TRAP frame at 0xefffffbc'
    MISSING 'TRAP frame at 0xf.......'
    MISSING '  trap 0x0000000d General Protection'
    MISSING '  eip  0x008.....'
    MISSING '  ss   0x----0023'
    
    QEMU output saved to jos.out.softint
badsegment: FAIL (0.9s) 
    AssertionError: ...
           esp  0x00000023
           ss   0x----ff53
    GOOD [00001000] free env 00001000
         Destroyed the only environment - nothing more to do!
         Welcome to the JOS kernel monitor!
         Type 'help' for a list of commands.
         qemu-system-i386: terminating on signal 15 from pid 1550893 (make)
    MISSING 'Incoming TRAP frame at 0xefffffbc'
    MISSING 'TRAP frame at 0xf.......'
    MISSING '  trap 0x0000000d General Protection'
    MISSING '  err  0x00000028'
    MISSING '  eip  0x008.....'
    MISSING '  ss   0x----0023'
    
    QEMU output saved to jos.out.badsegment
Part A score: 0/30 
*/
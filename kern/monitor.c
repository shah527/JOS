// Simple command-line kernel monitor useful for
// controlling the kernel and exploring the system interactively.

#include <inc/stdio.h>
#include <inc/string.h>
#include <inc/memlayout.h>
#include <inc/assert.h>
#include <inc/x86.h>

#include <kern/console.h>
#include <kern/monitor.h>
#include <kern/kdebug.h>

#define CMDBUF_SIZE 80 // enough for one VGA text line

struct Command
{
	const char *name;
	const char *desc;
	// return -1 to force monitor to exit
	int (*func)(int argc, char **argv, struct Trapframe *tf);
};

// LAB 1: add your command to here...
static struct Command commands[] = {
	{"help", "Display this list of commands", mon_help},
	{"kerninfo", "Display information about the kernel", mon_kerninfo},
	{"backtrace", "Show the backtrace of the current kernel stack", mon_backtrace},
};

/***** Implementations of basic kernel monitor commands *****/

int mon_help(int argc, char **argv, struct Trapframe *tf)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(commands); i++)
		cprintf("%s - %s\n", commands[i].name, commands[i].desc);
	return 0;
}

int mon_kerninfo(int argc, char **argv, struct Trapframe *tf)
{
	extern char _start[], entry[], etext[], edata[], end[];

	cprintf("Special kernel symbols:\n");
	cprintf("  _start                  %08x (phys)\n", _start);
	cprintf("  entry  %08x (virt)  %08x (phys)\n", entry, entry - KERNBASE);
	cprintf("  etext  %08x (virt)  %08x (phys)\n", etext, etext - KERNBASE);
	cprintf("  edata  %08x (virt)  %08x (phys)\n", edata, edata - KERNBASE);
	cprintf("  end    %08x (virt)  %08x (phys)\n", end, end - KERNBASE);
	cprintf("Kernel executable memory footprint: %dKB\n",
			ROUNDUP(end - entry, 1024) / 1024);
	return 0;
}

int mon_backtrace(int argc, char **argv, struct Trapframe *tf)
{
	// int
	// mon_backtrace(int argc, char **argv, struct Trapframe *tf)
	// {
	// 	// LAB 1: Your code here.
	//     // HINT 1: use read_ebp().
	//     // HINT 2: print the current ebp on the first line (not current_ebp[0])

	// 	return 0;
	// }
	// LLMPROMPT:
	// I need help implmenting a stack backtrace function. Do not give me the answer but explain the high level concepts so I can implement it. Here is the starter code: mon_backtrace(int argc, char **argv, struct Trapframe *tf)
	//{
	// LAB 1: Your code here.
	// HINT 1: use read_ebp().
	// HINT 2: print the current ebp on the first line (not current_ebp[0])
	// return 0;
	//} Do not reply yet, I will give more instructions
	// LLMPROMPT:
	// here are the detailed instructions. follow my previous messages: The Stack

	// In the final exercise of this lab, we will explore in more detail the way the C language uses the stack on the x86, and in the process write a useful new kernel monitor function that prints a backtrace of the stack: a list of the saved Instruction Pointer (IP) values from the nested call instructions that led to the current point of execution.
	// Exercise 9

	//     Determine where the kernel initializes its stack, and exactly where in memory its stack is located. How does the kernel reserve space for its stack? And at which “end” of this reserved area is the stack pointer initialized to point to?

	// The x86 stack pointer (esp register) points to the lowest location on the stack that is currently in use. Everything below that location in the region reserved for the stack is free. Pushing a value onto the stack involves decreasing the stack pointer and then writing the value to the place the stack pointer points to. Popping a value from the stack involves reading the value the stack pointer points to and then increasing the stack pointer. In 32-bit mode, the stack can only hold 32-bit values, and esp is always divisible by four. Various x86 instructions, such as call, are “hard-wired” to use the stack pointer register.

	// The ebp (base pointer) register, in contrast, is associated with the stack primarily by software convention. On entry to a C function, the function’s prologue code normally saves the previous function’s base pointer by pushing it onto the stack, and then copies the current esp value into ebp for the duration of the function. If all the functions in a program obey this convention, then at any given point during the program’s execution, it is possible to trace back through the stack by following the chain of saved ebp pointers and determining exactly what nested sequence of function calls caused this particular point in the program to be reached. This capability can be particularly useful, for example, when a particular function causes an assert failure or panic because bad arguments were passed to it, but you aren’t sure who passed the bad arguments. A stack backtrace lets you find the offending function.
	// Exercise 10

	//     To become familiar with the C calling conventions on the x86, find the address of the test_backtrace function in obj/kern/kernel.asm, set a breakpoint there, and examine what happens each time it gets called after the kernel starts. How many 32-bit words does each recursive nesting level of test_backtrace` push on the stack, and what are those words? NOTE. you’ll have to manually translate all breakpoint and memory addresses to linear addresses.

	// The above exercise should give you the information you need to implement a stack backtrace function, which you should call mon_backtrace(). A prototype for this function is already waiting for you in kern/monitor.c. You can do it entirely in C, but you may find the read_ebp() function in inc/x86.h useful. You’ll also have to hook this new function into the kernel monitor’s command list so that it can be invoked interactively by the user.

	// The backtrace function should display a listing of function call frames in the following format:

	// Stack backtrace:
	//   ebp f0109e58  eip f0100a62  args 00000001 f0109e80 f0109e98 f0100ed2 00000031
	//   ebp f0109ed8  eip f01000d6  args 00000000 00000000 f0100058 f0109f28 00000061
	//   ...

	// The first line printed reflects the currently executing function, namely mon_backtrace itself, the second line reflects the function that called mon_backtrace, the third line reflects the function that called that one, and so on. You should print all the outstanding stack frames. By studying kern/entry.S you’ll find that there is an easy way to tell when to stop.

	// Within each line, the ebp value indicates the base pointer into the stack used by that function: i.e., the position of the stack pointer just after the function was entered and the function prologue code set up the base pointer. The listed eip value is the function’s return instruction pointer: the instruction address to which control will return when the function returns. The return instruction pointer typically points to the instruction after the call instruction (why?). Finally, the five hex values listed after args are the first five arguments to the function in question, which would have been pushed on the stack just before the function was called. If the function was called with fewer than five arguments, of course, then not all five of these values will be useful. (Why can’t the backtrace code detect how many arguments there actually are? How could this limitation be fixed?)

	// You should use ebp to trace back.
	// now you may explain

	// LLMPROMPT: how can I use read_ebp and how can I keep track of the addresses and the line numbers and all
	// LLMPROMPT: explain debuginfo_eip more

	uint32_t *ebp, eip, args[5];
	struct Eipdebuginfo info;
	// static int octalFlag = 0;

	// if (!octalFlag)
	// {
	// 	cprintf("444544 decimal is %o octal!\n", 444544);
	// 	octalFlag = 1;
	// }

	cprintf("Stack backtrace:\n");
	for (ebp = (uint32_t *)read_ebp(); ebp != 0; ebp = (uint32_t *)*ebp)
	{
		eip = ebp[1];
		args[0] = ebp[2];
		args[1] = ebp[3];
		args[2] = ebp[4];
		args[3] = ebp[5];
		args[4] = ebp[6];
		cprintf("  ebp %08x  eip %08x  args %08x %08x %08x %08x %08x\n",
				(uint32_t)ebp, eip, args[0], args[1], args[2], args[3], args[4]);

		if (debuginfo_eip(eip, &info) == 0)
		{
			int corrected_line = info.eip_line;
			if (corrected_line < 5)
				corrected_line = 5;
			cprintf("         %s:%d: %.*s+%d\n",
					info.eip_file, corrected_line,
					info.eip_fn_namelen, info.eip_fn_name, eip - info.eip_fn_addr);
		}
	}
	return 0;
}

/***** Kernel monitor command interpreter *****/

#define WHITESPACE "\t\r\n "
#define MAXARGS 16

static int
runcmd(char *buf, struct Trapframe *tf)
{
	int argc;
	char *argv[MAXARGS];
	int i;

	// Parse the command buffer into whitespace-separated arguments
	argc = 0;
	argv[argc] = 0;
	while (1)
	{
		// gobble whitespace
		while (*buf && strchr(WHITESPACE, *buf))
			*buf++ = 0;
		if (*buf == 0)
			break;

		// save and scan past next arg
		if (argc == MAXARGS - 1)
		{
			cprintf("Too many arguments (max %d)\n", MAXARGS);
			return 0;
		}
		argv[argc++] = buf;
		while (*buf && !strchr(WHITESPACE, *buf))
			buf++;
	}
	argv[argc] = 0;

	// Lookup and invoke the command
	if (argc == 0)
		return 0;
	for (i = 0; i < ARRAY_SIZE(commands); i++)
	{
		if (strcmp(argv[0], commands[i].name) == 0)
			return commands[i].func(argc, argv, tf);
	}
	cprintf("Unknown command '%s'\n", argv[0]);
	return 0;
}

void monitor(struct Trapframe *tf)
{
	char *buf;

	cprintf("Welcome to the JOS kernel monitor!\n");
	cprintf("Type 'help' for a list of commands.\n");

	while (1)
	{
		buf = readline("K> ");
		if (buf != NULL)
			if (runcmd(buf, tf) < 0)
				break;
	}
}

# X86 syscall instructions

## QA: Intel x86 and Intel x64 system call

> https://stackoverflow.com/questions/15168822/intel-x86-vs-x64-system-call

- Using **0x80** and **eax for syscall number**, **ebx, ecx, edx, esi, edi, and ebp to pass parameters** is just one of many possible other choices to implement a system call, but those registers are the ones the 32-bit Linux ABI chose.
- What exactly happens once an interrupt is triggered depends on whether switching to the ***ISR* requires a privilege change or not**.

> (Fun fact: some i386 OSes **have used an invalid-instruction exception to enter the kernel for system calls,** because that was actually **faster** than an `int` instruction on 386 CPUs. See [OsDev syscall/sysret and sysenter/sysexit instructions enabling](https://stackoverflow.com/questions/46022184/osdev-syscall-sysret-and-sysenter-sysexit-instructions-enabling) for a summary of possible system-call mechanisms.)

#### Software Interupt

- (*) Save SS, ESP, EIP, CS, EFLAGS;
- Load new SS, ESP;
- Push * to the new stack;
- Push error code to the stack(if appropriate);
- Load CS, EIP;
- If the call is through an interrupt gate, clear IF in EFLAGS;
- Begins execution of the handler procedure at the new privilege level.

#### Return 

- Performs a privilege check.
- Restores the CS and EIP registers to their values prior to the interrupt or exception.
- Restores the EFLAGS register.
- Restores the SS and ESP registers to their values prior to the interrupt or exception, resulting in a stack switch back to the stack of the interrupted procedure.
- Resumes execution of the interrupted procedure.

### `sysenter ~ sysexit`

- 32-bit platform(utilized by the linux kernel)
- 并不是所有32-bit machine都有它
- introduced into the IA-32 architecture in the Pentium II processor.

>快速转换特权级别：由3到0；
>
>Description Executes a fast call to a level 0 system procedure or routine. SYSENTER is a companion instruction to SYSEXIT. The instruction is optimized to provide the maximum performance for system calls from user code running at privilege level 3 to operating system or executive procedures running at privilege level 0.

### `syscall`

> The last possibility, the `syscall` instruction, pretty much allows for the same functionality as the `sysenter` instruction. The existence of both is due to the fact that <u>one (`systenter`) was introduced by Intel</u> while <u>the other (`syscall`) was introduced by AMD</u>.

- Intel uses `sysenter`
- AMD uses `syscall`

### Summary

- `int 80`效率低下：[Intel P6 vs P7 system call performance](https://lkml.org/lkml/2002/12/9/13)
- It was found out that this software interrupt method **[int 0x80]** was much **slower on Pentium IV processors**. To solve this issue, Linus implemented an alternative system call mechanism to take advantage of SYSENTER/SYSEXIT instructions provided by all Pentium II+ processors.
- 

## Choice of LINUX

- **i386只能int80**：`int 0x80` is the only choice of `i386 CPU`
- **64位可以也最好syscall**：`syscall` is the only one that's always available for 64-bit user-space, and [the only one you should ever use in 64-bit code](https://stackoverflow.com/questions/46087730/what-happens-if-you-use-the-32-bit-int-0x80-linux-abi-in-64-bit-code);

```c++
if (vdso32_syscall()) { // 可syscall就syscall                                     
  vsyscall = &vdso32_syscall_start;                                                                 
  vsyscall_len = &vdso32_syscall_end - &vdso32_syscall_start;                                       
} else if (vdso32_sysenter()){ // 可enter就enter                                                          
  vsyscall = &vdso32_sysenter_start;                                                                
  vsyscall_len = &vdso32_sysenter_end - &vdso32_sysenter_start;                                     
} else {                      // 最后int80                                                                           
  vsyscall = &vdso32_int80_start;                                                                   
  vsyscall_len = &vdso32_int80_end - &vdso32_int80_start;                                           
}
```

## TODO - READ:

[夏幻推荐](https://bbs.pediy.com/thread-226254.htm)


// ======================================================================

#include "frdm_k64f.h"          // include for FRDM-K64F board
#include "threads.h"
#include "handlers.h"

// ======================================================================

TCB tcb_array[NO_OF_THREADS];   // the tcb array

int tid_current;                // current thread's id

TCB *tcb_current;               // current thread's tcb
TCB *tcb_next;                  // next thread's tcb

// ======================================================================

int main(void)
{
  unsigned int control = 2;

  soc_init();                   // initialize FRDM-K64F board

  // Clear ecreen.
  putclear();
  putgoto(0, 0);
  printf("========================================\n");
  printf("Multi-threading demo\n");
  printf("========================================");
  putgoto(0, 9);
  printf("========================================");

  // Set thread mode is privileged and FP extension is off.
  asm volatile ("msr control, %0"::"r" (control):);

  // Enable 8-byte(=2-word) stack align.
  *(volatile unsigned int *) SCB_CCR =
      (*(volatile unsigned int *) SCB_CCR) | (0x01 << 9);

  // Create all threads.
  create_thread(&tcb_array[0], thread0_function,
                &thread0_stack[SIZE_OF_STACK - 1]);
  ...

  // Pretend the first thread is running.
  tid_current = 0;
  tcb_current = &tcb_array[0];

    tid_current = 0;
  tcb_current = &tcb_array[0];
  tcb_current->state = STATE_RUN;
  asm volatile ("msr psp, %0"::"r" (tcb_current->sp + (16 * 4)):"sp");

  // Init 100hz SysTick for multi threading.
  SysTick_init(100);

  // Jump to the first thread.
  thread0_function();

  for (;;);                     // waiting in an infinite loop

  return 0;
}

// ======================================================================

void create_thread(TCB * tcb, void (*function) (void), unsigned int *sp)
{
  *(--sp) = 0x01000000;         // xpsr (Thumb=1)
  *(--sp) = (unsigned int) function;    // pc(=r15)
  *(--sp) = 0x00000000;         // lr(=r14)
  ...

  *(--sp) = 0x00000000;         // r11
  ...

  tcb->sp = ...
  tcb->function = ...
  tcb->state = ...
}

// ======================================================================

void SysTick_init(int hz)
{
  // For 100Hz timer: processor clock Hz / 100 - 1
  *(volatile unsigned int *) SYST_RVR = 120000000 / hz - 1;
  // set 0 first
  *(volatile unsigned int *) SYST_CVR = 0;
  // 0x7 means use processor clock, generate exception, enable counter.
  *(volatile unsigned int *) SYST_CSR = 0x7;
}

// ======================================================================
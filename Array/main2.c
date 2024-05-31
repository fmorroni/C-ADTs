#include "arrayADT.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { READY, RUNNING, BLOCKED, EXITED } State;

typedef struct PCB {
  uint32_t pid;
  uint8_t priority;
  State state;
  void* rsp;
  void* rbp;
  char* name;
  int waitedProcessExitCode;
  Array waitingForMe;
  void* stack;
} PCB;

const char* const StateStrings[4] = {"READY", "RUNNING", "BLOCKED", "EXITED"};

void printPCB(PCB* pcb) {
  printf(
    "{\n"
    "  pid: %d,\n"
    "  name: %s,\n"
    "  state: %s,\n"
    "  rsp: %p,\n"
    "  rbp: %p,\n"
    "  priority: %d\n"
    "  waitingForMe: ",
    pcb->pid, pcb->name, StateStrings[pcb->state], pcb->rsp, pcb->rbp, pcb->priority
  );
  Array_print(pcb->waitingForMe);
  printf("}");
}

void printPtr(PCB** ptr) {
  printf("%p", (void*)*ptr);
}

void setPCB(PCB* pcb, uint32_t pid, uint8_t priority, State state, void* stack, void* rsp, void* rbp, char* name) {
  pcb->pid = pid;
  pcb->priority = priority;
  pcb->state = state;
  pcb->stack = stack;
  pcb->rsp = rsp;
  pcb->rbp = rbp;
  pcb->name = name;
  pcb->waitingForMe = Array_initialize(sizeof(PCB*), 10, (PrintFn)printPtr, NULL);
}

void freePCB(PCB* pcb) {
  Array_free(pcb->waitingForMe);
}

int main() {
  Array a = Array_initialize(sizeof(PCB), 10, (PrintFn)printPCB, (FreeEleFn)freePCB);
  PCB pcb;
  setPCB(&pcb, 0, 1, BLOCKED, (void*)0x101030, (void*)0x102030, (void*)0x102030, "Hola");
  Array_push(a, &pcb);

  setPCB(&pcb, 1, 1, BLOCKED, (void*)0x103030, (void*)0x104030, (void*)0x104030, "Chau");
  Array_push(a, &pcb);
  Array_push(pcb.waitingForMe, Array_getEle(a, 0));

  setPCB(&pcb, 2, 1, BLOCKED, (void*)0x105030, (void*)0x106030, (void*)0x106030, "Chau2");
  Array_push(a, &pcb);

  setPCB(&pcb, 3, 1, BLOCKED, (void*)0x107030, (void*)0x108030, (void*)0x108030, "Carola");
  Array_push(a, &pcb);
  Array_push(pcb.waitingForMe, Array_getEle(a, 1));
  Array_push(pcb.waitingForMe, Array_getEle(a, 2));

  setPCB(&pcb, 4, 1, BLOCKED, (void*)0x108030, (void*)0x109030, (void*)0x109030, "asdf");
  Array_push(a, &pcb);
  setPCB(&pcb, 5, 1, READY, (void*)0x10A030, (void*)0x10B030, (void*)0x10B030, "asdf");
  Array_push(a, &pcb);
  setPCB(&pcb, 6, 1, READY, (void*)0x10C030, (void*)0x10D030, (void*)0x10D030, "qwer");
  Array_push(a, &pcb);
  Array_push(pcb.waitingForMe, Array_getEle(a, 4));

  Array_print(a);

  Array_free(a);
  return 0;
}

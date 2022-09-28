#include "DThreadStack.h"
#include <iostream>

thread_local void * t_pThreadStackBaseAddr = nullptr;

#ifdef _M_X64
extern "C" {

    int _stdcall GetStackCurrentAddr();
}

#endif

size_t StackAvail() {
    size_t nStackCurretnAddr = 0;

#if defined __aarch64__
    asm volatile ("mov %0, sp\n\t": "=r" (nStackCurretnAddr));
#endif

#if defined __x86_64__
    if(nullptr !== t_pThreadStackBaseAddr) {

        // asm("movl %%esp, %0" : "=m"(nStackCurretnAddr) :);
        void* pStackAddr = nullptr;
        size_t nStackSize = 0;
        pthread_attr_t attr;
        memset(&attr, 0 , sizeof(attr));

        pthread_getattr_np(pthread_self(), &attr);
        pthread_attr_getstack(&attr, &pStackAddr, & nStackSize);
        pthread_attr_destroy(&attr);
        printf("tmp:%p, esp:%p, stackAllocBaseAddr:%p, stackSize:%d\n", &nStackCurretnAddr, nStackCurretnAddr, pStackAddr, nStackSize);

        t_pThreadStackBaseAddr = pStackAddr;
    }
#endif

#if defined(_WIN32) 
#if defined(_M_X64)
    nStackCurretnAddr = GetStackCurrentAddr();
#else
    __asm{
        mov nStackCurretnAddr esp;
    }
#endif
    if(nullptr == t_pThreadStackBaseAddr) {
        MEMORY_BASIC_INFORMATION mbi;
        VirtualQuery((PVOID)&mbi, &mbi, sizeof(mbi));
        t_pThreadStackBaseAddr = mbi.AllocationBase;
        printf("tmp:%p, pageBaseAdrr:%p, stackBaseAddr:%p, :%lld\n", &mbi, mbi.BaseAddress, mbi.AllocationBase, mbi.RegionSize);
    }
#endif

    if( nullptr == t_pThreadStackBaseAddr)  {
        return 0;
    }

    return (size_t) &nStackCurretnAddr - (size_t)t_pThreadStackBaseAddr;

}
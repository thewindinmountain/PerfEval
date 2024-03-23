#include<time.h>
#include<stdio.h>
#include <asmjit/asmjit.h>

// Signature of the generated function.
typedef int (*SumFunc)(size_t count);      //我们构造的核心循环函数的定义

using namespace asmjit;


int main()
{

    for(int nop_num=0; nop_num<800;nop_num+=1){

        JitRuntime rt;                    // Create a runtime specialized for JIT.
        CodeHolder code;                  // Create a CodeHolder.

        code.init(rt.environment());      // Initialize code to match the JIT environment.
        x86::Assembler a(&code);          // Create and attach x86::Assembler to code.

        x86::Gp cnt;

        if (ASMJIT_ARCH_BITS == 64) {
        #if defined(_WIN32)
          cnt = x86::rdx;                 
        #else
          cnt = x86::rdi;                 
        #endif
        }
        else {
          cnt = x86::ecx;                 // Use ECX to hold the counter.
          // Fetch first and second arguments from [ESP + 4] and [ESP + 8].
          a.mov(cnt, x86::ptr(x86::esp, 4));
        }

        Label Loop = a.newLabel();        // 申明1个标签——Loop 用于循环跳转，即汇编中的label
        a.finit();                        // 浮点栈初始化，一定不能遗漏！否则会导致fsqrt指令执行报错
        a.fldpi();                        // 给浮点栈顶载入1个数值pi，用于fsqrt指令的输入值
        //以上两条指令防止在Loop外因为初始化仅需进行1次

        a.bind(Loop);                     // 将Loop标签放置在这个位置，也就是核心循环的入口处
    
        a.fsqrt();                        // 放入8条fsqrt指令，调用1次，放置1条
        a.fsqrt();
        a.fsqrt();
        a.fsqrt();
        a.fsqrt();
        a.fsqrt();
//        a.fsqrt();
//        a.fsqrt();

        for(int i=0; i<nop_num; i++){    // 根据nop_num,生成相应数量的nop指令
           a.nop();
        }

        a.dec(cnt);                       // cnt接受了我们输入的循环次数，将它减1。dec就是decline指令
        a.jnz(Loop);                      // 如果减1的结果不为0，就回到标签Loop处继续执行。jnz即jump if not zero
        a.ret();                          // 从函数中return，如果cnt-1的结果为0就会执行到此处。ret即return

        SumFunc fn;
        Error err = rt.add(&fn, &code);   // Add the generated code to the runtime.
        if (err) return 1;                // Handle a possible error returned by AsmJit.
        // ----> CodeHolder is no longer needed from here and can be destroyed <----

        struct timespec begin,end;
        clock_gettime(CLOCK_REALTIME, &begin);
        fn(200000);                       // 执行我们构造的函数fn
        clock_gettime(CLOCK_REALTIME, &end);
        printf("%f\n", ((end.tv_sec - begin.tv_sec) * 1.0e9 + end.tv_nsec - begin.tv_nsec)/200000);

        rt.release(fn);                   // Explicitly remove the function from the runtime
        code.reset();
     }

    return 0;
}
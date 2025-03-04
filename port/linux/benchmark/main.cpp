#include <benchmark/benchmark.h>
#include "test_common.h"

extern "C" {
#include "PikaMain.h"
#include "PikaParser.h"
#include "PikaStdLib_MemChecker.h"
#include "PikaVM.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataStrs.h"
#include "pikaScript.h"
}

static void for_loop_10000(benchmark::State& state) {
    for (auto _ : state) {
        PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
        /* run */
        pikaVM_run(pikaMain, (char *)
            "a = 0\n"
            "for i in range(0, 10000):\n"
            "    a = a + 1\n"
            "\n");
        obj_deinit(pikaMain);
    }
}
BENCHMARK(for_loop_10000)->Unit(benchmark::kMillisecond);

static void while_loop_10000(benchmark::State& state) {
    for (auto _ : state) {
        PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
        /* run */
        pikaVM_run(pikaMain, (char *)
            "i = 0\n"
            "while i < 10000:\n"
            "    i = i + 1\n"
            "\n");
        obj_deinit(pikaMain);
    }
}
BENCHMARK(while_loop_10000)->Unit(benchmark::kMillisecond);

void __platform_printf(char* fmt, ...) {}
static void for_print_1000(benchmark::State& state) {
    Args* buffs = New_strBuff();
    char* pikaAsm = pika_lines2Asm(buffs, (char*)
            "for i in range(1000):\n"
            "    print(i)\n"
            "\n");
    ByteCodeFrame bytecode_frame;
    byteCodeFrame_init(&bytecode_frame);
    byteCodeFrame_appendFromAsm(&bytecode_frame, pikaAsm);
    for (auto _ : state) {
        PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
        /* run */
        pikaVM_runByteCodeFrame(pikaMain, &bytecode_frame);
        obj_deinit(pikaMain);
    }
    byteCodeFrame_deinit(&bytecode_frame);
    args_deinit(buffs);
}
BENCHMARK(for_print_1000)->Unit(benchmark::kMillisecond);

static void prime_number_100(benchmark::State& state) {
    int num = 0;
    Args* buffs = New_strBuff();
    char* pikaAsm = pika_lines2Asm(buffs, (char*)
            "num = 0\n"
	        "i = 2\n"
            "while i < 100:\n"
            "    is_prime = 1\n"
	        "    j = 2\n"
            "    while j < i:\n"
            "        if i%j==0 :\n"
            "            is_prime = 0\n"
            "            break\n"
	        "        j += 1 \n"
            "    if is_prime:\n"
            "        num = num + i\n"
	        "    i += 1\n"
            "\n");
    ByteCodeFrame bytecode_frame;
    byteCodeFrame_init(&bytecode_frame);
    byteCodeFrame_appendFromAsm(&bytecode_frame, pikaAsm);
    for (auto _ : state) {
        PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
        /* run */
        pikaVM_runByteCodeFrame(pikaMain, &bytecode_frame);
        num = obj_getInt(pikaMain, (char*)"num");
        if (1060 != num) {
            printf("Error: prime_number_100\r\n");
        }
        obj_deinit(pikaMain);
    }
    byteCodeFrame_deinit(&bytecode_frame);
    args_deinit(buffs);
}
BENCHMARK(prime_number_100)->Unit(benchmark::kMillisecond);

static void prime_number_100_c(benchmark::State& state) {
    int num = 0;
    for (auto _ : state) {
        num = 0;
        /* run */
        for (int i = 2; i < 100; i++) {
            int is_prime = 1;
            for (int j = 2; j < i; j++) {
                if (i % j == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                num = num + i;
            }
        }
        if (1060 != num) {
            printf("Error: prime_number_100\r\n");
        }
    }
}
BENCHMARK(prime_number_100_c)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

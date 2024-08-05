#include "util.h"
#include <exception>
#include <cassert>

using namespace std;

static int gThrowExceptionCnt = 0;
static int gCatchExceptionCnt = 0;

struct TestException : public exception {
    TestException() {
        gThrowExceptionCnt++;
    }
    virtual const char * what () const _NOEXCEPT {
        return "Test Exception!";
    }

    void check_result() {
        gCatchExceptionCnt++;
    }
};

// increase call stack depth
void __attribute__((noinline)) ThrowExceptionCallStub() {
    throw TestException();
}

// increase call stack depth
void __attribute__((noinline)) TopCallStub() {
    ThrowExceptionCallStub();
}

static void Bm_function_throw_exception(benchmark::State &state)
{
    int ret;
    for (auto _ : state) {
        try {
            TopCallStub();
        }
        catch (TestException& e)
        {
            e.check_result();
            ret = gThrowExceptionCnt;
        }
        benchmark::DoNotOptimize(ret);
    }
}

MUSL_BENCHMARK(Bm_function_throw_exception);

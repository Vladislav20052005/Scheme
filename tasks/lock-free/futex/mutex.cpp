#include <mutex.h>
#include <cstdint>

#if defined(__linux__)

#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>

void FutexWait(void *value, uint64_t expected_value) {
    syscall(SYS_futex, value, FUTEX_WAIT_PRIVATE, expected_value, nullptr, nullptr, 0);
}

void FutexWakeOne(void *value) {
    syscall(SYS_futex, value, FUTEX_WAKE_PRIVATE, 1, nullptr, nullptr, 0);
}

void FutexWakeAll(void *value) {
    syscall(SYS_futex, value, FUTEX_WAKE_PRIVATE, INT_MAX, nullptr, nullptr, 0);
}

#elif defined(__APPLE__)

// NOLINTBEGIN(readability-identifier-naming)
extern "C" int __ulock_wait(uint32_t operation, void *addr, uint64_t value, uint32_t timeout);
extern "C" int __ulock_wake(uint32_t operation, void *addr, uint64_t wake_value);
// NOLINTEND(readability-identifier-naming)

#define UL_COMPARE_AND_WAIT 1
#define ULF_WAKE_ALL        0x00000100

void FutexWait(void *value, uint64_t expected_value) {
    __ulock_wait(UL_COMPARE_AND_WAIT, value, expected_value, 0);
}

void FutexWakeOne(void *value) {
    __ulock_wake(UL_COMPARE_AND_WAIT, value, 0);
}

void FutexWakeAll(void *value) {
    __ulock_wake(UL_COMPARE_AND_WAIT | ULF_WAKE_ALL, value, 0);
}

#undef ULF_WAKE_ALL
#undef UL_COMPARE_AND_WAIT

#endif

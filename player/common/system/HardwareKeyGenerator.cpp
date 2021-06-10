#include "HardwareKeyGenerator.hpp"

#include "common/Utils.hpp"
#include "common/system/System.hpp"
#include "common/logger/Logging.hpp"
#include "config/AppConfig.hpp"

#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <regex>

namespace bp = boost::process;

HardwareKey HardwareKeyGenerator::generate()
{
    auto key = cpuid() + static_cast<std::string>(System::macAddress());

    return HardwareKey{Md5Hash::fromString(key)};
}

std::string HardwareKeyGenerator::cpuid()
{
    const int CPUID_BUFFER = 25;

    unsigned int eax = 0, ebx = 0, ecx = 0, edx = 0;
    nativeCpuid(&eax, &ebx, &ecx, &edx);

    char buffer[CPUID_BUFFER] = {0};
    std::sprintf(buffer, "%08x%08x%08x", ebx, edx, ecx);

    return buffer;
}

inline void HardwareKeyGenerator::nativeCpuid(unsigned int* eax,
                                              unsigned int* ebx,
                                              unsigned int* ecx,
                                              unsigned int* edx)
{
    #if defined(BOOST_ARCH_X86)
    // This gets the cpuid of the cpu, by putting 0 in the eax regisister :
    // this returns the CPU's manufacturer ID string – a 
    // twelve-character ASCII string stored in EBX, EDX, ECX (in that order). 
    // The highest basic calling parameter (largest value that EAX can be set
    // to before calling CPUID) is returned in EAX.
    // All four registers are 32-bit registers so this will return a 128 bit
    // Value
        asm volatile("cpuid" : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx) : "0"(*eax), "2"(*ecx) : "memory");
    #elif defined(BOOST_ARCH_ARM)
        #if _M_ARM64
        // For rasp pi 4:

        // To access the MIDR_EL1 in AArch64 state, read the register with:
        // MRS <Xt>, MIDR_EL1; Read Main ID Register
        // This is only 32 bits, we still need

        // To access the MIDR in AArch32 state, read the CP15 register with:
        // MRC p15, 0, <Rt>, c0, c0, 0; Read Main ID Register
            asm volatile("MRS %0, MIDR_EL1": "=<Xt>"(*ebx));
        #endif
    #endif
}


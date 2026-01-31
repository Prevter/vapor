#include <sinaps.hpp>
#include <Geode/Geode.hpp>
using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
    #include <psapi.h>
#endif

size_t baseSize() {
    static size_t size = []() -> size_t {
    #ifdef GEODE_IS_WINDOWS
        auto handle = GetModuleHandle(nullptr);
        if (!handle) return 0;

        MODULEINFO info;
        if (!GetModuleInformation(GetCurrentProcess(), handle, &info, sizeof(info)))
            return 0;

        return info.SizeOfImage;
    #elif defined(GEODE_IS_INTEL_MAC)
        return 0x980000;
    #elif defined(GEODE_IS_ARM_MAC)
        return 0x8B0000;
    #else
        #error "Unsupported platform"
    #endif
    }();
    return size;
}

$execute {
    auto base = reinterpret_cast<uint8_t*>(base::get());
    auto size = baseSize();

    auto offset = sinaps::find<
        GEODE_WINDOWS("48 8B D8 ^ FF 15 ? ? ? ? 85 C0 74 11")
        GEODE_INTEL_MAC("E8 ? ? ? ? 48 89 C7 ^ E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 4C 8D 73")
        GEODE_ARM_MAC("22 00 80 52 ? ? ? ? ? ? ? ? ^ ? ? ? ? ? ? 00 36")
    >(base, size);

    if (offset == sinaps::not_found)
        return log::error("Failed to find address");

    auto addr = reinterpret_cast<void*>(base + offset);
    std::vector<uint8_t> bytes = {
        GEODE_WINDOWS(0xe9, 0x8a, 0x00, 0x00, 0x00)
        GEODE_INTEL_MAC(0xb0, 0x01, 0x90, 0x90, 0x90)
        GEODE_ARM_MAC(0x1F, 0x20, 0x03, 0xD5, 0x1F, 0x20, 0x03, 0xD5)
    };

    if (!Mod::get()->patch(addr, std::move(bytes))) {
        log::error("Failed to patch");
    }
}

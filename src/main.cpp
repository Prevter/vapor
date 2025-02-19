#include <Geode/Geode.hpp>
using namespace geode::prelude;

$on_mod(Loaded) {
    auto base = base::get();
    auto res = Mod::get()->patch(
    GEODE_WINDOWS(
        reinterpret_cast<void*>(base + 0x4b6d54),
        { 0xe9, 0x8a, 0x00, 0x00, 0x00 }
    )
    GEODE_INTEL_MAC(
        reinterpret_cast<void*>(base + 0x7dc3),
        {
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90
        }
    )
    GEODE_ARM_MAC(
        reinterpret_cast<void*>(base + 0x9ac8),
        {
            0x1F, 0x20, 0x03, 0xD5,
            0x1F, 0x20, 0x03, 0xD5
        }
    )
    );
    if (!res) { log::error("Failed to patch"); }
}
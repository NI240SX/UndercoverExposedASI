#pragma once
#include <cstdint>
namespace utils
{
	template <typename t> void set(std::uint32_t address, t value)
	{
		DWORD old;
		auto size = sizeof(value);
		VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &old);
		*reinterpret_cast<t*>(address) = value;
		VirtualProtect((LPVOID)address, size, old, &old);
	}

	template <typename t> void jump(std::uint32_t address, t function)
	{
		set<std::uint8_t>(address, 0xE9);
		set<std::uint32_t>(address + 1, std::uint32_t(function) - address - 5);
	}
}
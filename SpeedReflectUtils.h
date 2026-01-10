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

	// custom from here \/
	template <typename t> void call(std::uint32_t address, t function)
	{
		set<std::uint8_t>(address, 0xE8);
		set<std::uint32_t>(address + 1, std::uint32_t(function) - address - 5);
	}

	void nop(std::uint32_t address, uint32_t length) {
		for (int i = 0; i < length; i++) {
			set<char>(address + i, 0x90);
		}
	}

	template <typename t> void setFun(std::uint32_t address, t function)
	{
		set<std::uint32_t>(address, std::uint32_t(function));
	}

	template <typename t> t get(std::uint32_t address) {
		DWORD old;
		auto size = sizeof(t);
		VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &old);
		t ret = *reinterpret_cast<t*>(address);
		VirtualProtect((LPVOID)address, size, old, &old);
		return ret;
	}
}
#pragma once
#include <cstdint>
namespace utils
{
	/**
	 * Replace existing code data with new one
	 *
	 * @param  address:		the address to edit
	 * @param  value:		the value to inject
	 *
	 * @return the previous value
	 */
	template <typename t> t set(std::uint32_t address, t value)
	{
		DWORD old;
		t oldValue;
		auto size = sizeof(value);
		VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &old);
		oldValue = *reinterpret_cast<t*>(address);
		*reinterpret_cast<t*>(address) = value;
		VirtualProtect((LPVOID)address, size, old, &old);

		return oldValue;
	}

	/**
	 * Make a jump from existing code to a function
	 *
	 * @param  address:		the address to jump from
	 * @param  function:	the function to jump to
	 *
	 * @return the previously called function
	 */
	template <typename t> t jump(std::uint32_t address, t function)
	{
		set<std::uint8_t>(address, 0xE9);
		return (t)(set<std::uint32_t>(address + 1, std::uint32_t(function) - address - 5) + address + 5);
		// value +address +5 = std::uint32_t(function)
	}

	/**
	 * Make a call from existing code to a function
	 *
	 * @param  address:		the address to call from
	 * @param  function:	the function to call
	 *
	 * @return the previously called function
	 */
	template <typename t> t call(std::uint32_t address, t function)
	{
		set<std::uint8_t>(address, 0xE8);
		return (t)(set<std::uint32_t>(address + 1, std::uint32_t(function) - address - 5) + address + 5);
		//		set<std::uint32_t>(address + 1, std::uint32_t(function) - address - 5);
	}

	/**
	 * Fill with NOPs
	 *
	 * @param  address:		the address to start filling from
	 * @param  length:		the number of bytes to NOP
	 */
	void nop(std::uint32_t address, uint32_t length) {
		for (int i = 0; i < length; i++) {
			set<char>(address + i, 0x90);
		}
	}

	/**
	 * Set a function address in code
	 *
	 * @param  address:		the address to set
	 * @param  function:	the function to be set
	 *
	 * @return the previously called function
	 */
	//template <typename t> void setFun(std::uint32_t address, t function)
	//{
	//	set<std::uint32_t>(address, std::uint32_t(function));
	//}

	template <typename t> t get(std::uint32_t address) {
		DWORD old;
		auto size = sizeof(t);
		VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &old);
		t ret = *reinterpret_cast<t*>(address);
		VirtualProtect((LPVOID)address, size, old, &old);
		return ret;
	}
}
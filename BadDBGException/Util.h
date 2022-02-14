#pragma once

#include <ntifs.h> 
#include <minwindef.h> 
#include <ntstrsafe.h>
#include <ntimage.h>
#include <cstdint>
#include <intrin.h>

#define Log(x,...)  Util::Print(x, __VA_ARGS__)
namespace Util 
{

	PVOID GetNtoskrnlbase()
	{
		typedef unsigned char uint8_t;
		auto Idt_base = reinterpret_cast<uintptr_t>(KeGetPcr()->IdtBase);
		auto align_page = *reinterpret_cast<uintptr_t*>(Idt_base + 4) >> 0xc << 0xc;

		for (; align_page; align_page -= PAGE_SIZE)
		{
			for (int index = 0; index < PAGE_SIZE - 0x7; index++)
			{
				auto current_address = static_cast<intptr_t>(align_page) + index;

				/*
				Windows 7 - 11 support
				VfPowerDumpIrpStack sig:
				48 8D 3D ? ? ? FF 48 8B 8C ? ? ? ? ? E8
				*/
				if (*reinterpret_cast<uint8_t*>(current_address) == 0x48
					&& *reinterpret_cast<uint8_t*>(current_address + 1) == 0x8D
					&& *reinterpret_cast<uint8_t*>(current_address + 2) == 0x3D
					&& *reinterpret_cast<uint8_t*>(current_address + 6) == 0xFF
					&& *reinterpret_cast<uint8_t*>(current_address + 7) == 0x48
					&& *reinterpret_cast<uint8_t*>(current_address + 8) == 0x8B
					&& *reinterpret_cast<uint8_t*>(current_address + 9) == 0x8C
					&& *reinterpret_cast<uint8_t*>(current_address + 15) == 0xE8

					)
				{

					auto nto_base_offset = *reinterpret_cast<int*>(current_address + 3);
					auto nto_base_ = (current_address + nto_base_offset + 7);
					if (!(nto_base_ & 0xfff))
					{
						return (PVOID)nto_base_;
					}
				}
			}
		}

		return NULL;
	}

	void disable_wp()
	{
		ULONG_PTR dwCr0 = __readcr0();
		dwCr0 &= 0xFFFEFFFF;
		__writecr0(dwCr0);
	}

	void enable_wp()
	{
		ULONG_PTR dwCr0 = __readcr0();
		dwCr0 |= 0x00010000;
		__writecr0(dwCr0);
	}

	__forceinline  ULONG Print(const char* text, ...)
	{

		va_list(args);
		va_start(args, text);


		auto result = vDbgPrintExWithPrefix("[BadDBGException] ", 0, 0, text, args);

		va_end(args);
		return result;

	}


	__forceinline bool  CheckMask(const char* base, const char* pattern, const char* mask)
	{
		for (; *mask; ++base, ++pattern, ++mask)
		{
			if ('x' == *mask && *base != *pattern)
			{
				return false;
			}
		}

		return true;
	}


	__forceinline PVOID FindPattern(PVOID base, int length, const char* pattern, const char* mask)
	{
		length -= static_cast<int>(strlen(mask));
		for (auto i = 0; i <= length; ++i)
		{
			const auto* data = static_cast<char*>(base);
			const auto* address = &data[i];
			if (CheckMask(address, pattern, mask))
				return PVOID(address);
		}

		return nullptr;
	}

	__forceinline PVOID FindPatternImage(PVOID base, const char* secthionName, const char* pattern, const char* mask)
	{
		PVOID match = nullptr;

		auto* headers = reinterpret_cast<PIMAGE_NT_HEADERS>(static_cast<char*>(base) + static_cast<PIMAGE_DOS_HEADER>(base)->e_lfanew);
		auto* sections = IMAGE_FIRST_SECTION(headers);

		for (auto i = 0; i < headers->FileHeader.NumberOfSections; ++i)
		{
			auto* section = &sections[i];
			if (memcmp(section->Name, secthionName, strlen(secthionName)) == 0)
			{
				match = FindPattern(static_cast<char*>(base) + section->VirtualAddress, section->Misc.VirtualSize, pattern, mask);
				if (match)
					break;
			}
		}

		return match;
	}



}

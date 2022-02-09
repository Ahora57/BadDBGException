#include "Util.h"


/*

Alert! After some time it trigger BSOD with CRITICAL_STRUCTURE_CORRUPTION  by PatchGuard(we change code)
*/
NTSTATUS DriverEntry(PVOID baseNtoskrnl)
{


	//set base address if  driver parameter = NULL
	if (!baseNtoskrnl)
	{
		baseNtoskrnl = Util::GetNtoskrnlbase();
	}

	/*
DbgkForwardException:


windows 8.1:

GE:000000014047F416 65 48 8B 04 25 88 01 00 00                                      mov     rax, gs:188h
PAGE:000000014047F41F 45 8A E8                                                        mov     r13b, r8b
PAGE:000000014047F422 40 8A FA                                                        mov     dil, dl
PAGE:000000014047F425 48 8B B0 B8 00 00 00                                            mov     rsi, [rax+0B8h]
:000000014047F42C 4C 8B F1                                                        mov     r14, rcx
PAGE:000000014047F42F 41 BC 01 00 00 00                                               mov     r12d, 1
PAGE:000000014047F435 84 D2                                                           test    dl, dl
PAGE:000000014047F437 0F 84 DB B2 0F 00                                               jz      loc_14057A718
PAGE:000000014047F43D 65 48 8B 04 25 88 01 00 00                                      mov     rax, gs:188h
PAGE:000000014047F446 8B 88 B4 06 00 00                                               mov     ecx, [rax+6B4h]
PAGE:000000014047F44C F6 C1 04                                                        test    cl, 4
PAGE:000000014047F44F 75 4B                                                           jnz     short loc_14047F49C
PAGE:000000014047F451 48 8B 9E 10 04 00 00                                            mov     rbx, [rsi+410h]

windows 10:

PAGE:0000000140720036 83 64 24 58 00                                                  and     [rsp+170h+var_118], 0
PAGE:000000014072003B C7 44 24 30 A8 00 D0 00                                         mov     [rsp+170h+var_140], 0D000A8h
PAGE:0000000140720043 C7 44 24 34 08 00 00 00                                         mov     [rsp+170h+var_13C], 8
PAGE:000000014072004B 65 48 8B 04 25 88 01 00 00                                      mov     rax, gs:188h
PAGE:0000000140720054 48 8B B0 B8 00 00 00                                            mov     rsi, [rax+0B8h]
PAGE:000000014072005B 40 84 FF                                                        test    dil, dil
PAGE:000000014072005E 0F 84 20 3B 11 00                                               jz      loc_140833B84
PAGE:0000000140720064 65 48 8B 04 25 88 01 00 00                                      mov     rax, gs:188h
PAGE:000000014072006D 8B 88 10 05 00 00                                               mov     ecx, [rax+510h]
PAGE:0000000140720073 F6 C1 04                                                        test    cl, 4
PAGE:0000000140720076 75 47                                                           jnz     short loc_1407200BF
PAGE:0000000140720078 48 8B 9E 78 05 00 00                                            mov     rbx, [rsi+578h]

problem windows 10 build :

PAGE:00000001404F71C4 83 64 24 58 00                                                  and     [rsp+170h+var_118], 0
PAGE:00000001404F71C9 C7 44 24 30 A8 00 D0 00                                         mov     [rsp+170h+var_140], 0D000A8h
PAGE:00000001404F71D1 C7 44 24 34 08 00 00 00                                         mov     [rsp+170h+var_13C], 8
PAGE:00000001404F71D9 65 48 8B 04 25 88 01 00 00                                      mov     rax, gs:188h
PAGE:00000001404F71E2 48 8B B0 B8 00 00 00                                            mov     rsi, [rax+0B8h]
PAGE:00000001404F71E9 40 84 FF                                                        test    dil, dil
PAGE:00000001404F71EC 74 24                                                           jz      short loc_1404F7212
PAGE:00000001404F71EE 65 48 8B 04 25 88 01 00 00                                      mov     rax, gs:188h
PAGE:00000001404F71F7 8B 88 C0 06 00 00                                               mov     ecx, [rax+6C0h]
PAGE:00000001404F71FD F6 C1 04                                                        test    cl, 4
PAGE:00000001404F7200 74 04                                                           jz      short loc_1404F7206
PAGE:00000001404F7202 33 DB                                                           xor     ebx, ebx
PAGE:00000001404F7204 EB 07                                                           jmp     short loc_1404F720D


65 48 8B 04 25 88 ? ? ?  8B 88 ? ? ? ? F6 C1 04 

windows 11:


PAGE:00000001406C5BA2 83 64 24 58 00                                                  and     [rsp+170h+var_118], 0
PAGE:00000001406C5BA7 C7 44 24 30 A8 00 D0 00                                         mov     [rsp+170h+var_140], 0D000A8h
PAGE:00000001406C5BAF C7 44 24 34 08 00 00 00                                         mov     [rsp+170h+var_13C], 8
PAGE:00000001406C5BB7 65 48 8B 04 25 88 01 00 00                                      mov     rax, gs:188h
PAGE:00000001406C5BC0 48 8B B0 B8 00 00 00                                            mov     rsi, [rax+0B8h]
PAGE:00000001406C5BC7 40 84 FF                                                        test    dil, dil
PAGE:00000001406C5BCA 0F 84 A0 63 1C 00                                               jz      loc_14088BF70
PAGE:00000001406C5BD0 65 48 8B 04 25 88 01 00 00                                      mov     rax, gs:188h
PAGE:00000001406C5BD9 8B 88 60 05 00 00                                               mov     ecx, [rax+560h]
PAGE:00000001406C5BDF F6 C1 04                                                        test    cl, 4
PAGE:00000001406C5BE2 75 47                                                           jnz     short loc_1406C5C2B
PAGE:00000001406C5BE4 48 8B 9E 78 05 00 00                                            mov     rbx, [rsi+578h]


pattern: 

65 48 8B 04 25 88 ? ? ?  8B 88 ? ? ? ? F6 C1 04 


patch 
75 -> EB  aka jnz -> jmp

74 04  ->      nop nop 
	
	
	*/



	auto DbgkForwardExceptionPattern = (uint64_t)Util::FindPatternImage(
		baseNtoskrnl, 
		"PAGE", 
		"\x65\x48\x8B\x04\x25\x88\x00\x00\x00\x8B\x88\x00\x00\x00\x00\xF6\xC1\x04",
		"xxxxxx???xx????xxx");


	
	if (DbgkForwardExceptionPattern)
	{
		for (size_t i = 0;  *(uint8_t*)(DbgkForwardExceptionPattern +i ) != 0xC3; i++) // check on return for present false set
		{

			if (*(uint8_t*)(DbgkForwardExceptionPattern + i) == 0x75)
			{
				Log("Address ->\t%x\n", DbgkForwardExceptionPattern);
				
				_disable();
				Util::disable_wp();

				*(uint8_t*)(DbgkForwardExceptionPattern + i) = 0xEB;// jnz -> jmp

				Util::enable_wp();
				_enable();
				break;
				
			}
			else if(*(uint8_t*)(DbgkForwardExceptionPattern + i) == 0x74)
			{
				Log("Address ->\t%x\n", DbgkForwardExceptionPattern);
				
				_disable();
				Util::disable_wp();

				*(uint8_t*)(DbgkForwardExceptionPattern + i) = 0x90;  // set 2 nop 
				*(uint8_t*)(DbgkForwardExceptionPattern + i + 1 ) = 0x90;

				Util::enable_wp();
				_enable();
				break;
				
			}

		}
	}
	else
	{
		Log("Can't find pattern!\n");
	}


	return STATUS_SUCCESS;
}
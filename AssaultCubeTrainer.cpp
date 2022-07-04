// AssaultCubeTrainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"


int main()
{
	//Get ProcId of target process
	DWORD procId = GetProcId(L"ac_client.exe");

	//Getmodulebaseaddress
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

	//Get Handle to Process
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	// Resolve base address of the pointer chain
	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x17E0A8;

	std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

	//Resolve ammo n health pointer chain
	std::vector<unsigned int> ammoOffsets = { 0x368, 0x14, 0x0 };
	std::vector<unsigned int> healthOffsets = { 0xEC };
	std::vector<unsigned int> recoil1Offsets = { 0x364, 0xC, 0x5E };
	std::vector<unsigned int> recoil2Offsets = { 0x364, 0xC, 0x60 };


	uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);
	uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, healthOffsets);
	uintptr_t recoilAddr1 = FindDMAAddy(hProcess, dynamicPtrBaseAddr, recoil1Offsets);
	uintptr_t recoilAddr2 = FindDMAAddy(hProcess, dynamicPtrBaseAddr, recoil2Offsets);



	std::cout << "ammoAddr = " << "0x" << std::hex << ammoAddr << std::endl;
	std::cout << "healthAddr = " << "0x" << std::hex << healthAddr << std::endl;
	std::cout << "recoilAddr1 = " << "0x" << std::hex << recoilAddr1 << std::endl;
	std::cout << "recoilAddr2 = " << "0x" << std::hex << recoilAddr2 << std::endl;



	//Read ammo n health value
	int ammoValue = 0;
	int healthValue = 0;
	int recoilValue1 = 0;
	int recoilValue2 = 0;
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
	ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);
	ReadProcessMemory(hProcess, (BYTE*)recoilAddr1, &recoilValue1, sizeof(recoilValue1), nullptr);
	ReadProcessMemory(hProcess, (BYTE*)recoilAddr2, &recoilValue2, sizeof(recoilValue2), nullptr);


	std::cout << "Current Ammo = " << std::dec << ammoValue << std::endl;
	std::cout << "Current Health = " << std::dec << healthValue << std::endl;


	//write to it
	int newAmmo = 0;
	int newHealth = 0;
	int newRecoil1 = 0;
	int newRecoil2 = 0;
	std::cout << "Input new ammo" << std::endl;
	std::cin >> newAmmo;
	WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

	std::cout << "we detected you were a minority, so we automatically toggled norecoil. you dont have a choice :3" << std::endl;
	WriteProcessMemory(hProcess, (BYTE*)recoilAddr1, &newRecoil1, sizeof(newRecoil1), nullptr);
	WriteProcessMemory(hProcess, (BYTE*)recoilAddr2, &newRecoil2, sizeof(newRecoil2), nullptr);



	std::cout << "Input new health" << std::endl;
	std::cin >> newHealth;
	WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);

	//read again
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
	std::cout << "New Ammo = " << std::dec << ammoValue << std::endl;

	WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);
	std::cout << "New Health = " << std::dec << healthValue << std::endl;


	getchar();
	return 0;
}



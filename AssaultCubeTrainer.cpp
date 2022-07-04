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
	uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);
	uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, healthOffsets);


	std::cout << "ammoAddr = " << "0x" << std::hex << ammoAddr << std::endl;
	std::cout << "healthAddr = " << "0x" << std::hex << healthAddr << std::endl;


	//Read ammo n health value
	int ammoValue = 0;
	int healthValue = 0;
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
	ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);

	std::cout << "Current Ammo = " << std::dec << ammoValue << std::endl;
	std::cout << "Current Health = " << std::dec << healthValue << std::endl;


	//write to it
	int newAmmo = 0;
	int newHealth = 0;
	std::cout << "Input new ammo" << std::endl;
	std::cin >> newAmmo;
	WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

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



#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include<cstdint>
#include <thread>

namespace offset
{
	constexpr ::std::ptrdiff_t dwForceJump = 0x52B7BDC;
	constexpr ::std::ptrdiff_t m_fFlags = 0x104;
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDE6964;
	constexpr ::std::ptrdiff_t m_iHealth = 0x100;
}



//hack fn
void BunnyHop(const HMODULE instance) noexcept
{
	const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll"));

		//hack loop
		while (!GetAsyncKeyState(VK_END))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			if (!GetAsyncKeyState(VK_SPACE))
				continue;

			// get local player
			const auto localPlayer = *reinterpret_cast<std::uintptr_t*>(client + offset::dwLocalPlayer);

			if (!localPlayer)
				continue;

			const auto health = *reinterpret_cast<std::int32_t*>(localPlayer + offset::m_iHealth);

			// is alive
			if (!health)
				continue;

			const auto flags = *reinterpret_cast<std::int32_t*>(localPlayer + offset::m_fFlags);

			(flags & (1 << 0)) ?
				*reinterpret_cast<std::uintptr_t*>(client + offset::dwForceJump) = 6 :
				*reinterpret_cast<std::uintptr_t*>(client + offset::dwForceJump) = 4;

		}
	//uninject
	FreeLibraryAndExitThread(instance, 0);
}

//entry point
int __stdcall DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)


{
	if (reason == 1)
	{
		DisableThreadLibraryCalls(instance);

		// create hack thread
		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(BunnyHop),
			instance,
			0,
			nullptr
		);

		if(thread)
			CloseHandle(thread);

	}
	
	
	return 1;
} 
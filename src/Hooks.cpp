#include "Hooks.h"
#include "Manager.h"
#include "Settings.h"

namespace Hooks
{
	class MeleeDamage
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(42832, 44001), REL::VariantOffset(0x396, 0x37A, 0x3E9) };
			auto& trampoline = SKSE::GetTrampoline();
			originalFunction = trampoline.write_branch<5>(target.address(), Thunk);
		}

	private:
		static void Thunk(RE::HitData* a_hitData)
		{
			if (Settings::GetSingleton()->isEnabled) {
				Manager::HitDamage::ApplyScaling(a_hitData);
			}

			originalFunction(a_hitData);	
		}

		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class ArrowDamage
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(42833, 44002), REL::VariantOffset(0x163, 0x163, 0x163) };
			auto& trampoline = SKSE::GetTrampoline();
			originalFunction = trampoline.write_branch<5>(target.address(), Thunk);
		}

	private:
		static void Thunk(RE::HitData* a_hitData)
		{
			if (Settings::GetSingleton()->isEnabled) {
				Manager::HitDamage::ApplyScaling(a_hitData);
			}

			originalFunction(a_hitData);
		}

		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class AbstractDamage
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(42834, 44003), REL::VariantOffset(0x8D, 0x8D, 0x8D) };
			auto& trampoline = SKSE::GetTrampoline();
			originalFunction = trampoline.write_branch<5>(target.address(), Thunk);
		}

	private:
		static void Thunk(RE::HitData* a_hitData)
		{
			if (Settings::GetSingleton()->isEnabled) {
				Manager::HitDamage::ApplyScaling(a_hitData);
			}

			originalFunction(a_hitData);
		}

		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class ExplosionDamage
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(42835, 44004), REL::VariantOffset(0xD5, 0xDD, 0xD5) };
			auto& trampoline = SKSE::GetTrampoline();
			originalFunction = trampoline.write_branch<5>(target.address(), Thunk);
		}

	private:
		static void Thunk(RE::HitData* a_hitData)
		{
			if (Settings::GetSingleton()->isEnabled) {
				Manager::HitDamage::ApplyScaling(a_hitData);
			}

			originalFunction(a_hitData);
		}

		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class CollisionDamage
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(42836, 44005), REL::VariantOffset(0x17E, 0x178, 0x17E) };
			auto& trampoline = SKSE::GetTrampoline();
			originalFunction = trampoline.write_branch<5>(target.address(), Thunk);
		}

	private:
		static void Thunk(RE::HitData* a_hitData)
		{
			if (Settings::GetSingleton()->isEnabled) {
				Manager::HitDamage::ApplyScaling(a_hitData);
			}

			originalFunction(a_hitData);
		}

		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class MagicDamage
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(33763, 34547), REL::VariantOffset(0x52F, 0x7B1, 0x4B1) };
			auto& trampoline = SKSE::GetTrampoline();
			originalFunction = trampoline.write_call<5>(target.address(), Thunk);
		}

	private:
		static void Thunk(void* a_unk1, RE::ActiveEffect* a_activeEffect, void* a_unk3, void* a_unk4, void* a_unk5)
		{
			if (Settings::GetSingleton()->isEnabled) {
				Manager::MagicDamage::ApplyScaling(a_activeEffect);
			}

			originalFunction(a_unk1, a_activeEffect, a_unk3, a_unk4, a_unk5);
		}

		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class ReflectDamage
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(37673, 38627), REL::VariantOffset(0x3DC, 0x4C4, 0x3DC) };
			auto& trampoline = SKSE::GetTrampoline();
			originalFunction = trampoline.write_call<5>(target.address(), Thunk);
		}

	private:
		static bool Thunk(RE::Actor* a_target, float a_healthDamage, RE::Actor* a_source, bool a_dontAdjustDifficulty)
		{
			if (Settings::GetSingleton()->isEnabled) {
				a_healthDamage = Manager::ReflectDamage::ApplyScaling(a_source, a_target, a_healthDamage);
			}

			return originalFunction(a_target, a_healthDamage, a_source, a_dontAdjustDifficulty);
		}

		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class HandleBeenAttacked
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(33559, 34335), REL::VariantOffset(0x19A, 0x19A, 0x1A4) };
			REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(36016, 36991), REL::VariantOffset(0x1E91, 0x23DE, 0x1EED) };
			REL::Relocation<std::uintptr_t> target3{ RELOCATION_ID(37633, 38586), REL::VariantOffset(0xE39, 0x1062, 0xE62) };

			auto& trampoline = SKSE::GetTrampoline();
			originalFunction = trampoline.write_call<5>(target1.address(), Thunk);
			originalFunction = trampoline.write_call<5>(target2.address(), Thunk);
			originalFunction = trampoline.write_call<5>(target3.address(), Thunk);
		}

	private:
		static void Thunk(RE::Actor* a_victim, RE::Actor* a_aggressor, void* a_unk3, std::uint32_t a_unk4)
		{
			auto settings = Settings::GetSingleton();
			if (settings->isEnabled && settings->isIgnoreAttack) {
				if (Manager::HandleBeenAttacked::CanIgnoreAttack(a_aggressor, a_victim)) {
					return;
				}
			}

			originalFunction(a_victim, a_aggressor, a_unk3, a_unk4);
		}

		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	void Install()
	{
		// Don't need: trap damage, environmental damage
		logger::info("Installing hooks...");
		SKSE::AllocTrampoline(112);
		MeleeDamage::Install();
		ArrowDamage::Install();
		AbstractDamage::Install();
		ExplosionDamage::Install();
		CollisionDamage::Install();
		MagicDamage::Install();
		ReflectDamage::Install();
		HandleBeenAttacked::Install();
		logger::info("Hooks installed successfully.");
	}
}

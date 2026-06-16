#include "Hooks.h"
#include "Manager.h"
#include "Settings.h"

namespace Hooks
{
	void Install()
	{
		// Don't need: trap damage, environmental damage
		logger::info("Installing hooks...");
		SKSE::AllocTrampoline(98);
		MeleeDamage::Install();
		ArrowDamage::Install();
		AbstractDamage::Install();
		ExplosionDamage::Install();
		CollisionDamage::Install();
		MagicDamage::Install();
		ReflectDamage::Install();
		logger::info("Hooks installed successfully.");
	}

	void MeleeDamage::Install()
	{
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(42832, 44001), REL::VariantOffset(0x396, 0x37A, 0x3E9) };
		auto& trampoline = SKSE::GetTrampoline();
		originalFunction = trampoline.write_branch<5>(target.address(), Thunk);
	}

	void MeleeDamage::Thunk(RE::HitData* a_hitData)
	{
		if (Settings::GetSingleton()->isEnabled) {
			Manager::HitDamage::ApplyScaling(a_hitData);
		}

		originalFunction(a_hitData);	
	}

	void ArrowDamage::Install()
	{
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(42833, 44002), REL::VariantOffset(0x163, 0x163, 0x163) };
		auto& trampoline = SKSE::GetTrampoline();
		originalFunction = trampoline.write_branch<5>(target.address(), Thunk);
	}

	void ArrowDamage::Thunk(RE::HitData* a_hitData)
	{
		if (Settings::GetSingleton()->isEnabled) {
			Manager::HitDamage::ApplyScaling(a_hitData);
		}

		originalFunction(a_hitData);
	}

	void AbstractDamage::Install()
	{
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(42834, 44003), REL::VariantOffset(0x8D, 0x8D, 0x8D) };
		auto& trampoline = SKSE::GetTrampoline();
		originalFunction = trampoline.write_branch<5>(target.address(), Thunk);
	}

	void AbstractDamage::Thunk(RE::HitData* a_hitData)
	{
		if (Settings::GetSingleton()->isEnabled) {
			Manager::HitDamage::ApplyScaling(a_hitData);
		}

		originalFunction(a_hitData);
	}

	void ExplosionDamage::Install()
	{
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(42835, 44004), REL::VariantOffset(0xD5, 0xDD, 0xD5) };
		auto& trampoline = SKSE::GetTrampoline();
		originalFunction = trampoline.write_branch<5>(target.address(), Thunk);
	}

	void ExplosionDamage::Thunk(RE::HitData* a_hitData)
	{
		if (Settings::GetSingleton()->isEnabled) {
			Manager::HitDamage::ApplyScaling(a_hitData);
		}

		originalFunction(a_hitData);
	}

	void CollisionDamage::Install()
	{
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(42836, 44005), REL::VariantOffset(0x17E, 0x178, 0x17E) };
		auto& trampoline = SKSE::GetTrampoline();
		originalFunction = trampoline.write_branch<5>(target.address(), Thunk);
	}

	void CollisionDamage::Thunk(RE::HitData* a_hitData)
	{
		if (Settings::GetSingleton()->isEnabled) {
			Manager::HitDamage::ApplyScaling(a_hitData);
		}

		originalFunction(a_hitData);
	}

	void MagicDamage::Install()
	{
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(33763, 34547), REL::VariantOffset(0x52F, 0x7B1, 0x4B1) };
		auto& trampoline = SKSE::GetTrampoline();
		originalFunction = trampoline.write_call<5>(target.address(), Thunk);
	}

	void MagicDamage::Thunk(void* a_unk1, RE::ActiveEffect* a_activeEffect, void* a_unk3, void* a_unk4, void* a_unk5)
	{
		if (Settings::GetSingleton()->isEnabled) {
			Manager::MagicDamage::ApplyScaling(a_activeEffect);
		}

		originalFunction(a_unk1, a_activeEffect, a_unk3, a_unk4, a_unk5);
	}

	void ReflectDamage::Install()
	{
		REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(37673, 38627), REL::VariantOffset(0x3DC, 0x4C4, 0x3DC) };
		auto& trampoline = SKSE::GetTrampoline();
		originalFunction = trampoline.write_call<5>(target.address(), Thunk);
	}

	bool ReflectDamage::Thunk(RE::Actor* a_target, float a_healthDamage, RE::Actor* a_source, bool a_dontAdjustDifficulty)
	{
		if (Settings::GetSingleton()->isEnabled) {
			a_healthDamage = Manager::ReflectDamage::ApplyScaling(a_source, a_target, a_healthDamage);
		}

		return originalFunction(a_target, a_healthDamage, a_source, a_dontAdjustDifficulty);
	}
}

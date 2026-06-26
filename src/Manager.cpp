#include "Manager.h"
#include "Configs.h"
#include "Utils.h"

namespace Manager
{
	namespace
	{
		float CalcFinalDamage(const RE::Actor* a_source, const RE::Actor* a_target, float a_damage)
		{
			if (!a_source || !a_target) {
				return a_damage;
			}

			if (a_source == a_target) {
				return a_damage;
			}

			if (a_damage < 0.1f) {
				return a_damage;
			}

			auto scaling = Configs::GetSingleton()->GetScaling(a_source, a_target);	
			return a_damage * scaling;
		}
	}

	void HitDamage::ApplyScaling(RE::HitData* a_hitData)
	{
		if (!a_hitData) {
			return;
		}

		const auto aggressor = a_hitData->aggressor.get().get();
		const auto target = a_hitData->target.get().get();
		const auto damage = a_hitData->totalDamage;
		a_hitData->totalDamage = CalcFinalDamage(aggressor, target, damage);
	}

	void MagicDamage::ApplyScaling(RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			return;
		}

		if (Utils::Game::IsDamagingMagic(a_activeEffect)) {
			const auto magicTarget = a_activeEffect->target;
			if (magicTarget && magicTarget->MagicTargetIsActor()) {
				const auto caster = a_activeEffect->GetCasterActor().get();
				const auto target = skyrim_cast<RE::Actor*>(magicTarget);
				const auto damage = -a_activeEffect->magnitude;
				a_activeEffect->magnitude = -CalcFinalDamage(caster, target, damage);
			}
		}
	}

	float ReflectDamage::ApplyScaling(RE::Actor* a_source, RE::Actor* a_target, float a_damage)
	{
		return CalcFinalDamage(a_source, a_target, a_damage);
	}

	bool HandleBeenAttacked::CanIgnoreAttack(RE::Actor* a_source, RE::Actor* a_target)
	{
		if (!a_source || !a_target) {
			return false;
		}

		auto scaling = Configs::GetSingleton()->GetScaling(a_source, a_target);
		if (scaling == 0.0f) {
			return true;
		}

		return false;
	}
}

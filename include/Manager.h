#pragma once

namespace Manager
{
	class HitDamage
	{
	public:
		static void ApplyScaling(RE::HitData* a_hitData);
	};

	class MagicDamage
	{
	public:
		static void ApplyScaling(RE::ActiveEffect* a_activeEffect);
	};

	class ReflectDamage
	{
	public:
		static float ApplyScaling(RE::Actor* a_source, RE::Actor* a_target, float a_damage);
	};
}

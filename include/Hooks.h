#pragma once

namespace Hooks
{
	void Install();

	class MeleeDamage
	{
	public:
		static void Install();

	private:
		static void Thunk(RE::HitData* a_hitData);
		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class ArrowDamage
	{
	public:
		static void Install();

	private:
		static void Thunk(RE::HitData* a_hitData);
		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class AbstractDamage
	{
	public:
		static void Install();

	private:
		static void Thunk(RE::HitData* a_hitData);
		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class ExplosionDamage
	{
	public:
		static void Install();

	private:
		static void Thunk(RE::HitData* a_hitData);
		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class CollisionDamage
	{
	public:
		static void Install();

	private:
		static void Thunk(RE::HitData* a_hitData);
		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class MagicDamage
	{
	public:
		static void Install();

	private:
		static void Thunk(void* a_unk1, RE::ActiveEffect* a_activeEffect, void* a_unk3, void* a_unk4, void* a_unk5);
		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class ReflectDamage
	{
	public:
		static void Install();

	private:
		static bool Thunk(RE::Actor* a_target, float a_healthDamage, RE::Actor* a_source, bool a_dontAdjustDifficulty);
		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};

	class HandleBeenAttacked
	{
	public:
		static void Install();

	private:
		static void Thunk(RE::Actor* a_victim, RE::Actor* a_aggressor, void* a_unk3, std::uint32_t a_unk4);
		static inline REL::Relocation<decltype(Thunk)> originalFunction;
	};
}

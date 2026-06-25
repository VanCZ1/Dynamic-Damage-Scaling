#pragma once
#include "Utils.h"

namespace Conditions
{
	enum class ConditionType : std::uint8_t
	{
		kNone,
		kAll,
		kPlayer,
		kNPC,
		kFollower,
		kID,
		kRace,
		kFaction,
		kKeyword,
		kSex,
		kLevel,
		kCommanded,

		kTotal
	};

	inline constexpr std::array ConditionNames{
		"None"sv,
		"All"sv,
		"Player"sv,
		"NPC"sv,
		"Follower"sv,
		"ID"sv,
		"Race"sv,
		"Faction"sv,
		"Keyword"sv,
		"Sex"sv,
		"Level"sv,
		"Commanded"sv
	};
	static_assert(ConditionNames.size() == static_cast<std::size_t>(ConditionType::kTotal));

	inline constexpr std::string_view GetConditionTypeName(const ConditionType a_type)
	{
		return ConditionNames.at(static_cast<int>(a_type));
	}

	template <typename T>
	constexpr ConditionType GetConditionType(const T& a_str)
	{
		const auto it = std::find(ConditionNames.begin(), ConditionNames.end(), a_str);
		return it == ConditionNames.end() ? ConditionType::kNone : static_cast<ConditionType>(std::distance(ConditionNames.begin(), it));
	}

	enum class SubConditionType : std::uint8_t
	{
		kNone,
		kAll,
		kPlayer,
		kNPC,
		kFollower,

		kTotal
	};

	inline constexpr std::array SubConditionNames{
		"None"sv,
		"All"sv,
		"Player"sv,
		"NPC"sv,
		"Follower"sv
	};
	static_assert(SubConditionNames.size() == static_cast<std::size_t>(SubConditionType::kTotal));

	inline constexpr std::string_view GetSubConditionTypeName(const SubConditionType a_type)
	{
		return SubConditionNames.at(static_cast<int>(a_type));
	}

	template <typename T>
	constexpr SubConditionType GetSubConditionType(const T& a_str)
	{
		const auto it = std::find(SubConditionNames.begin(), SubConditionNames.end(), a_str);
		return it == SubConditionNames.end() ? SubConditionType::kNone : static_cast<SubConditionType>(std::distance(SubConditionNames.begin(), it));
	}

	template <typename T>
	struct Range
	{
		T min{ std::numeric_limits<T>::min() };
		T max{ std::numeric_limits<T>::max() };

		constexpr bool IsValid() const
		{
			return min <= max;
		}

		constexpr bool Contains(T a_value) const
		{
			return a_value >= min && a_value <= max;
		}
	};

	struct Condition
	{
		ConditionType type{ ConditionType::kNone };
		SubConditionType subType{ SubConditionType::kNone };
		std::string value;
		bool negated{ false };

		RE::TESForm* formValue{ nullptr };
		RE::TESFaction* factionValue{ nullptr };
		RE::BGSKeyword* keywordValue{ nullptr };
		RE::SEX sexValue{ RE::SEX::kNone };
		Range<std::uint16_t> levelRange;
	};

	std::vector<Condition> ParseConditions(const std::string& a_entry);
	bool MatchConditions(const std::vector<Condition>& a_conditions, const RE::Actor* a_actor);
	std::string SerializeConditions(const std::vector<Condition>& a_conditions);
}

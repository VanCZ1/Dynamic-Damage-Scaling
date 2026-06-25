#include "Conditions.h"
#include "Utils.h"

namespace Conditions
{
	namespace
	{
		RE::TESForm* ResolveForm(const std::string& a_str)
		{
			bool isFormID = a_str.starts_with("0x") || a_str.starts_with("0X");
			if (isFormID) {
				try {
					if (const auto splitID = Utils::String::Split(a_str, "~"); splitID.size() == 2) {
						const RE::FormID formID = std::stoul(splitID[0], nullptr, 16);
						const std::string& modName = splitID[1];
						return Utils::Game::LookupForm(formID, modName);
					} else {
						const RE::FormID formID = std::stoul(a_str, nullptr, 16);
						return RE::TESForm::LookupByID(formID);
					}
				} catch (const std::exception& e) {
					logger::error("Invalid formID: {}, {}", a_str, e.what());
					return nullptr;
				}
			}

			return RE::TESForm::LookupByEditorID(a_str);
		}

		template <typename T>
		Range<T> ResolveRange(const std::string& a_value)
		{
			constexpr T rangeMin = std::numeric_limits<T>::min();
			constexpr T rangeMax = std::numeric_limits<T>::max();
			constexpr Range<T> rangeInvalid{ rangeMax, rangeMin };

			if (a_value.size() < 2 || a_value.front() != '(' || a_value.back() != ')') {
				logger::warn("Invalid range condition: {}", a_value);
				return rangeInvalid;
			}

			const std::string inner = a_value.substr(1, a_value.size() - 2);
			const auto splitRange = Utils::String::Split(inner, "/");
			if (splitRange.size() != 2) {
				logger::warn("Invalid range: {}", a_value);
				return rangeInvalid;
			}

			const auto toNumber = [](const std::string& str, T fallback) -> T {
				if (str.empty()) {
					return fallback;
				}
				try {
					return static_cast<T>(std::stoul(str));
				} catch (const std::exception& e) {
					logger::error("Invalid value: {}, {}", str, e.what());
					return fallback;
				}
			};

			return { toNumber(splitRange[0], rangeMin), toNumber(splitRange[1], rangeMax) };
		}

		void ParseSubCondition(Condition& a_condition, const std::string& a_value)
		{
			a_condition.subType = GetSubConditionType(a_value);
			switch (a_condition.subType) {
			case SubConditionType::kNone:
				{
					a_condition.formValue = ResolveForm(a_value);
				}
				break;
			case SubConditionType::kAll:
			case SubConditionType::kPlayer:
			case SubConditionType::kNPC:
			case SubConditionType::kFollower:
				break;
			default:
				break;
			}
		}

		bool MatchSubCondition(const Condition& a_condition, const RE::Actor* a_actor)
		{
			if (!a_actor) {
				return false;
			}

			bool result = false;

			switch (a_condition.subType) {
			case SubConditionType::kNone:
				{
					if (const auto base = a_actor->GetActorBase()) {
						result = base == a_condition.formValue;
					}
				}
				break;
			case SubConditionType::kAll:
				{
					result = true;
				}
				break;
			case SubConditionType::kPlayer:
				{
					result = a_actor->IsPlayerRef();
				}
				break;
			case SubConditionType::kNPC:
				{
					result = !a_actor->IsPlayerRef();
				}
				break;
			case SubConditionType::kFollower:
				{
					result = a_actor->IsPlayerTeammate();
				}
				break;
			default:
				break;
			}

			return result;
		}

		Condition ParseSingleCondition(std::string a_str)
		{
			Condition condition;
			if (a_str.empty()) {
				return condition;
			}

			if (a_str[0] == '-') {
				condition.negated = true;
				a_str.erase(0, 1);
			}

			std::string typeStr;
			std::string valueStr;

			const std::size_t pos = a_str.find('=');
			if (pos == std::string::npos) {
				typeStr = a_str;
			} else {
				typeStr = a_str.substr(0, pos);
				valueStr = a_str.substr(pos + 1);
			}
			condition.type = GetConditionType(typeStr);
			condition.value = valueStr;

			switch (condition.type) {
			case ConditionType::kNone:
				{
					logger::warn("Invalid condition type: {}", typeStr);
				}
				break;
			case ConditionType::kAll:
			case ConditionType::kPlayer:
			case ConditionType::kNPC:
			case ConditionType::kFollower:
				break;
			case ConditionType::kID:
			case ConditionType::kRace:
				{
					condition.formValue = ResolveForm(valueStr);
				}
				break;
			case ConditionType::kFaction:
				{
					condition.formValue = ResolveForm(valueStr);
					condition.factionValue = condition.formValue ? condition.formValue->As<RE::TESFaction>() : nullptr;
				}
				break;
			case ConditionType::kKeyword:
				{
					condition.formValue = ResolveForm(valueStr);
					condition.keywordValue = condition.formValue ? condition.formValue->As<RE::BGSKeyword>() : nullptr;
				}
				break;
			case ConditionType::kSex:
				{
					if (valueStr == "Male"sv) {
						condition.sexValue = RE::SEX::kMale;
					} else if (valueStr == "Female"sv) {
						condition.sexValue = RE::SEX::kFemale;
					}
				}
				break;
			case ConditionType::kLevel:
				{
					condition.levelRange = ResolveRange<std::uint16_t>(valueStr);
				}
				break;
			case ConditionType::kCommanded:
				{
					ParseSubCondition(condition, valueStr);
				}
				break;
			default:
				break;
			}

			return condition;
		}

		bool MatchSingleCondition(const Condition& a_condition, const RE::Actor* a_actor)
		{
			if (!a_actor) {
				return false;
			}

			bool result = false;

			switch (a_condition.type) {
			case ConditionType::kNone:
				{
					return false;
				}
				break;
			case ConditionType::kAll:
				{
					result = true;
				}
				break;
			case ConditionType::kPlayer:
				{
					result = a_actor->IsPlayerRef();
				}
				break;
			case ConditionType::kNPC:
				{
					result = !a_actor->IsPlayerRef();
				}
				break;
			case ConditionType::kFollower:
				{
					result = a_actor->IsPlayerTeammate();
				}
				break;
			case ConditionType::kID:
				{
					if (const auto base = a_actor->GetActorBase()) {
						result = base == a_condition.formValue;
					}
				}
				break;
			case ConditionType::kRace:
				{
					if (const auto race = a_actor->GetRace()) {
						result = race == a_condition.formValue;
					}
				}
				break;
			case ConditionType::kFaction:
				{
					if (a_condition.factionValue) {
						result = a_actor->IsInFaction(a_condition.factionValue);
					}
				}
				break;
			case ConditionType::kKeyword:
				{
					if (a_condition.keywordValue) {
						result = a_actor->HasKeyword(a_condition.keywordValue);
					}
				}
				break;
			case ConditionType::kSex:
				{
					if (const auto base = a_actor->GetActorBase()) {
						result = base->GetSex() == a_condition.sexValue;
					}
				}
				break;
			case ConditionType::kLevel:
				{
					const auto& range = a_condition.levelRange;
					result = range.IsValid() && range.Contains(a_actor->GetLevel());
				}
				break;
			case ConditionType::kCommanded:
				{
					const auto commander = a_actor->GetCommandingActor().get();
					if (commander) {
						result = MatchSubCondition(a_condition, commander);
					}
				}
				break;
			default:
				break;
			}

			return a_condition.negated ? !result : result;
		}

		void SerializeSingleCondition(const Condition& a_condition, std::string& a_result)
		{
			if (a_condition.negated) {
				a_result += '-';
			}

			const auto typeStr = GetConditionTypeName(a_condition.type);
			a_result += typeStr;

			if (!a_condition.value.empty()) {
				a_result += '=' + a_condition.value;
			}
		}
	}

	std::vector<Condition> ParseConditions(const std::string& a_entry)
	{
		std::vector<Condition> conditions;
		if (a_entry.empty()) {
			return conditions;
		}

		const auto parts = Utils::String::Split(a_entry, ",");
		for (const auto& part : parts) {
			if (!part.empty()) {
				conditions.push_back(ParseSingleCondition(part));
			}
		}

		return conditions;
	}

	bool MatchConditions(const std::vector<Condition>& a_conditions, const RE::Actor* a_actor)
	{
		if (a_conditions.empty()) {
			return false;
		}

		for (const auto& condition : a_conditions) {
			if (!MatchSingleCondition(condition, a_actor)) {
				return false;
			}
		}

		return true;
	}

	std::string SerializeConditions(const std::vector<Condition>& a_conditions)
	{
		if (a_conditions.empty()) {
			return "";
		}

		std::string result;
		for (const auto& condition : a_conditions) {
			if (!result.empty()) {
				result += ',';
			}
			SerializeSingleCondition(condition, result);
		}

		return result;
	}
}

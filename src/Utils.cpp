#include "Utils.h"

namespace Utils
{
	namespace String
	{
		std::string Trim(const std::string& a_str)
		{
			const auto first = a_str.find_first_not_of(" \t\r\n");
			if (first == std::string::npos) {
				return {};
			}

			const auto last = a_str.find_last_not_of(" \t\r\n");
			return a_str.substr(first, last - first + 1);
		}

		std::vector<std::string> Split(const std::string& a_str, std::string_view a_delimiter)
		{
			std::vector<std::string> result;
			if (a_str.empty()) {
				return result;
			}

			std::size_t start = 0;
			std::size_t end = a_str.find(a_delimiter);

			while (end != std::string::npos) {
				result.push_back(a_str.substr(start, end - start));
				start = end + a_delimiter.length();
				end = a_str.find(a_delimiter, start);
			}
			result.push_back(a_str.substr(start));

			return result;
		}

		std::string ToLower(std::string_view a_str)
		{
			std::string result(a_str);
			std::ranges::transform(result, result.begin(), [](const unsigned char ch) {
				return static_cast<unsigned char>(std::tolower(ch));
			});
			return result;
		}
	}

	namespace Game
	{
		RE::TESForm* LookupForm(RE::FormID a_formID, std::string_view a_modName)
		{
			const auto dataHandler = RE::TESDataHandler::GetSingleton();
			if (!dataHandler) {
				return nullptr;
			}

			if (a_modName.empty()) {
				return nullptr;
			}

			return dataHandler->LookupForm(a_formID, a_modName);
		}

		bool IsDamagingMagic(const RE::ActiveEffect* a_activeEffect)
		{
			if (!a_activeEffect) {
				return false;
			}

			const auto effect = a_activeEffect->effect;
			if (!effect) {
				return false;
			}

			const auto baseEffect = effect->baseEffect;
			if (!baseEffect) {
				return false;
			}

			if (baseEffect->HasArchetype(RE::EffectArchetype::kPeakValueModifier)) {
				return false;
			}

			if (baseEffect->data.primaryAV != RE::ActorValue::kHealth && baseEffect->data.secondaryAV != RE::ActorValue::kHealth) {
				return false;
			}

			return a_activeEffect->magnitude < 0.0f;
		}
	}
}

#pragma once

namespace Utils
{
	namespace String
	{
		[[nodiscard]] std::string Trim(const std::string& a_str);
		[[nodiscard]] std::vector<std::string> Split(const std::string& a_str, std::string_view a_delimiter);
		[[nodiscard]] std::string ToLower(std::string_view a_str);
	}
	
	namespace Game
	{
		[[nodiscard]] RE::TESForm* LookupForm(RE::FormID a_formID, std::string_view a_modName);
		[[nodiscard]] bool IsDamagingMagic(const RE::ActiveEffect* a_activeEffect);
		[[nodiscard]] int CalcFollowerCount();
	}
}

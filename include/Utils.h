#pragma once

namespace Utils
{
	namespace String
	{
		std::string Trim(const std::string& a_str);
		std::vector<std::string> Split(const std::string& a_str, std::string_view a_delimiter);
		std::string ToLower(std::string_view a_str);
	}
	
	namespace Game
	{
		RE::TESForm* LookupForm(RE::FormID a_formID, std::string_view a_modName);
		bool IsDamagingMagic(const RE::ActiveEffect* a_activeEffect);
		int CalcFollowerCount();
	}
}

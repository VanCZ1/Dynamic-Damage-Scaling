#pragma once

class Translations : public REX::Singleton<Translations>
{
public:
	void Load();
	const char* Get(std::string_view a_key);

private:
	struct StringHash
	{
		using is_transparent = void;
		std::size_t operator()(const std::string& a_str) const { return std::hash<std::string>{}(a_str); }
		std::size_t operator()(std::string_view a_str) const { return std::hash<std::string_view>{}(a_str); }
		std::size_t operator()(const char* a_str) const { return std::hash<std::string_view>{}(a_str); }
	};

	std::unordered_map<std::string, std::string, StringHash, std::equal_to<>> transTable;
	static inline constexpr auto transPath{ "Data/SKSE/Plugins/DynamicDamageScaling/Translations.json" };
};

inline const char* Trans(std::string_view a_key)
{
	return Translations::GetSingleton()->Get(a_key);
}

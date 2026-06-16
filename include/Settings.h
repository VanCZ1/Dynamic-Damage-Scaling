#pragma once

class Settings : public REX::Singleton<Settings>
{
public:
	void Load();
	void Save();
	void Reset();

	bool isEnabled{ true };

private:
	static inline constexpr auto settingPath{ "Data/SKSE/Plugins/DynamicDamageScaling/Settings.ini" };
};

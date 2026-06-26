#include "Settings.h"

void Settings::Load()
{
	CSimpleIniA ini;
	ini.SetUnicode();

	const auto rc = ini.LoadFile(settingPath);
	if (rc < 0) {
		logger::warn("Failed to load settings.");
		return;
	}

	isEnabled = ini.GetBoolValue("General", "Enabled", isEnabled);
	isIgnoreAttack = ini.GetBoolValue("General", "IgnoreAttack", isIgnoreAttack);

	logger::info("Settings loaded.");
}

void Settings::Save()
{
	CSimpleIniA ini;
	ini.SetUnicode();
	
	ini.SetBoolValue("General", "Enabled", isEnabled);
	ini.SetBoolValue("General", "IgnoreAttack", isIgnoreAttack);

	const auto rc = ini.SaveFile(settingPath);
	if (rc < 0) {
		logger::warn("Failed to save settings.");
		return;
	}

	logger::info("Settings saved.");
}

void Settings::Reset()
{
	isEnabled = true;
	isIgnoreAttack = true;
	Save();
}

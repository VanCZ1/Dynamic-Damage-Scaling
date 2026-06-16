#pragma once
#include "Conditions.h"

#include <shared_mutex>

struct ScalingRule
{
	std::vector<Conditions::Condition> source;
	std::vector<Conditions::Condition> target;
	float scaling{ 1.0f };
	std::string comment;

	std::string displaySource;
	std::string displayTarget;
};

struct ScalingConfig
{
	std::string fileName;
	std::vector<ScalingRule> rules;

	std::string displayName;
};

class Configs : public REX::Singleton<Configs>
{
public:
	void Save();
	void Load();
	void Reload();
	void SaveConfigFile(const ScalingConfig& a_config);
	void LoadConfigFile(std::string_view a_fileName);
	void DeleteConfigFile(std::string_view a_fileName);
	void AddConfig(ScalingConfig a_config);
	void DeleteConfig(std::size_t a_index);

	std::size_t GetConfigCount() const;
	ScalingConfig GetConfig(std::size_t a_index) const;

	template <typename Func>
	void ForEachConfigs(Func&& a_func) const
	{
		std::shared_lock lock(configMutex);
		for (std::size_t i = 0; i < scalingConfigs.size(); ++i) {
			a_func(i, scalingConfigs[i]);
		}
	}

	float GetScaling(const RE::Actor* a_source, const RE::Actor* a_target) const;

private:
	void ParseConfigs();
	ScalingConfig ParseSingleConfig(const std::filesystem::path& a_filepath);
	void SortConfigs();

	mutable std::shared_mutex configMutex;
	std::vector<ScalingConfig> scalingConfigs;
	static inline constexpr auto configFolder{ "Data/SKSE/Plugins/DynamicDamageScaling/Configs" };
};

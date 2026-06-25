#include "Configs.h"
#include "Parser.h"

namespace
{
	constexpr const char* kRuleKey = "Damage";
	constexpr float kScalingMin = 0.0f;
	constexpr float kScalingMax = 10000.0f;

	struct KeyParser
	{
		template <typename Data>
		bool operator()(const std::string& a_key, [[maybe_unused]] Data& a_data) const
		{
			return a_key == kRuleKey;
		}
	};

	struct SourceParser
	{
		template <typename Data>
		void operator()(const std::string& a_entry, Data& a_data) const
		{
			if (!a_entry.empty()) {
				a_data.source = Conditions::ParseConditions(a_entry);
			}
		}
	};

	struct TargetParser
	{
		template <typename Data>
		void operator()(const std::string& a_entry, Data& a_data) const
		{
			if (!a_entry.empty()) {
				a_data.target = Conditions::ParseConditions(a_entry);
			}
		}
	};

	struct ScalingParser
	{
		template <typename Data>
		void operator()(const std::string& a_entry, Data& a_data) const
		{
			if (!a_entry.empty()) {
				try {
					float value = std::stof(a_entry);
					a_data.scaling = std::isfinite(value) ? std::clamp(value, kScalingMin, kScalingMax) : 1.0f;
				} catch (...) {
					a_data.scaling = 1.0f;
				}
			}
		}
	};

	void CacheConfigDisplay(ScalingConfig& a_config)
	{
		std::string name = a_config.fileName;
		if (name.ends_with(".ini")) {
			name.erase(name.length() - 4);
		}

		a_config.displayName = name;
	}

	void CacheRuleDisplay(ScalingRule& a_rule)
	{
		a_rule.displaySource = SerializeConditions(a_rule.source);
		a_rule.displayTarget = SerializeConditions(a_rule.target);

		if (a_rule.comment.empty()) {
			return;
		}

		const auto lines = Utils::String::Split(a_rule.comment, "\n");
		if (lines.empty()) {
			return;
		}

		std::string lastLine = lines.back();
		if (!lastLine.empty()) {
			lastLine.erase(0, 1);
		}

		const auto sections = Utils::String::Split(lastLine, "|");
		if (sections.size() > 0) {
			if (auto part = Utils::String::Trim(sections[0]); !part.empty()) {
				a_rule.displaySource = part;
			}
		}
		if (sections.size() > 1) {
			if (auto part = Utils::String::Trim(sections[1]); !part.empty()) {
				a_rule.displayTarget = part;
			}
		}
	}
}

void Configs::Save()
{
	std::unique_lock lock(configMutex);

	SortConfigs();
	for (const auto& config : scalingConfigs) {
		SaveConfigFile(config);
	}

	logger::info("All configs saved.");
}

void Configs::Load()
{
	std::unique_lock lock(configMutex);

	ParseConfigs();
	SortConfigs();

	logger::info("All configs loaded.");
}

void Configs::Reload()
{
	std::unique_lock lock(configMutex);

	ParseConfigs();
	SortConfigs();

	logger::info("All configs reloaded.");
}

void Configs::SaveConfigFile(const ScalingConfig& a_config)
{
	const std::filesystem::path folder(configFolder);
	if (!std::filesystem::exists(folder)) {
		std::filesystem::create_directories(folder);
	}

	const std::filesystem::path filePath = folder / a_config.fileName;

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.SetMultiKey();

	for (const auto& rule : a_config.rules) {
		std::string value = std::format("{}|{}|{:.2f}",
			SerializeConditions(rule.source),
			SerializeConditions(rule.target),
			rule.scaling);
		ini.SetValue("", kRuleKey, value.c_str(), rule.comment.empty() ? nullptr : rule.comment.c_str(), false);
	}

	if (ini.SaveFile(filePath.string().c_str()) < 0) {
		logger::error("Failed to save: {}", a_config.fileName);
	}
}

void Configs::LoadConfigFile(std::string_view a_fileName)
{
	std::unique_lock lock(configMutex);

	std::erase_if(scalingConfigs, [&](const ScalingConfig& config) {
		return config.fileName == a_fileName;
	});

	const std::filesystem::path filePath = std::filesystem::path(configFolder) / a_fileName;
	if (!std::filesystem::exists(filePath)) {
		logger::warn("File not found: {}", a_fileName);
		SortConfigs();
		return;
	}

	auto config = ParseSingleConfig(filePath);
	scalingConfigs.push_back(std::move(config));
	SortConfigs();

	logger::info("File loaded: {}", a_fileName);
}

void Configs::DeleteConfigFile(std::string_view a_fileName)
{
	const std::filesystem::path filePath = std::filesystem::path(configFolder) / a_fileName;
	if (!std::filesystem::exists(filePath)) {
		logger::warn("File not found: {}", a_fileName);
		return;
	}

	std::filesystem::remove(filePath);
	logger::info("File deleted: {}", a_fileName);
}

void Configs::AddConfig(ScalingConfig a_config)
{
	std::unique_lock lock(configMutex);

	for (auto& rule : a_config.rules) {
		CacheRuleDisplay(rule);
	}
	scalingConfigs.push_back(std::move(a_config));
}

void Configs::DeleteConfig(std::size_t a_index)
{
	std::unique_lock lock(configMutex);

	if (a_index >= scalingConfigs.size()) {
		logger::warn("Index {} out of range {}", a_index, scalingConfigs.size());
		return;
	}
	scalingConfigs.erase(scalingConfigs.begin() + static_cast<std::ptrdiff_t>(a_index));
}

std::size_t Configs::GetConfigCount() const
{
	std::shared_lock lock(configMutex);
	return scalingConfigs.size();
}

ScalingConfig Configs::GetConfig(std::size_t a_index) const
{
	std::shared_lock lock(configMutex);

	if (a_index >= scalingConfigs.size()) {
		logger::warn("Index {} out of range {}", a_index, scalingConfigs.size());
		return {};
	}
	return scalingConfigs[a_index];
}

float Configs::GetScaling(const RE::Actor* a_source, const RE::Actor* a_target) const
{
	std::shared_lock lock(configMutex);

	// For the same file: Only the last scaling will be applied
	// For different files: All scaling will be multiplied
	float result = 1.0f;
	for (const auto& config : scalingConfigs) {
		for (auto rule = config.rules.rbegin(); rule != config.rules.rend(); ++rule) {
			if (Conditions::MatchConditions(rule->source, a_source) &&
				Conditions::MatchConditions(rule->target, a_target)) {
				result *= rule->scaling;
				if (result == 0.0f) {
					return result;
				}
				break;
			}
		}
	}

	return std::clamp(result, kScalingMin, kScalingMax);
}

void Configs::ParseConfigs()
{
	scalingConfigs.clear();

	const std::filesystem::path folder(configFolder);
	if (!std::filesystem::exists(folder)) {
		logger::warn("Configs folder not found: {}", configFolder);
		return;
	}

	try {
		for (const auto& entry : std::filesystem::directory_iterator(folder)) {
			if (!entry.is_regular_file()) {
				continue;
			}
			if (entry.path().extension() != ".ini") {
				continue;
			}

			auto config = ParseSingleConfig(entry.path());
			scalingConfigs.push_back(std::move(config));
		}
	} catch (const std::exception& e) {
		logger::error("Failed to parse: {}, {}", configFolder, e.what());
	}
}

ScalingConfig Configs::ParseSingleConfig(const std::filesystem::path& a_filePath)
{
	ScalingConfig config;
	config.fileName = a_filePath.filename().string();
	CacheConfigDisplay(config);

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.SetMultiKey();
	if (ini.LoadFile(a_filePath.string().c_str()) < 0) {
		logger::error("Failed to open: {}", config.fileName);
		return config;
	}

	const auto values = ini.GetSection("");
	if (values && !values->empty()) {
		for (const auto& [key, value] : *values) {
			try {
				auto rule = Parser::Parse<ScalingRule,
					KeyParser,
					SourceParser,
					TargetParser,
					ScalingParser>(key.pItem, value);

				if (rule) {
					rule->comment = key.pComment ? key.pComment : "";
					CacheRuleDisplay(*rule);
					config.rules.push_back(std::move(*rule));
				}
			} catch (const std::exception& e) {
				logger::error("Skipping error rule: {}:{}, {}", config.fileName, value, e.what());
			}
		}
	}

	return config;
}

void Configs::SortConfigs()
{
	std::sort(scalingConfigs.begin(), scalingConfigs.end(),
		[](const ScalingConfig& a, const ScalingConfig& b) {
			return a.fileName < b.fileName;
		});
}

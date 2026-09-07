#include "Translations.h"

void Translations::Load()
{
	transTable.clear();

	std::ifstream file(transPath);
	if (!file.is_open()) {
		logger::warn("Failed to open translation file: {}", transPath);
		return;
	}

	try {
		nlohmann::json data;
		file >> data;

		for (const auto& [key, value] : data.items()) {
			if (value.is_string()) {
				transTable.insert_or_assign(key, value.get<std::string>());
			}
		}

		logger::info("Translation file loaded: {}", transPath);
	} catch (const std::exception& e) {
		logger::error("Failed to load translation file: {}, {}", transPath, e.what());
	}
}

const char* Translations::Get(std::string_view a_key)
{
	const auto it = transTable.find(a_key);
	if (it != transTable.end()) {
		return it->second.c_str();
	}
	
	logger::warn("Missing translation key: {}", a_key);
	auto [iterator, inserted] = transTable.try_emplace(std::string(a_key), std::string(a_key));
	return iterator->second.c_str();
}

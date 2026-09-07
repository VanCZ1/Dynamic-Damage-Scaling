#pragma once
#include "Utils.h"

// https://github.com/powerof3/Spell-Perk-Item-Distributor/blob/master/SPID/src/Parser.h
// License: MIT 
namespace Parser
{
	template <typename Data, typename... ComponentParsers, std::size_t... Is>
	void ParseEach(Data& a_data, const std::string a_splited[sizeof...(ComponentParsers)], std::index_sequence<Is...>)
	{
		(ComponentParsers()(a_splited[Is], a_data), ...);
	}

	template <typename KeyComponentParser, typename Data>
	concept key_component_parser = requires(KeyComponentParser const, const std::string& a_key, Data& a_data) {
		{
			KeyComponentParser()(a_key, a_data)
		} -> std::same_as<bool>;
	};

	template <typename ComponentParser, typename Data>
	concept component_parser = requires(ComponentParser const, const std::string& a_section, Data& a_data) {
		{
			ComponentParser()(a_section, a_data)
		} -> std::same_as<void>;
	};

	template <typename Data, key_component_parser<Data> KeyComponentParser, component_parser<Data>... ComponentParsers>
	std::optional<Data> Parse(const std::string& a_key, const std::string& a_entry)
	{
		Data data{};

		if (!KeyComponentParser()(a_key, data)) {
			return std::nullopt;
		}

		if (a_entry.empty()) {
			return std::nullopt;
		}

		const auto sections = Utils::String::Split(a_entry, "|");
		const std::size_t numberOfSections = sections.size();
		constexpr std::size_t numberOfComponents = sizeof...(ComponentParsers);

		if (numberOfSections > numberOfComponents) {
			logger::error("Too many sections: {}", a_entry);
			return std::nullopt;
		}

		std::string finalSections[numberOfComponents];
		for (std::size_t i = 0; i < numberOfSections; ++i) {
			finalSections[i] = sections[i];
		}

		ParseEach<Data, ComponentParsers...>(data, finalSections, std::index_sequence_for<ComponentParsers...>());

		return data;
	};
}

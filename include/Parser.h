#pragma once
#include "Utils.h"

// clang-format off
//https://github.com/powerof3/Spell-Perk-Item-Distributor/blob/master/SPID/src/Parser.h
//MIT License
//
//Copyright (c) 2021 powerofthree
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
// clang-format on
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

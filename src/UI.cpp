#include "UI.h"
#include "Configs.h"
#include "Settings.h"
#include "Translations.h"

#include "API/SKSEMenuFramework.h"

namespace UI
{
	namespace
	{
		void HelpMarker(const char* a_str)
		{
			ImGuiMCP::SameLine();
			ImGuiMCP::TextDisabled("(?)");
			if (ImGuiMCP::IsItemHovered()) {
				ImGuiMCP::BeginTooltip();
				ImGuiMCP::TextUnformatted(a_str);
				ImGuiMCP::EndTooltip();
			}
		}

		void RenderSingleConfig(const ScalingConfig& a_config, const RE::Actor* a_actor)
		{
			const ImGuiMCP::ImGuiTableFlags tableFlags =
				ImGuiMCP::ImGuiTableFlags_Borders |
				ImGuiMCP::ImGuiTableFlags_RowBg |
				ImGuiMCP::ImGuiTableFlags_SizingStretchProp |
				ImGuiMCP::ImGuiTableFlags_Resizable;

			if (ImGuiMCP::BeginTable("RulesTable", 3, tableFlags)) {
				ImGuiMCP::TableSetupColumn(Trans("Configs.Rules.Attacker"), ImGuiMCP::ImGuiTableColumnFlags_None);
				ImGuiMCP::TableSetupColumn(Trans("Configs.Rules.Victim"), ImGuiMCP::ImGuiTableColumnFlags_None);
				ImGuiMCP::TableSetupColumn(Trans("Configs.Rules.Scaling"), ImGuiMCP::ImGuiTableColumnFlags_None);
				ImGuiMCP::TableHeadersRow();

				// Green Highlight
				constexpr ImGuiMCP::ImU32 matchHighlight = IM_COL32(150, 255, 50, 80);

				for (const auto& [index, rule] : std::views::enumerate(a_config.rules)) {
					ImGuiMCP::TableNextRow();

					bool attackerMatch = a_actor && Conditions::MatchConditions(rule.source, a_actor);
					bool victimMatch = a_actor && Conditions::MatchConditions(rule.target, a_actor);

					ImGuiMCP::TableSetColumnIndex(0);
					if (attackerMatch) {
						ImGuiMCP::TableSetBgColor(ImGuiMCP::ImGuiTableBgTarget_CellBg, matchHighlight);
					}
					ImGuiMCP::TextUnformatted(rule.displaySource.c_str());

					ImGuiMCP::TableSetColumnIndex(1);
					if (victimMatch) {
						ImGuiMCP::TableSetBgColor(ImGuiMCP::ImGuiTableBgTarget_CellBg, matchHighlight);
					}
					ImGuiMCP::TextUnformatted(rule.displayTarget.c_str());

					ImGuiMCP::TableSetColumnIndex(2);
					ImGuiMCP::Text("%.2f", rule.scaling);
				}

				ImGuiMCP::EndTable();
			}
		}

		void __stdcall RenderConfigs()
		{
			auto configs = Configs::GetSingleton();

			if (ImGuiMCP::Button(Trans("Configs.ReloadAll"))) {
				configs->Reload();
			}

			ImGuiMCP::Separator();

			const auto consoleRefr = RE::Console::GetSelectedRef();
			const auto consoleActor = consoleRefr ? consoleRefr->As<RE::Actor>() : nullptr;
			configs->ForEachConfigs([&](std::size_t i, const ScalingConfig& config) {
				ImGuiMCP::PushID(static_cast<int>(i));
				if (ImGuiMCP::CollapsingHeader(config.displayName.c_str())) {
					RenderSingleConfig(config, consoleActor);
				}
				ImGuiMCP::PopID();
			});
		}

		void __stdcall RenderSettings()
		{
			auto settings = Settings::GetSingleton();

			if (ImGuiMCP::Checkbox(Trans("Settings.Enable"), &settings->isEnabled)) {
				settings->Save();
			}

			if (ImGuiMCP::Checkbox(Trans("Settings.IgnoreAttack"), &settings->isIgnoreAttack)) {
				settings->Save();
			}
			HelpMarker(Trans("Settings.IgnoreAttack.Desc"));
		}
	}

	void Register()
	{
		if (!SKSEMenuFramework::IsInstalled()) {
			logger::warn("SKSE Menu Framework is not installed.");
			return;
		}

		SKSEMenuFramework::SetSection(Trans("Plugin"));
		SKSEMenuFramework::AddSectionItem(Trans("Settings"), RenderSettings);
		SKSEMenuFramework::AddSectionItem(Trans("Configs"), RenderConfigs);
		logger::info("SKSE Menu registered successfully.");
	}
}

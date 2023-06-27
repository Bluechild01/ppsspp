#pragma once

#include <cstdint>

#include "Common/File/Path.h"
#include "Common/UI/View.h"
#include "Common/UI/UIScreen.h"
#include "Common/UI/ViewGroup.h"
#include "Core/RetroAchievements.h"
#include "UI/MiscScreens.h"
#include "UI/TabbedDialogScreen.h"

// Lists the achievements and leaderboards for one game.
class RetroAchievementsListScreen : public TabbedUIDialogScreenWithGameBackground {
public:
	RetroAchievementsListScreen(const Path &gamePath) : TabbedUIDialogScreenWithGameBackground(gamePath) {}
	const char *tag() const override { return "RetroAchievementsListScreen"; }

	void CreateTabs() override;

protected:
	bool ShowSearchControls() const override { return false; }

private:
	void CreateAchievementsTab(UI::ViewGroup *viewGroup);
	void CreateLeaderboardsTab(UI::ViewGroup *viewGroup);
	void CreateStatisticsTab(UI::ViewGroup *viewGroup);
};

// Lets you manage your account, and shows some achievement stats and stuff.
class RetroAchievementsSettingsScreen : public TabbedUIDialogScreenWithGameBackground {
public:
	RetroAchievementsSettingsScreen(const Path &gamePath) : TabbedUIDialogScreenWithGameBackground(gamePath) {}
	const char *tag() const override { return "RetroAchievementsSettingsScreen"; }

	void CreateTabs() override;
	void sendMessage(const char *message, const char *value) override;

protected:
	bool ShowSearchControls() const override { return false; }

private:
	void CreateAccountTab(UI::ViewGroup *viewGroup);
	void CreateSettingsTab(UI::ViewGroup *viewGroup);

	std::string username_;
	std::string password_;
};

class RetroAchievementsLeaderboardScreen : public TabbedUIDialogScreenWithGameBackground {
public:
	RetroAchievementsLeaderboardScreen(const Path &gamePath, int leaderboardID);
	~RetroAchievementsLeaderboardScreen();

	const char *tag() const override { return "RetroAchievementsLeaderboardScreen"; }

	void CreateTabs() override;

	void update() override;

protected:
	bool ShowSearchControls() const override { return false; }

private:
	void Poll();

	int leaderboardID_;

	// Keep the fetched list alive and destroy in destructor.
	rc_client_leaderboard_entry_list_t *entryList_ = nullptr;

	rc_client_leaderboard_entry_list_t *pendingEntryList_ = nullptr;

	rc_client_async_handle_t *pendingAsyncCall_ = nullptr;
};

class UIContext;

enum class AchievementRenderStyle {
	LISTED,
	UNLOCKED,
	PROGRESS_INDICATOR,
};

void MeasureAchievement(const UIContext &dc, const rc_client_achievement_t *achievement, AchievementRenderStyle style, float *w, float *h);
void RenderAchievement(UIContext &dc, const rc_client_achievement_t *achievement, AchievementRenderStyle style, const Bounds &bounds, float alpha, float startTime, float time_s);

void MeasureGameAchievementSummary(const UIContext &dc, int gameID, float *w, float *h);
void RenderGameAchievementSummary(UIContext &dc, int gameID, const Bounds &bounds, float alpha);

class AchievementView : public UI::ClickableItem {
public:
	AchievementView(const rc_client_achievement_t *achievement, UI::LayoutParams *layoutParams = nullptr) : UI::ClickableItem(layoutParams), achievement_(achievement) {}

	void Click() override;
	void Draw(UIContext &dc) override;
	void GetContentDimensions(const UIContext &dc, float &w, float &h) const override;
private:
	const rc_client_achievement_t *achievement_;
};

class LeaderboardSummaryView : public UI::ClickableItem {
public:
	LeaderboardSummaryView(const rc_client_leaderboard_t *leaderboard, UI::LayoutParams *layoutParams = nullptr) : UI::ClickableItem(layoutParams), leaderboard_(leaderboard) {}

	void Draw(UIContext &dc) override;
	void GetContentDimensions(const UIContext &dc, float &w, float &h) const override;

private:
	const rc_client_leaderboard_t *leaderboard_;
};

class GameAchievementSummaryView : public UI::Item {
public:
	GameAchievementSummaryView(int gameID, UI::LayoutParams *layoutParams = nullptr) : UI::Item(layoutParams), gameID_(gameID) {}

	void Draw(UIContext &dc) override;
	void GetContentDimensions(const UIContext &dc, float &w, float &h) const override;
private:
	int gameID_;
};

#pragma once
#include <string>

struct Stats
{
	Stats() = default;
	~Stats() = default;

	[[nodiscard]] static int GetKills() { return m_kills; }
	[[nodiscard]] static int GetTotalKills() { return m_totalKills; }
	[[nodiscard]] static int GetDamage() { return m_damage; }
	[[nodiscard]] static int GetThisLevelDamage() { return m_totalDamage - m_lastLevelDamage; }
	[[nodiscard]] static int GetTotalDamage() { return m_totalDamage; }
	[[nodiscard]] static int GetDeaths() { return m_deaths; }
	[[nodiscard]] static int GetTimePlayed() { return m_timer; }
	[[nodiscard]] static std::string GetLastLevel() { return m_lastLevel; }

	void SetKills(const int kills) { m_kills = kills; m_totalKills += kills; }
	void AddDamage(const int damage) { m_damage += damage; m_totalDamage += damage; }
	void IncrementDeaths() { m_deaths++; }
	void SetLastLevel(const std::string level_name) { m_lastLevel = level_name; m_lastLevelDamage = m_totalDamage; }
	void SetTimePlayed(int time) { m_timer = time; }

private:
	inline static int m_kills = 0;
	inline static int m_totalKills = 0;
	inline static int m_damage = 0;
	inline static int m_lastLevelDamage = 0;
	inline static int m_totalDamage = 0;
	inline static int m_deaths = 0;
	inline static int m_timer = 0;
	inline static std::string m_lastLevel{};
};

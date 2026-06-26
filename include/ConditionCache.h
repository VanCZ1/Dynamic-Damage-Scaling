#pragma once

class ConditionCache
{
public:
	static ConditionCache& GetSingleton();

	void Reset();
	int GetFollowerCount();

protected:
	ConditionCache() = default;
	~ConditionCache() = default;
	ConditionCache(const ConditionCache&) = delete;
	ConditionCache(ConditionCache&&) = delete;
	ConditionCache& operator=(const ConditionCache&) = delete;
	ConditionCache& operator=(ConditionCache&&) = delete;

private:
	struct Cache
	{
		std::optional<int> followerCount;
	} cache;
};

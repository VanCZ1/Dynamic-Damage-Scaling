#include "ConditionCache.h"
#include "Utils.h"

ConditionCache& ConditionCache::GetSingleton()
{
	thread_local ConditionCache singleton;
	return singleton;
}

void ConditionCache::Reset()
{
	cache = {};
}

int ConditionCache::GetFollowerCount()
{
	if (!cache.followerCount) {
		cache.followerCount = Utils::Game::CalcFollowerCount();
	}

	return *cache.followerCount;
}

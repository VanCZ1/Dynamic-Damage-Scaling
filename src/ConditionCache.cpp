#include "ConditionCache.h"
#include "Utils.h"

namespace ConditionCache
{
	namespace
	{
		struct Data
		{
			std::optional<int> followerCount;
		};
		thread_local Data data;
	}

	void Reset()
	{
		data = {};
	}

	int GetFollowerCount()
	{
		if (!data.followerCount) {
			data.followerCount = Utils::Game::CalcFollowerCount();
		}
		return *data.followerCount;
	}
}

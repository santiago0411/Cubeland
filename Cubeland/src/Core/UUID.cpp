#include "pch.h"
#include "UUID.h"

#include <random>

namespace Cubeland
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_RandomEngine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformIntDistribution;

	UUID::UUID()
		: m_UUID(s_UniformIntDistribution(s_RandomEngine))
	{
	}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{
	}

	UUID::UUID(const UUID& other)
		: m_UUID(other.m_UUID)
	{
	}
}

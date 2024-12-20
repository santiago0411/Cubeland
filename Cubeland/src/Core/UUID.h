#pragma once

#include <cstdint>

namespace Cubeland
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& other);

		operator uint64_t() noexcept { return m_UUID; }
		operator const uint64_t() const noexcept { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Cubeland::UUID>
	{
		std::size_t operator()(const Cubeland::UUID& uuid) const noexcept
		{
			return uuid;
		}
	};
}

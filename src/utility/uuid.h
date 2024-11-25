#pragma once
#include <cstdint>
#include <string>

class UUID {
public:
	uint64_t uuid;

	UUID();
	UUID(uint64_t uuid);
	UUID(const UUID&) = default;

	std::string string() const { return std::to_string(uuid); }

	operator uint64_t() const { return uuid; }
};

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<UUID> {
		size_t operator()(const UUID& uuid) const {
			return (uint64_t)uuid;
		}
	};
}

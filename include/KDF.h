#pragma once

#include "./Common.h"

class KDF {

	public:
		using key_32 = std::array<uint8_t, 32>;
		using salt_16 = std::array<uint8_t, 16>;

		KDF();

		static key_32 derive(std::string_view _password,
			std::span<const uint8_t, 16> _salt);

		static salt_16 randomSalt();

		static void zero(key_32& _key);
};
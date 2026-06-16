#pragma once

#include "./Common.h"

class CryptoEngine {

	public:
		using key = std::array<uint8_t, crypto_secretbox_KEYBYTES>;
		using Nonce = std::array<uint8_t, crypto_secretbox_NONCEBYTES>;

		static std::vector<uint8_t> encrypt(std::string& _plaintext,
			std::span<uint8_t, crypto_secretbox_KEYBYTES> _key);

		static std::string decrypt(std::span<const uint8_t> _cipertextPayload,
			std::span<const uint8_t, crypto_secretbox_KEYBYTES> _key);

		static Nonce randomNonce();
};
#pragma once

#include "Common.h"
#include "Crypto.h"
#include "Credential.h"

class Vault {
	inline static const std::filesystem::path VaultPath = Common::defaultVaultPath();
	CryptoEngine cryptoEngine;
	inline static bool isUnlocked = false;
	inline static KDF::salt_16 _salt;
	inline static KDF::key_32 _key;

	public:

		static void init(const std::string& _masterPassword);

		static void save(std::vector<Credential>& _credentials);

		static nlohmann::json unlock(const std::string& _masterPassword);

		static bool getIsUnlocked();

		KDF::key_32* getKey();
};
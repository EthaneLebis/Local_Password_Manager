#include "../include/Vault.h"
#include "../include/Common.h"
#include "../include/KDF.h"
#include "../include/Crypto.h"

void Vault::init(const std::string& _masterPassword) {
	_salt = KDF::randomSalt();
	_key = KDF::derive(_masterPassword, _salt);

	std::string plaintext = nlohmann::json::array().dump();

	auto encryptedData = CryptoEngine::encrypt(plaintext, _key);

	std::filesystem::path TempPath = VaultPath;
	TempPath.replace_extension(".vault.tmp");

	{
		std::ofstream createVaultFile(TempPath, std::ios::out | std::ios::binary);

		if (!createVaultFile.is_open()) {
			std::cerr << "Could not create vault file at path.\n";

			return;
		}

		createVaultFile.write(reinterpret_cast<const char*>(_salt.data()), _salt.size());
		createVaultFile.write(reinterpret_cast<const char*>(encryptedData.data()), encryptedData.size());
		createVaultFile.close();
	}

	if (std::filesystem::exists(VaultPath))
		std::filesystem::remove(VaultPath);

	std::filesystem::rename(TempPath, VaultPath);

	std::cout << "Vault initialized successfully with a secure profile format. Use 'vault unlock' first.\n";
}

void Vault::save(std::vector<Credential>& _credentials) {
	nlohmann::json json = nlohmann::json::array();

	for (const auto& credential : _credentials) 
		json.push_back(credential.to_json());

	std::string plaintext = json.dump();

	auto encryptedData = CryptoEngine::encrypt(plaintext, _key);

	std::filesystem::path TempPath = VaultPath;
	TempPath.replace_extension(".vault.tmp");

	{
		std::ofstream createVaultFile(TempPath, std::ios::out | std::ios::binary);

		if (!createVaultFile.is_open()) {
			std::cerr << "Could not create vault file at path.\n";

			return;
		}

		createVaultFile.write(reinterpret_cast<const char*>(_salt.data()), _salt.size());
		createVaultFile.write(reinterpret_cast<const char*>(encryptedData.data()), encryptedData.size());
		createVaultFile.close();
	}

	if (std::filesystem::exists(VaultPath))
		std::filesystem::remove(VaultPath);

	std::filesystem::rename(TempPath, VaultPath);

	std::cout << "Saved successfully.\n";
}

nlohmann::json Vault::unlock(const std::string& _masterPassword) {
	std::ifstream readVaultFile(VaultPath, std::ios::in | std::ios::binary);

	if (!readVaultFile.is_open()) 
		throw std::runtime_error("Could not open vault file.");

	readVaultFile.read(reinterpret_cast<char*>(_salt.data()), _salt.size());

	std::vector<uint8_t> encryptedPayload((std::istreambuf_iterator<char>(readVaultFile)),
		std::istreambuf_iterator<char>());

	readVaultFile.close();

	_key = KDF::derive(_masterPassword, _salt);

	try {
		std::string json = CryptoEngine::decrypt(encryptedPayload, _key);


		std::cout << "Access granted.\n";

		auto vaultData = nlohmann::json::parse(json);

		isUnlocked = true;

		return vaultData;
	}
	catch (const std::exception& _error) {
		std::cerr << "Access denied. " << _error.what() << std::endl;
	}

	throw std::runtime_error("Could not open vault file.");
}

bool Vault::getIsUnlocked() {
	return isUnlocked;
}

KDF::key_32* Vault::getKey() {
	return &_key;
}
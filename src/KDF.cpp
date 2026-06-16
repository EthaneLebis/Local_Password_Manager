#include "../include/KDF.h"

KDF::KDF() {
	if (sodium_init() < 0)
		std::cerr << "libsodium initialization failed.\n";
}


KDF::key_32 KDF::derive(std::string_view _password,
	std::span<const uint8_t, 16> _salt) {

	key_32 key{ };

	int rc = crypto_pwhash(
		key.data(),
		key.size(),
		_password.data(),
		_password.size(),
		_salt.data(),
		crypto_pwhash_OPSLIMIT_INTERACTIVE,
		crypto_pwhash_MEMLIMIT_INTERACTIVE,
		crypto_pwhash_ALG_ARGON2ID13);

	if (rc != 0)
		throw std::runtime_error("KDF failed - possibly out of memory.\n");

	return key;
}

KDF::salt_16 KDF::randomSalt() {
	salt_16 salt{ };

	randombytes_buf(salt.data(), salt.size());

	return salt;
}

void KDF::zero(key_32& _key) {
	sodium_memzero(_key.data(), _key.size());
}
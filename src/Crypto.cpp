#include "../include/Crypto.h"

std::vector<uint8_t> CryptoEngine::encrypt(std::string& _plaintext,
	std::span<uint8_t, crypto_secretbox_KEYBYTES> _key) {

	Nonce nonce = randomNonce();

	std::vector<uint8_t> output(crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES + _plaintext.size());

	std::copy(nonce.begin(), nonce.end(), output.begin());

	uint8_t* ciphertext = output.data() + crypto_secretbox_NONCEBYTES;

	crypto_secretbox_easy(ciphertext, reinterpret_cast<const unsigned char*>(_plaintext.data()),
		_plaintext.size(), nonce.data(), _key.data());

	return output;
}

std::string CryptoEngine::decrypt(std::span<const uint8_t> _cipertextPayload,
	std::span<const uint8_t, crypto_secretbox_KEYBYTES> _key) {

	if (_cipertextPayload.size() < crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES) {
		throw std::runtime_error("Ciphertext payload is too short or corrupted.");
	}

	Nonce nonce{ };

	std::copy(_cipertextPayload.begin(), _cipertextPayload.begin() + crypto_secretbox_NONCEBYTES, nonce.begin());

	const uint8_t* ciphertext = _cipertextPayload.data() + crypto_secretbox_NONCEBYTES;
	size_t cipertextSize = _cipertextPayload.size() - crypto_secretbox_NONCEBYTES;

	std::string plaintext;
	plaintext.resize(cipertextSize - crypto_secretbox_MACBYTES);

	int result = crypto_secretbox_open_easy(
		reinterpret_cast<unsigned char*>(plaintext.data()),
		ciphertext,
		cipertextSize,
		nonce.data(),
		_key.data());

	if (result != 0)
		throw std::runtime_error("Decryption failed. Data tampered or invalid key.");

	return plaintext;
}

CryptoEngine::Nonce CryptoEngine::randomNonce() {
	Nonce nonce{ };

	randombytes_buf(nonce.data(), nonce.size());

	return nonce;
}
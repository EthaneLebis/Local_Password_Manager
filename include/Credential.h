#pragma once

#include "./Common.h"
#include "KDF.h"

struct Credential {
	std::string website;
	std::string username;
	std::string password;
	std::string note;
	int64_t created_at = 0;

	static Credential from_json(const nlohmann::json& _credential) {
		Credential credential;

		credential.website = _credential.value("website", "");
		credential.username = _credential.value("username", "");
		credential.password = _credential.value("password", "");
		credential.note = _credential.value("note", "");
		credential.created_at = _credential.value("created_at", int64_t{ 0 });

		return credential;
	}

	nlohmann::json to_json() const {
		return {
			{ "website",	website },
			{ "username",	username },
			{ "password",	password },
			{ "note",		note },
			{ "created_at",	created_at }
		};
	}

	Credential() = default;

	Credential(Credential&& other) noexcept
		: website(std::move(other.website)),
		username(std::move(other.username)),
		password(std::move(other.password)),
		note(std::move(other.note)),
		created_at(other.created_at) {

		other.created_at = 0;
	}

	Credential& operator=(Credential&& other) noexcept {
		if (this != &other) {
			website = std::move(other.website);
			username = std::move(other.username);
			password = std::move(other.password);
			note = std::move(other.note);
			created_at = other.created_at;
			other.created_at = 0;
		}

		return *this;
	}

	Credential(const Credential&) = delete;
	Credential& operator=(const Credential&) = delete;
};

class Credentials {
	inline static std::vector<Credential> credentials = { };
	std::filesystem::path VaultPath = Common::defaultVaultPath();
	nlohmann::json credentials_json;

	public:

		~Credentials() = default;

		static void setCredentials(const nlohmann::json& _credentials);

		static Credential* getCredential(std::string& _website);

		std::vector<Credential>& getCredentials();

		void addCredential(Credential&& _credential);

		static bool updateCredential(std::string& _website);

		static bool deleteCredential(std::string& _website);
};
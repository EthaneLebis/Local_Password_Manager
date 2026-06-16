#include "./include/Common.h"

#include "./include/Credential.h"

Credentials::Credentials() {
	VaultPath = Common::defaultVaultPath();

	if (!std::filesystem::exists(VaultPath))
		return;
		
	std::ifstream VaultFile(VaultPath);

	if (VaultFile.is_open()) {
		nlohmann::json vault_json;
		VaultFile >> vault_json;
		VaultFile.close();

		// TODO: Master Password addition

		if (vault_json.contains("credentials") && vault_json["credentials"].is_array()) {
			for (const auto& item : vault_json["credentials"]) {
				Credential _credential = Credential::from_json(item);
				
				credentials.push_back(std::move(_credential));
			}
		}
	}
}

Credential Credentials::getCredential() {
	return;
}

std::vector<Credential> Credentials::getCredentials() {
	return credentials;
}

void Credentials::addCredential(Credential _credential) {
	credentials.push_back(_credential);

	if (saveToDisc())
		std::cout << "Successfully saved new credential to file!\n";
}

bool Credentials::updateCredential() {
	return;
}

bool Credentials::deleteCredential() {
	return;
}

bool Credentials::saveToDisc() {
	nlohmann::json vault_json;
	vault_json["credentials"] = nlohmann::json::array();

	for (const auto& credential : credentials) {
		nlohmann::json obj;
		obj["website"] = credential.website;
		obj["username"] = credential.username;
		obj["password"] = credential.password;
		obj["note"] = credential.note;
		obj["created_at"] = credential.created_at;

		vault_json["credentials"].push_back(obj);
	}

	std::ofstream VaultFile(VaultPath);

	if (VaultFile.is_open()) {
		VaultFile << vault_json.dump(4);
		VaultFile.close();

		return true;
	}

	return false;
}
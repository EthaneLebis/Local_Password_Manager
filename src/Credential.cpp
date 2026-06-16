#include "../include/Common.h"
#include "../include/Credential.h"

void Credentials::setCredentials(const nlohmann::json& _credentials) {
	if (!_credentials.is_array())
		return;

	credentials.clear();

	for (const auto& credential : _credentials) {
		Credential _credential = Credential::from_json(credential);

		credentials.push_back(std::move(_credential));
	}
}

Credential* Credentials::getCredential(std::string& _website) {
	for (auto& credential : credentials) {
		if (credential.website == _website)
			return &credential;
	}

	return nullptr;
}

std::vector<Credential>& Credentials::getCredentials() {
	return credentials;
}

void Credentials::addCredential(Credential&& _credential) {
	credentials.push_back(std::move(_credential));
}

bool Credentials::updateCredential(std::string& _website) {
	for (auto& credential : credentials) {
		if (credential.website == _website) {
			std::string username, note;
			
			std::cout << "New username (Enter to keep current): ";
			std::getline(std::cin, username);
			if (!username.empty()) credential.username = username;

			std::cout << "New note (Enter to keep current): ";
			std::getline(std::cin, note);
			if (!note.empty()) credential.note = note;

			credential.password = Common::getPassword("New password (Enter to keep current): ");

			return true;
		}
	}

	return false;
}

bool Credentials::deleteCredential(std::string& _website) {
	size_t originalSize = credentials.size();

	std::erase_if(credentials, [&](const Credential& credential) {
		return credential.website == _website;
		});

	return credentials.size() < originalSize;
}
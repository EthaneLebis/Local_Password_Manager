#include "../Common.h"

#include "../include/Command.h"
#include "../include/Credential.h"
#include "../include/KDF.h"

void Command::run() {
	bool isRunning = true;
	std::string command;

	do {

		std::cout << "vault-cli $ ";

		std::getline(std::cin, command);

		command = Common::trim(command);

		if (command == "vault init") {
			Command::_init();
		}
		else if (command == "vault unlock") {
			Command::_unlock();
		}
		else if (command == "vault add") {
			Command::_add();
		}
		else if (command == "vault get") {
			Command::_get();
		}
		else if (command == "vault list") {
			Command::_list();
		}
		else if (command == "vault delete") {
			Command::_delete();
		}
		else if (command == "vault update") {
			Command::_update();
		}
		else if (command == "vault generate") {
			Command::_generate();
		}
		else if (command == "vault help") {
			Command::_help();
		}
		else if (command == "vault exit") {
			Command::_exit(isRunning);
		}
		else {
			std::cerr << "Error: " << command << " is not a recognized vault command.\n";
			std::cout << "Type 'vault help' to see available commands.\n";
		}

	} while (isRunning);
}

void Command::_init() {
	std::filesystem::path VaultPath(Common::defaultVaultPath());

	if (std::filesystem::exists(VaultPath)) {
		std::cerr << "The file is already exists at " << VaultPath << ".\n";
		std::cerr << "Use 'vault add' to add credentials.\n";

		return;
	}

	std::string password_1 = Common::getPassword("Create master password: ");
	std::string password_2 = Common::getPassword("Confirm master password: ");

	if (password_1 != password_2) {
		std::cerr << "Password do not match.\n";

		return;
	}

	std::filesystem::create_directories(VaultPath.parent_path());

	vaultEngine.init(password_1);
}

void Command::_unlock() {
	std::filesystem::path VaultPath(Common::defaultVaultPath());

	if (!std::filesystem::exists(VaultPath)) {
		std::cerr << "No vault found. Run 'vault init' first.\n";

		return;
	}

	if (vaultEngine.getIsUnlocked()) {
		std::cerr << "Vault is already unlocked.\n";

		return;
	}

	std::string masterPassword = Common::getPassword("Enter master password: ");

	try {
		db.setCredentials(vaultEngine.unlock(masterPassword));
	}
	catch (const std::exception& _error) {
		std::cerr << "Access denied: " << _error.what() << '\n';
	}
}

void Command::_add() {
	std::filesystem::path VaultPath(Common::defaultVaultPath());

	if (!std::filesystem::exists(VaultPath)) {
		std::cerr << "No vault found. Run 'vault init' first.\n";

		return;
	}

	if (!vaultEngine.getIsUnlocked()) {
		std::cerr << "Vault is not unlocked. Call 'vault unlock' first.\n";

		return;
	}

	Credential credential;

	std::cout << "Website: ";
	std::getline(std::cin, credential.website);

	if (std::any_of(db.getCredentials().begin(), db.getCredentials().end(),
		[&](const Credential& _credential) {
			return _credential.website == credential.website;
		})) {

		std::cerr << "A credential already exists. Use 'vault update' or 'vault delete' for exists one. Or use different website.\n";

		return;
	}

	std::cout << "Username: ";
	std::getline(std::cin, credential.username);

	credential.password = Common::getPassword("Password: ");

	std::cout << "Notes (optional, Enter to skip): ";
	std::getline(std::cin, credential.note);

	credential.created_at = std::time(nullptr);

	db.addCredential(std::move(credential));

	vaultEngine.save(db.getCredentials());
}

void Command::_get() {
	std::filesystem::path VaultPath(Common::defaultVaultPath());

	if (!std::filesystem::exists(VaultPath)) {
		std::cerr << "No vault found. Run 'vault init' first.\n";

		return;
	}

	if (!vaultEngine.getIsUnlocked()) {
		std::cerr << "Vault is not unlocked. Call 'vault unlock' first.\n";

		return;
	}

	const auto& list = db.getCredentials();

	if (list.empty()) {
		std::cerr << "Your vault is empty. Use 'vault add' to save credential.\n";

		return;
	}

	std::string website;
	std::cout << "Website: ";
	std::getline(std::cin, website);

	Credential* credential = db.getCredential(website);

	if (credential != nullptr) {
		std::cout << "Username: " << credential->username << "\n";
		std::cout << "Note: " << credential->note << "\n";
		
		if (Common::copyToClipboard(credential->password))
			std::cout << "Password was copied clipboard.\n";
	}
	else {
		std::cerr << "No any credential found.\n";
	}
}

void Command::_list() {
	std::filesystem::path VaultPath(Common::defaultVaultPath());

	if (!std::filesystem::exists(VaultPath)) {
		std::cerr << "No vault found. Run 'vault init' first.\n";

		return;
	}

	if (!vaultEngine.getIsUnlocked()) {
		std::cerr << "Vault is not unlocked. Call 'vault unlock' first.\n";

		return;
	}

	const auto& list = db.getCredentials();

	if (list.empty()) {
		std::cerr << "Your vault is empty. Use 'vault add' to save credential.\n";

		return;
	}

	std::cout << "\n" << std::left
		<< std::setw(20) << "Website"
		<< std::setw(20) << "Username"
		<< std::setw(25) << "Created At"
		<< std::setw(30) << "Notes" << "\n";
	std::cout << std::string(95, '-') << "\n";

	for (const auto& credential : list) {
		std::time_t timeVal = static_cast<std::time_t>(credential.created_at);

		std::tm timeStruct{ };

		if (localtime_s(&timeStruct, &timeVal) == 0) {
			char timeBuffer[20];
			std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M", &timeStruct);

			std::cout << std::setw(20) << credential.website
				<< std::setw(20) << credential.username
				<< std::setw(25) << timeBuffer
				<< std::setw(30) << (credential.note.empty() ? "---" : credential.note) << "\n";
		}
		else {
			std::cout << std::setw(20) << credential.website
				<< std::setw(20) << credential.username
				<< std::setw(25) << "Unknown date"
				<< std::setw(30) << (credential.note.empty() ? "---" : credential.note) << "\n";
		}
	}
	std::cout << std::string(95, '-') << "\n\n";
}

void Command::_delete() {
	std::filesystem::path VaultPath(Common::defaultVaultPath());

	if (!std::filesystem::exists(VaultPath)) {
		std::cerr << "No vault found. Run 'vault init' first.\n";

		return;
	}

	if (!vaultEngine.getIsUnlocked()) {
		std::cerr << "Vault is not unlocked. Call 'vault unlock' first.\n";

		return;
	}

	const auto& list = db.getCredentials();

	if (list.empty()) {
		std::cerr << "Your vault is empty. Use 'vault add' to save credential.\n";

		return;
	}

	std::string website;
	std::cout << "Website: ";
	std::getline(std::cin, website);

	if (db.deleteCredential(website)) {
		std::cout << "Credential deleted successfully.\n";

		vaultEngine.save(db.getCredentials());
	}
	else {
		std::cerr << "No any credential found.\n";
	}
}

void Command::_update() {
	std::filesystem::path VaultPath(Common::defaultVaultPath());

	if (!std::filesystem::exists(VaultPath)) {
		std::cerr << "No vault found. Run 'vault init' first.\n";

		return;
	}

	if (!vaultEngine.getIsUnlocked()) {
		std::cerr << "Vault is not unlocked. Call 'vault unlock' first.\n";

		return;
	}

	const auto& list = db.getCredentials();

	if (list.empty()) {
		std::cerr << "Your vault is empty. Use 'vault add' to save credential.\n";

		return;
	}

	std::string website;
	std::cout << "Website: ";
	std::getline(std::cin, website);

	if (db.updateCredential(website)) {
		std::cout << "Credential updated successfully.\n";

		vaultEngine.save(db.getCredentials());
	}
	else {
		std::cerr << "No any credential found.\n";
	}
}

void Command::_generate() {
	int passwordLength = Common::getInt("Enter length of the password: ");

	if (passwordLength == -1) {
		std::cout << "Invalid numerical input.\n";

		return;
	}

	std::string randomPassword;

	for (size_t i = 0; i < passwordLength; i++) {
		randomPassword.push_back(Common::getPasswordCharacters()[randombytes_uniform(Common::getPasswordCharacters().size())]);
	}

	std::cout << randomPassword << '\n';
	if (Common::copyToClipboard(randomPassword)) 
		std::cout << "Password was copied clipboard.\n";
}

void Command::_help() {
	std::cout << std::left << " |\n";
	std::cout << std::setw(28) << " | - vault init" << "Create a new vault at `~/.vault/default.vault`\n |" << std::endl;
	std::cout << std::setw(28) << " | - vault unlock" << "Unlock and enter an interactive session\n |" << std::endl;
	std::cout << std::setw(28) << " | - vault add" << "Add a new credential\n |" << std::endl;
	std::cout << std::setw(28) << " | - vault get" << "Retrive credentials for a site and directly copy the password\n |" << std::endl;
	std::cout << std::setw(28) << " | - vault list" << "List all stored site names\n |" << std::endl;
	std::cout << std::setw(28) << " | - vault delete" << "Remove a credential\n |" << std::endl;
	std::cout << std::setw(28) << " | - vault update" << "Update a credential\n |" << std::endl;
	std::cout << std::setw(28) << " | - vault exit" << "Close the project\n |" << std::endl;
	std::cout << std::setw(28) << " | - vault generate" << "Generate random password\n |\n" << std::endl;
}

void Command::_exit(bool& _isRunning) {
	std::cout << "Closing vault securely...\n";

	if (vaultEngine.getKey() != nullptr)
		KDF::zero(*(vaultEngine.getKey()));

	_isRunning = false;
}
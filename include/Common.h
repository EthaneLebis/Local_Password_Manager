#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <array>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <conio.h>
#include <sodium.h>
#include <Windows.h>

class Common {

	public:

		static std::string_view trim(std::string_view _string) {
			while (!_string.empty()
				&& std::isspace(static_cast<unsigned char>(_string.front()))) {

				_string.remove_prefix(1);
			}

			while (!_string.empty()
				&& std::isspace(static_cast<unsigned char>(_string.back()))) {

				_string.remove_suffix(1);
			}

			return _string;
		}

		static std::filesystem::path defaultVaultPath() {
			char* userProfileEnv = nullptr;
			size_t size = 0;

			if (_dupenv_s(&userProfileEnv, &size, "USERPROFILE") != 0
				|| userProfileEnv == nullptr) {

				std::cerr << "Could not find USERPROFILE enviroment variable.\n";

				return std::filesystem::path(".") / ".vault" / "default.vault";
			}

			std::filesystem::path UserProfileEnv(userProfileEnv);

			free(userProfileEnv);

			return UserProfileEnv / ".vault" / "default.vault";
		}

		static std::string getPassword(const std::string& _prompt) {
			std::string password;
			char _char = 0;

			std::cout << _prompt << std::flush;

			while (true) {
				_char = _getch();

				if (_char == '\r' || _char == '\n') {
					break;
				}
				else if (_char == '\b') {
					if (!password.empty()) {
						password.pop_back();

						std::cout << "\b \b" << std::flush;
					}
				}
				else if (_char == 0 || _char == -32) {
					_getch();
				}
				else {
					password.push_back(_char);
					std::cout << static_cast<char>(219) << std::flush;
				}
			}

			std::cout << '\n';

			return password;
		}

		static bool copyToClipboard(const std::string& _text) {
			if (!OpenClipboard(nullptr))
				return false;

			EmptyClipboard();

			HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, _text.size() + 1);
			if (!hGlob) {
				CloseClipboard();
				return false;
			}

			char* pBuf = static_cast<char*>(GlobalLock(hGlob));
			if (pBuf) {
				std::memcpy(pBuf, _text.data(), _text.size());
				pBuf[_text.size()] = '\0';
				GlobalUnlock(hGlob);
			}

			if (!SetClipboardData(CF_TEXT, hGlob)) {
				GlobalFree(hGlob);
				CloseClipboard();
				return false;
			}

			CloseClipboard();
			return true;
		}
};
#pragma once

#include "./Credential.h"
#include "Vault.h"

class Command {
	Credentials db;
	Vault vaultEngine;

	public:

		void run();

		void _init();

		void _unlock();

		void _add();

		void _get();

		void _list();

		void _delete();

		void _update();

		void _help();

		void _exit(bool& isRunning);
};
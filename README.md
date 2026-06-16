# Local Password Manager
> An offline CLI vault built in C++ with Microsoft Visual Studio editor.

## Table of Contents

1. [Project Overview](#1-project-overview)
2. [User Input](#2-user-input)
	- [Subcommands](#21-subcommands)

## 1. Project Overview

A secure command-line tool that stores acccounts credentials in a single encrypted local file. Providing one password to store local files. No any cloud syns, and third-party servers.

## 2. User Input

### Subcommands

| Commands					| Description										|
|---------------------------|---------------------------------------------------|
| `vault init`				| Create a new vault at `~/.vault/default.vault`	|
| `vault unlock`			| Unlock and enter an interactive session			|
| `vault add`				| Add a new credential								|
| `vault get <site>`		| Retrive credentials for a site					|
| `vault list`				| List all stored site names						|
| `vault delete <site>`		| Remove a credential								|
| `vault export`			| Export decrypted JSON to stdout					|
| `vault help`				| Show subcommands									|


#### vault init

The program starts with vault init to create .vault file on disk. Then can add passwords and related websites. If the user not start with vault init, user can't add password. For vault init user run it just once and never run it again.


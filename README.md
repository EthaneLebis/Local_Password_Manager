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
| `vault get`       		| Retrive credentials for a site					|
| `vault list`				| List all stored credential						|
| `vault delete`			| Remove a credential								|
| `vault update`			| Update the saved credential						|
| `vault generate`			| Generates random password based on length			|
| `vault help`				| Show subcommands									|
| `vault exit`				| Exit the project									|

#### vault init

The program starts with vault init to create .vault file on disk. Then can add passwords and related websites. If the user not start with vault init, user can't add password. For vault init user run it just once and never run it again.

#### vault unlock

Each of starting the program this must run to use `vault add`, `vault get`, `vault list`, `vault delete`, `vault update`. This was added to protect the user. Using the master password was taken in `vault init` part should enter in this section. This is similar to authontication. Without knowing the master password User can't obtain credentials, and can't see .vault file data. 

#### vault add

To add new credential this can be used. Adding website, password, note, username, and create_at saved automaticly. Each time new credential saves in vector on RAM, moreover saves on disk that .vault file.  

#### vault get

Entering the website name, user can get data which was provided in `vault add`. Additianally it directly copy the password to clipboard.

#### vault list

Shows all credentials in a table without password to see possible websites and credentials to enter in `vault get`, `vault delete`, `vault update`.

#### vault delete

Entering the website name, user can remove the credential permenently from disk and RAM.

#### vault update

Using website user update provided credential. User can update first of all password.  

#### vault generate

Using randombytes_uniform() which provided by sodium.h that libsodium documentation to generate the random password. This function was used to provide that each character has same probability.

#### vault help

Shows possible subcommands can use in this project terminal.

#### vault exit

Exit the teminal that close the session. Additianally make the key value zero to provide the memmory safe.
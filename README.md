C++ Key-Value Store

A small in-memory key-value store written in C++ for learning purposes.
The project focuses on practicing multi-file C++ projects, basic command-line interaction, and clean program structure.

Build
cmake -S . -B build
cmake --build build

Run:

cd build
./kv 
./kv --help (for help)

Milestone 1–3 (server + client)
Server listens on 12345. Client connects and sends commands. Full KV over network: SET, GET, DEL, EXISTS, SIZE, KEYS, CLEAR.

Terminal 1: ./kvserver
Terminal 2: ./kvclient 127.0.0.1 12345

Usage

Run the program and use the interactive prompt:

kv> HELP
kv> EXIT


Commands

Command	              Description
SET <key> <value>	Store a value
GET <key>	        Retrieve a value (or nil)
DEL <key>	        Delete a key (returns 1/0)
EXISTS <key>	    Check if key exists (returns 1/0)
SIZE	            Number of keys
KEYS	            List all keys
CLEAR	            Remove everything
HELP	            Show commands
EXIT	            Quit

This project is mainly to get more comfortable with:

separating headers and implementations

building C++ programs with g++

writing simple interactive tools

# key-value (C++)

> In-memory key-value store. There's a local REPL (`kv`) and a TCP server + client (`kvserver` / `kvclient`) so you can hit the same commands over the network. Built for practice with C++ layout, sockets, and a little line-based protocol.

**Requirements:** CMake and a C++17 compiler. The server uses POSIX sockets — build and run under WSL or Linux (not native Windows).

---

## Build

```bash
cmake -S . -B build
cmake --build build
```

Binaries land in `build/`: `kv`, `kvserver`, `kvclient`.

---

## Local REPL

```bash
cd build
./kv
```

Prompt is `kv>`. `HELP` lists commands, `EXIT` quits.

---

## Networked mode

Terminal 1 — server listens on port 12345, accepts multiple clients:

```bash
./build/kvserver
```

Terminal 2, 3, ... — clients:

```bash
./build/kvclient 127.0.0.1 12345
```

All clients share one KVStore (SET in one client, GET in another). Server spawns a thread per connection; the store is protected by a mutex.

Same commands as below, one line per request; responses are one line each (KEYS returns keys space-separated on one line).

---

## Commands

| Command          | What it does                |
|------------------|-----------------------------|
| `SET key value`  | Store a value               |
| `GET key`        | Print value or `(nil)`      |
| `DEL key`        | `1` if deleted, else `0`    |
| `EXISTS key`     | `1` or `0`                  |
| `SIZE`           | Number of keys              |
| `KEYS`           | All keys, space-separated   |
| `CLEAR`          | Wipe the store              |
| `HELP`           | Short help text             |

`EXIT` / `quit` only applies to the local REPL and the client (not the server).

---

## Tests

Local store: `./kv --run-tests` runs a few asserts.

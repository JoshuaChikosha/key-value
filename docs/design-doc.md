# C++ Key-Value Store (Single-Threaded) — Design Doc

## 0) Goal
Build a small, single-process, in-memory key-value database with a command-line interface (REPL).
This project is meant to teach:
- C++ project structure (include/ vs src/, headers vs cpp)
- classes + constructors/destructors
- references vs copies
- const correctness
- error handling (return values / std::optional / exceptions)
- clean API design

Non-goals:
- networking, HTTP
- multithreading
- persistence (optional stretch)

## 1) User Experience
Program runs as a REPL:
- prints a prompt `kv> `
- user types commands
- program executes and prints result

### Commands
Required:
- SET <key> <value>        -> prints OK
- GET <key>                -> prints value or (nil)
- DEL <key>                -> prints 1 if deleted else 0
- EXISTS <key>             -> prints 1 or 0
- SIZE                     -> number of keys
- KEYS                     -> list all keys (any order ok)
- CLEAR                    -> remove everything
- HELP                     -> print commands
- EXIT                     -> quit program

Optional (nice stretch):
- INCR <key>               -> integer increment, error if non-int
- APPEND <key> <suffix>    -> string append
- STATS                    -> prints internal stats (ops count)

### Example session
kv> SET a 10
OK
kv> GET a
10
kv> EXISTS a
1
kv> DEL a
1
kv> GET a
(nil)
kv> SIZE
0
kv> EXIT

## 2) Data Model
Store everything in memory:
- key: std::string
- value: std::string

Use:
- std::unordered_map<std::string, std::string>

## 3) Architecture / Modules
Suggested files:

include/
- kv_store.h        (class KVStore public API)
- command.h         (Command struct + parsing result types)

src/
- main.cpp          (REPL loop, I/O)
- kv_store.cpp      (implementation)
- command.cpp       (parser implementation)

docs/
- design.md

## 4) Core Classes

### KVStore (business logic)
Responsibilities:
- store/lookup/update/delete key-value pairs
- provide methods used by main.cpp

API sketch:
class KVStore {
public:
  bool set(const std::string& key, const std::string& value);
  std::optional<std::string> get(const std::string& key) const;
  bool del(const std::string& key);
  bool exists(const std::string& key) const;
  size_t size() const;
  std::vector<std::string> keys() const;
  void clear();
private:
  std::unordered_map<std::string, std::string> map_;
};

Notes:
- Use references (`const std::string&`) to avoid copying
- Mark read-only methods as `const`

### Command parsing
A tiny parser that turns a line of user input into:
- command name
- args vector

Design:
- split by whitespace (keep it simple)
- uppercase command for matching
- support quoted values as a stretch (optional)

Command struct:
struct Command {
  std::string name;               // e.g. "GET"
  std::vector<std::string> args;  // e.g. ["a"]
};

Parsing result:
- return std::optional<Command>
- empty input -> nullopt

## 5) Error handling strategy
Keep it beginner-friendly:
- No exceptions for normal command errors.
- Print friendly errors for wrong arg counts.

Examples:
- GET with no key -> "ERR wrong number of arguments for GET"
- Unknown command -> "ERR unknown command"

## 6) Testing Strategy
No test framework required at first.
We’ll do:
- "manual" tests via REPL
- plus a `--run-tests` mode in main.cpp that runs assertions

### Core invariants
- After SET k v, GET k returns v
- DEL removes key; second DEL returns 0
- SIZE matches number of keys
- CLEAR empties everything

## 7) Milestones & Checklists

### Milestone A — Build + skeleton (0.5 day)
- [ ] Project builds and runs
- [ ] `kv --help` prints usage
- [ ] REPL prints prompt and accepts input (even if unimplemented)
- [ ] Basic folder structure committed

### Milestone B — KVStore implemented (1 day)
- [ ] Implement KVStore with unordered_map
- [ ] Unit-ish checks in `--run-tests`:
  - set/get
  - del
  - exists
  - size
  - clear

### Milestone C — Command parsing + execution (1 day)
- [ ] Parse user input into Command
- [ ] Implement HELP/EXIT
- [ ] Implement required commands (SET/GET/DEL/EXISTS/SIZE/KEYS/CLEAR)
- [ ] Proper error messages for bad args

### Milestone D — Polish (0.5–1 day)
- [ ] Clean output formatting (nil, OK, ints)
- [ ] README with build/run examples
- [ ] Add STATS or INCR (optional)

## 8) Definition of Done
- one-command build
- REPL supports all required commands
- `--run-tests` passes
- README explains:
  - what it does
  - how to build/run
  - sample session

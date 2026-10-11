# CodeVault

**A lightweight local version control system built from scratch in C.**

## Project Description

CodeVault is a lightweight, C-based version control system that allows developers to track file changes, stage files, create commits, inspect repository status, compare working files against committed versions, and restore previous versions.

The project implements core version-control concepts from scratch, including content hashing, object storage, staging, commit history, and file restoration, without relying on Git internally.

## Features

- **Repository Initialization** — Initialize a local CodeVault repository.
- **File Tracking** — Add files to the staging area.
- **Content Hashing** — Identify file contents using a custom 64-bit FNV-1a-style hash.
- **Object Storage** — Store file contents using their hashes and avoid duplicate storage.
- **Commits** — Create commits with messages and parent-commit references.
- **Repository Status** — Identify staged, committed, modified, and untracked files.
- **Diff** — Compare working files against the latest committed versions and display changed lines.
- **Restore** — Restore individual files or roll back the repository to a parent commit.
- **Ignore Rules** — Use `.codevaultignore` to exclude specified files and directories from status and staging.
- **Build Automation** — Compile the project using Make.

## Technology Stack

- **Language:** C
- **Platform:** Linux
- **Compiler:** GCC
- **Build System:** Make
- **Development Version Control:** Git

## Project Structure

```text
CodeVault/
├── include/              # Header files
├── src/                  # C source files
├── tests/                # Test programs
├── .codevaultignore      # CodeVault ignore rules
├── .gitignore            # Git ignore rules
├── Makefile              # Build automation
├── README.md             # Project documentation
└── LICENSE               # MIT License
```

## Prerequisites

- GCC
- Make
- Linux or a Linux-compatible environment, such as WSL
- Git (for developing and hosting the project)

## Build

Clone the repository and navigate into the project directory:

```bash
git clone <repository-url>
cd CodeVault
```

Compile the project:

```bash
make
```

To remove the compiled executable:

```bash
make clean
```

## Usage

Run the following commands from the project root.

### 1. Initialize a repository

```bash
./codevault init
```

Creates the `.codevault/` directory containing repository metadata, the staging index, references, and object storage.

### 2. Stage a file

```bash
./codevault add <file>
```

Adds a file to the staging area or updates its staged content hash when it changes.

### 3. Check repository status

```bash
./codevault status
```

Displays file states, including staged, committed, modified, and untracked files.

### 4. Create a commit

```bash
./codevault commit "Initial commit"
```

Stores the staged file contents and commit metadata, updates the `main` reference, and clears the staging index after a successful commit.

### 5. Compare changes

Compare all files present in the latest commit:

```bash
./codevault diff
```

Compare a specific file:

```bash
./codevault diff <file>
```

Displays added and removed lines between the committed version and the current working file.

### 6. Restore files

Restore a specific file from the current commit:

```bash
./codevault restore <file>
```

Restore all files from the current commit and move `main` to its parent commit:

```bash
./codevault restore
```

The full restore operation moves the repository's `main` reference backward one commit at a time. Single-file restoration does not change the reference.

## Ignore Rules

CodeVault uses `.codevaultignore` to define files and directories that should be ignored.

Example:

```text
codevault
*.o
*.tmp
.vscode/
.git/
```

Supported patterns depend on the current ignore implementation. The intention is to support exact filenames, wildcard patterns, and directory exclusions.

**Note:** `.codevaultignore` controls CodeVault, while `.gitignore` controls Git. They serve different purposes.

## Repository Storage

CodeVault stores its repository data inside `.codevault/`:

```text
.codevault/
├── objects/              # File contents and commit objects
├── refs/
│   └── heads/
│       └── main          # Current main commit reference
├── index                 # Staging area
└── HEAD                  # Current reference information
```

File objects are stored by content hash. Commit objects contain the commit message, parent reference, and the list of tracked filenames and their content hashes.

This design allows commits to reference existing file objects instead of duplicating every file's contents in every commit.

## Project Goals

1. Understand the internal concepts behind version-control systems.
2. Implement file tracking, staging, and persistent storage in C.
3. Apply hashing and algorithmic techniques to a practical project.
4. Practice file handling, command-line interface design, and error handling.
5. Build a modular C application with separate source and header files.
6. Strengthen systems programming and software-engineering skills.

## Current Limitations

- Local version control only; remote synchronization is not implemented.
- Only the `main` reference is currently supported.
- Branch creation, switching, and merging are not implemented.
- The custom 64-bit hash is not cryptographically secure.
- Ignore-pattern support is limited to the patterns implemented by CodeVault.
- File paths containing spaces may not be supported by the current index and commit format.

## Future Enhancements

- Commit history command.
- Branch creation and checkout.
- Merge support and conflict detection.
- More robust diff output.
- Improved file-path and large-file handling.
- Expanded testing and error handling.

## Status

**Under Development**

CodeVault is being developed incrementally, with the core repository, staging, commit, status, diff, restore, and ignore functionality being implemented in C.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

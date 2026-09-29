# CodeVault

A lightweight local version control system built from scratch in C.

## Project Description

CodeVault is a C-based local version control system designed to help developers track changes to their project files, create and manage versions, inspect project history, compare changes, and restore previous versions.

The project aims to implement the core ideas behind a version control system from scratch rather than relying on an existing version-control implementation.

## Planned Features

* Initialize a CodeVault repository
* Track project files
* Stage files for a commit
* Create commits with messages
* View commit history
* Check the current repository status
* Compare changes between versions
* Restore previous versions of files
* Branch and checkout support as an advanced feature

## Technology

* **Language:** C
* **Platform:** Linux
* **Build System:** Make
* **Version Control:** Git

## Project Structure

```text
CodeVault/
├── include/        # Header files
├── src/            # C source files
├── tests/          # Test programs
├── Makefile        # Build automation
├── README.md       # Project documentation
├── LICENSE         # Project license
└── .gitignore      # Ignored files
```

## Prerequisites

* GCC
* Make
* Linux or a Linux-compatible environment
* Git

## Build

Build instructions will be added as the implementation progresses.

## Usage

The planned command-line interface will provide commands such as:

```bash
./codevault init
./codevault add <file>
./codevault status
./codevault commit "Commit message"
./codevault log
./codevault diff
./codevault restore <commit>
```

Branching and merging will be considered as advanced features after the core functionality is complete.

## Project Goals

The main goals of CodeVault are:

1. Understand how version-control systems work internally.
2. Implement file tracking and version storage in C.
3. Apply data structures and algorithms to a practical problem.
4. Practice file handling, command-line interfaces, and persistent storage.
5. Build a complete C project following good software-engineering practices.

## Status

**Under Development**

The project is being developed incrementally, starting with repository initialization and progressing toward file tracking, commits, history, comparison, and restoration.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

# 🐚 Minishell

A simplified Bash-like shell written in C — built from scratch as part of the 42 Network curriculum.

> “As beautiful as a shell.”

---

## 📌 Project Overview

**Minishell** is a fully functional Unix shell that mimics key behaviors of Bash.  
It parses and executes commands, handles redirections and pipes, manages environment variables, and supports built-in commands and signal handling.

This project was a deep dive into:

- Process management (`fork`, `execve`, `wait`)
- File descriptor manipulation (`dup`, `pipe`)
- Terminal I/O
- Parsing and quoting logic
- Signal handling (e.g., `Ctrl-C`, `Ctrl-D`)
- Error handling and memory management in a long-running program

---

## 🧠 Key Features

### ✅ Mandatory Implementation

- Prompt that waits for user input
- Command history via `readline`
- Command execution from `PATH`, relative, or absolute paths
- Quote handling:
  - `'single quotes'`: literal interpretation
  - `"double quotes"`: allows `$` expansion
- **Environment variable expansion**:
  - `$VAR`
  - `$?` (last command’s exit status)
- **Redirections**:
  - `<`: input redirection
  - `>`: output redirection (overwrite)
  - `>>`: output redirection (append)
  - `<<`: heredoc with delimiter
- **Pipes** (`|`) for command chaining
- **Built-in commands**:
  - `echo [-n]`
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- **Signal handling**:
  - `Ctrl-C`: resets prompt
  - `Ctrl-D`: exits shell
  - `Ctrl-\`: ignored

---

## ⚙️ How It Works

### 🟢 Startup

- Displays a shell prompt
- Waits for user input
- Parses input into tokens and builds execution structures

### ⚙️ Execution

- Runs built-ins directly
- Forks and executes external commands via `execve`
- Sets up pipes and redirections
- Expands variables using the current environment

---

## 🛠 Installation & Execution

### 📦 Dependencies

Before compiling `minishell`, make sure you have the required libraries installed:

```bash
sudo apt-get install libreadline-dev build-essential
```

## 🧱 Compilation

```bash
make

```

## ▶️ Running the Shell

```bash
./minishell

```

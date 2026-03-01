# fdup - Fast Duplicate File Finder

A lightweight, efficient command-line tool for finding and managing duplicate files in your directories.

## Features

- **Fast Detection** - Uses a two-pass approach: first matches files by size, then verifies duplicates by content hash
- **Recursive Search** - Scan entire directory trees with the `-r` flag
- **Interactive Deletion** - Safely clean up duplicates with interactive prompts (`-d` flag)
- **Zero Dependencies** - Pure C++17, no external libraries required
- **Cross-Platform** - Works on Linux, macOS, and other Unix-like systems

## Installation

### Building from Source

**Prerequisites:**

- C++17 compatible compiler (g++, clang++)
- Make

**Build and Install:**

```bash
# Clone or download the repository
git clone <repository-url>
cd find-duplicates

# Build the binary
make

# (Optional) Install system-wide
sudo make install
```

The `make install` command installs `fdup` to `/usr/local/bin/`, making it available globally.

### Uninstall

```bash
sudo make uninstall
```

### Manual Installation

If you prefer not to use `make install`, you can run the binary directly:

```bash
# Build only
make

# Run from current directory
./fdup <directory>
```

## Usage

```bash
fdup [options] <directory>
```

### Options

- `-r, --recursive` - Search recursively through all subdirectories
- `-d, --delete` - Enable interactive deletion mode

### Examples

**Find duplicates in a single directory:**

```bash
fdup ~/Downloads
```

**Find duplicates recursively:**

```bash
fdup -r ~/Documents
```

**Find and interactively delete duplicates:**

```bash
fdup -d ~/Downloads
```

**Recursive search with interactive deletion:**

```bash
fdup -r -d ~/Pictures
```

### Interactive Deletion Mode

When using the `-d` flag, fdup will prompt you for each set of duplicates:

```
Which one do you wanna keep?
 [1] /path/to/file1.jpg
 [2] /path/to/file2.jpg
 [3] /path/to/file3.jpg
```

Enter the number corresponding to the file you want to keep. All other duplicates will be deleted.

## Contributing

Contributions are welcome! Feel free to:

- Report bugs
- Suggest new features
- Submit pull requests

## Author

[chungchihhan/harry@gmail.com]

# Pipex

A C implementation of shell-style pipelines using processes, pipes, and file-descriptor redirection. Developed as part of the 42 curriculum.

## Features

- Connects multiple commands into a pipeline.
- Redirects the first command's input from a file and the last command's output to a file.
- Includes command-path resolution, quoted-argument parsing, and a `here_doc` mode.
- Uses a bundled C utility library in `mylib/`.

## Build

Requirements: a C compiler, Make, and a POSIX system.

```sh
make
```

This builds the utility library and the `pipex` executable.

## Usage

```sh
./pipex infile "grep hello" "wc -l" outfile
```

This corresponds to the pipeline:

```sh
< infile grep hello | wc -l > outfile
```

Additional commands can be passed before the output file:

```sh
./pipex infile "cat" "grep hello" "wc -l" outfile
```

Here-document mode takes exactly two commands in the current argument check:

```sh
./pipex here_doc END "cat" "wc -l" outfile
```

Enter input and finish with the delimiter `END`. This is a pipeline exercise rather than a complete shell; it does not provide a full shell language.

## Layout and cleanup

- `src/pipex.c`: argument handling, pipe allocation, and process creation.
- `src/childs.c`, `src/get_cmds.c`: execution and command lookup.
- `src/heredoc.c`: here-document handling.
- `src/ft_split_quote.c`: argument splitting.
- `inc/`: shared declarations.

```sh
make clean
make fclean
make re
```

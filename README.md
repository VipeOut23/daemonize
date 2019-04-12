# Daemonize

A little daemonizing program wrapper for POSIX

## Installing

```
$ make && make install
```

Targets: 
  - daemonize (default)
  - install
  - uninstall
  - clean
Environment:
  - PREFIX (un-/install prefix | def: ~/.local)

## Usage

``` shell
$ daemonize <program> <args ...>
```

The PID of the daemon is printed to stdout

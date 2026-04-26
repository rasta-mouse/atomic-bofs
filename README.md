## Atomic BOFs

Atomic BOFs is my attempt at providing repeatable, atomic test units for BOF execution.  The goal was to provide an easy means of running BOFs outside of a C2 framework, whilst maintaining the equivalent functionality.

### Harness

The harness is responsible for loading a COFF into memory, calling its entry point, and passing any packed arguments. Users should not have to modify the code in this directory, as it's customised using `@config.spec` files from your test units.

### Test Units

Each test unit requires a `config.spec` file that sets the following variables:

- `$COFF` is the raw bytes of the BOF to load.  You'd typically grab this using Crystal Palace's `load` command.
- `$ARGS` are packed arguments for the BOF.  Use Crystal Palace's `pack` command to form these.

> [!NOTE]
> You are required to pack the length of a value (such as strings) as required by the Beacon APIs.

- `%entrypoint` is the name of the BOFs entrypoint.  This is typicall `go` for x64 BOFs and `_go` for x86.

This repo provides the following examples units:

#### test

The `test` directory contains a simple BOF that prints output using the `BeaconPrintf` API.  The `config.spec` file packs the string to be printed.

#### whoami

The `trustedsec` directory demonstrates how custom evasion tradecraft can be weaved into existing 3rd-party BOFs.  It uses TrustedSec's whoami BOF as an example.

> [!NOTE]
> These BOFs are not included in this repo because of their GPL-2.0 licensing.  You can download them from [their repo](https://github.com/trustedsec/CS-Situational-Awareness-BOF/tree/master/SA/whoami).

The `config.spec` file intercepts the `export` command in the harness' spec file to merge and hooks a Win32 API before the final loader is built.

### Further reading

- [https://rastamouse.me/atomic-bofs/](https://rastamouse.me/atomic-bofs/)

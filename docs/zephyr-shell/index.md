# Enabling Zephyr Shell

Zephyr includes interactive shells that lets you experiment and debug.

## Base Shell
The base shell can be configured either in the `prj.conf` file or at build time using command line arguments.

```text
# prj.conf
CONFIG_SHELL=y
```

```bash
# West Build using custom parameters
west build -p -b m5stack_core2/esp32/procpu samples/hello_world -- -DCONFIG_SHELL=y
```

You can then access it from the console and interact with it.

*Note: Anything written on the console output will mix with the shell. Consequently, it is easier to work with a "quiet" software such as the Hello World sample.*

```bash
west espressif monitor
```

When the prompt is ready, you can type `help` command to see available features:

```text
Available commands:
  clear    : Clear screen.
  device   : Device commands
  devmem   : Read/write physical memory
            Usage:
            Read memory at address with optional width:
            devmem address [width]
            Write memory at address with mandatory width and value:
            devmem address <width> <value>
  help     : Prints the help message.
  history  : Command history.
  kernel   : Kernel commands
  rem      : Ignore lines beginning with 'rem '
  resize   : Console gets terminal screen size or assumes default in case the
            readout fails. It must be executed after each terminal width change
            to ensure correct text display.
  retval   : Print return value of most recent command
  shell    : Useful, not Unix-like shell commands.

```

Beside `device list` which lists the configured devices and their status, the features are quite limited but you can extend shell functionalities.

## Regulator Shell

```text
# prj.conf
CONFIG_REGULATOR_SHELL=y

# West command line
-DCONFIG_REGULATOR_SHELL=y
```

```text
regulator - Regulator playground
Subcommands:
  enable    : Enable regulator
             Usage: enable <device>
  disable   : Disable regulator
             Usage: disable <device>
  vlist     : List all supported voltages
             Usage: vlist <device>
  vset      : Set voltage
             Input requires units, e.g. 200mv, 20.5mv, 10uv, 1v...
             Usage: vset <device> <minimum> [<maximum>]
             If maximum is not set, exact voltage will be requested
  vget      : Get voltage
             Usage: vget <device>
  clist     : List all supported current limits
             Usage: clist <device>
  iset      : Set current limit
             Input requires units, e.g. 200ma, 20.5ma, 10ua, 1a...
             Usage: iset <device> <minimum> [<maximum>]If maximum is not set,
             exact current will be requested
  iget      : Get current limit
             Usage: iget <device>
  modeset   : Set regulator mode
             Usage: modeset <device> <mode identifier>
  modeget   : Get regulator mode
             Usage: modeget <device>
  adset     : Set active discharge
             Usage: adset <device> <enable/disable>
  adget     : Get active discharge
             Usage: adget <device>
  errors    : Get errors
             Usage: errors <device>
  dvsset    : Set regulator dynamic voltage scaling state
             Usage: dvsset <device> <state identifier>
  shipmode  : Enable regulator ship mode
             Usage: shipmode <device>
```

Use `device list` to get device names.

## I2C Shell

```text
# prj.conf
CONFIG_I2C_SHELL=y

# West command line
-DCONFIG_I2C_SHELL=y
```

```text
i2c - I2C commands
Subcommands:
  scan        : Scan I2C devices
               Usage: scan <device>
  recover     : Recover I2C bus
               Usage: recover <device>
  read        : Read bytes from an I2C device
               Usage: read <device> <addr> <reg> [<bytes>]
  read_byte   : Read a byte from an I2C device
               Usage: read_byte <device> <addr> <reg>
  write       : Write bytes to an I2C device
               Usage: write <device> <addr> <reg> [<byte1>, ...]
  write_byte  : Write a byte to an I2C device
               Usage: write_byte <device> <addr> <reg> <value>
  speed       : Configure I2C bus speed
               Usage: speed <device> <speed>
```

Use `device list` to get device names.

# hyprgrid

Experimental Hyprland plugin for treating numeric workspaces as a fixed 10x10 grid.

The plugin registers Lua callbacks under `hl.plugin.grid` and maps grid coordinates to workspace IDs:

```text
workspace_id = (row - 1) * 10 + col
```

## Status

Alpha / personal-use ready. Hyprland plugins are ABI-sensitive, so rebuild this plugin after Hyprland updates and use it with the Hyprland version it was compiled against.

## Features

- Switch to a column while keeping the current row.
- Switch to a row while keeping the current column.
- Move horizontally by a relative column delta.
- Move the focused window to a column in the current row.
- Direction-aware animation styles for horizontal/vertical movement.

## Requirements

- Hyprland headers / development package
- `pkg-config`
- C++ compiler with C++23 support
- Development packages used by Hyprland headers, including `pixman-1`, `libdrm`, `pangocairo`, `libinput`, `libudev`, `wayland-server`, `xkbcommon`, and Lua headers

## Build

```sh
make
```

This creates:

```text
gridworkspace.so
```

## Install

```sh
make install
```

By default this installs to:

```text
~/.config/hypr/plugins/gridworkspace.so
```

You can override the install directory:

```sh
make install INSTALL_DIR=/absolute/path/to/plugins
```

Then load the plugin from your Hyprland config using an absolute path, for example:

```ini
plugin = /home/you/.config/hypr/plugins/gridworkspace.so
```

Restart or reload Hyprland after changing plugin configuration.

## Lua API

After the plugin is loaded, these callbacks are available from Hyprland's Lua config environment:

```lua
hl.plugin.grid.col(col)        -- switch to column col in the active row, col: 1..10
hl.plugin.grid.row(row)        -- switch to row row in the active column, row: 1..10
hl.plugin.grid.col_rel(delta)  -- move horizontally by delta columns, clamped to 1..10
hl.plugin.grid.move(col)       -- move focused window to column col in the active row, col: 1..10
```

Examples:

```lua
hl.plugin.grid.row(2)
hl.plugin.grid.col(5)
hl.plugin.grid.col_rel(-1)
hl.plugin.grid.move(4)
```

## Development

```sh
make clean
make
```

Build artifacts such as `*.so` are intentionally ignored by git. Publish source in the repository and attach compiled `.so` files to GitHub releases only when they are tied to a specific Hyprland version / ABI.

## License

MIT

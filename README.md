# hyprgrid

[![Build](https://github.com/lostf1sh/hyprgrid/actions/workflows/build.yml/badge.svg)](https://github.com/lostf1sh/hyprgrid/actions/workflows/build.yml)

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

## Install with hyprpm

```sh
hyprpm add https://github.com/lostf1sh/hyprgrid
hyprpm enable hyprgrid
hyprpm reload
```

After Hyprland updates, rebuild/update plugins:

```sh
hyprpm update
hyprpm reload
```

## Manual build

```sh
make
```

This creates:

```text
gridworkspace.so
```

## Manual install

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

## Keybind examples

These examples are for Hyprland's Lua config (`hyprland.lua`). Wrap plugin calls in functions so they run when the keybind is pressed, not while the config is loading.

```lua
local mainMod = "SUPER"

-- SUPER + [1-0]: switch column in the current row.
-- SUPER + SHIFT + [1-0]: move focused window to column in the current row.
-- SUPER + CTRL + [1-0]: switch row while keeping the current column.
for i = 1, 10 do
    local key = i % 10 -- 10 maps to key 0

    hl.bind(mainMod .. " + " .. key, function()
        hl.plugin.grid.col(i)
    end)

    hl.bind(mainMod .. " + SHIFT + " .. key, function()
        hl.plugin.grid.move(i)
    end)

    hl.bind(mainMod .. " + CTRL + " .. key, function()
        hl.plugin.grid.row(i)
    end)
end

-- Horizontal movement across columns.
hl.bind(mainMod .. " + left", function()
    hl.plugin.grid.col_rel(-1)
end)

hl.bind(mainMod .. " + right", function()
    hl.plugin.grid.col_rel(1)
end)
```

## Development

```sh
make clean
make
```

Build artifacts such as `*.so` are intentionally ignored by git. Publish source in the repository and attach compiled `.so` files to GitHub releases only when they are tied to a specific Hyprland version / ABI.

## License

MIT

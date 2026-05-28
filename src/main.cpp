#include <src/includes.hpp>
#include <src/Compositor.hpp>
#include <src/helpers/Monitor.hpp>
#include <src/desktop/Workspace.hpp>
#include <src/desktop/state/FocusState.hpp>
#include <src/managers/KeybindManager.hpp>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include "globals.hpp"

APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

namespace {
    constexpr int GRID_COLS    = 10;
    constexpr int GRID_ROWS    = 10;
    int           g_currentRow = 1;
    int           g_currentCol = 1;

    int wsId(int row, int col) {
        return (row - 1) * GRID_COLS + col;
    }

    void syncFromActive() {
        const auto pMon = Desktop::focusState()->monitor();
        if (!pMon || !pMon->m_activeWorkspace)
            return;
        const int id = pMon->m_activeWorkspace->m_id;
        if (id < 1 || id > GRID_COLS * GRID_ROWS)
            return; // outside the grid (e.g. special workspace); keep last known
        g_currentRow = ((id - 1) / GRID_COLS) + 1;
        g_currentCol = ((id - 1) % GRID_COLS) + 1;
    }

    void applyStyle(int target, const std::string& style) {
        auto pWs = g_pCompositor->getWorkspaceByID(target);
        if (!pWs) {
            const auto pMon = Desktop::focusState()->monitor();
            if (pMon)
                pWs = g_pCompositor->createNewWorkspace(target, pMon->m_id, std::to_string(target), true);
        }
        if (pWs)
            pWs->m_animationStyle = style;
    }

    void doSwitch(int row, int col, const std::string& style) {
        const int target = wsId(row, col);
        applyStyle(target, style);
        g_pKeybindManager->m_dispatchers["workspace"](std::to_string(target));
    }

    void doMove(int row, int col, const std::string& style) {
        const int target = wsId(row, col);
        applyStyle(target, style);
        g_pKeybindManager->m_dispatchers["movetoworkspace"](std::to_string(target));
    }
}

static int luaCol(lua_State* L) {
    const int col = (int)luaL_checkinteger(L, 1);
    if (col < 1 || col > GRID_COLS) {
        lua_pushstring(L, "grid.col: out of range (1..10)");
        return lua_error(L);
    }
    syncFromActive();
    g_currentCol = col;
    doSwitch(g_currentRow, col, "slidefade 15%");
    return 0;
}

static int luaRow(lua_State* L) {
    const int row = (int)luaL_checkinteger(L, 1);
    if (row < 1 || row > GRID_ROWS) {
        lua_pushstring(L, "grid.row: out of range (1..10)");
        return lua_error(L);
    }
    syncFromActive();
    g_currentRow = row;
    doSwitch(row, g_currentCol, "slidefadevert 15%");
    return 0;
}

static int luaColRel(lua_State* L) {
    const int delta = (int)luaL_checkinteger(L, 1);
    syncFromActive();
    int next = g_currentCol + delta;
    if (next < 1)         next = 1;
    if (next > GRID_COLS) next = GRID_COLS;
    if (next == g_currentCol)
        return 0;
    g_currentCol = next;
    doSwitch(g_currentRow, next, "slidefade 15%");
    return 0;
}

static int luaMove(lua_State* L) {
    const int col = (int)luaL_checkinteger(L, 1);
    if (col < 1 || col > GRID_COLS) {
        lua_pushstring(L, "grid.move: out of range (1..10)");
        return lua_error(L);
    }
    syncFromActive();
    g_currentCol = col;
    doMove(g_currentRow, col, "slidefade 15%");
    return 0;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    HyprlandAPI::addLuaFunction(PHANDLE, "grid", "col",     ::luaCol);
    HyprlandAPI::addLuaFunction(PHANDLE, "grid", "row",     ::luaRow);
    HyprlandAPI::addLuaFunction(PHANDLE, "grid", "move",    ::luaMove);
    HyprlandAPI::addLuaFunction(PHANDLE, "grid", "col_rel", ::luaColRel);

    return {"gridworkspace", "2D workspace grid with direction-aware animations", "f1sh", "0.1"};
}

APICALL EXPORT void PLUGIN_EXIT() {
}

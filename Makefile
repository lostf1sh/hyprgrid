CXX ?= g++
PKG_CONFIG ?= pkg-config

TARGET ?= gridworkspace.so
SRC := src/main.cpp
INSTALL_DIR ?= $(HOME)/.config/hypr/plugins

PKGS := hyprland pixman-1 libdrm pangocairo libinput libudev wayland-server xkbcommon

CPPFLAGS += $(shell $(PKG_CONFIG) --cflags $(PKGS))
CXXFLAGS ?= -O2 -g -std=c++23
CXXFLAGS += -shared -fPIC -fno-gnu-unique -Wno-c++11-narrowing
LDLIBS += $(shell $(PKG_CONFIG) --libs pangocairo)

all: $(TARGET)

$(TARGET): $(SRC) src/globals.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LDLIBS)

install: $(TARGET)
	install -Dm755 $(TARGET) $(INSTALL_DIR)/$(TARGET)

uninstall:
	rm -f $(INSTALL_DIR)/$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean install uninstall

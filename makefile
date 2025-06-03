PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
SYSTEMD_UNIT_DIR = /lib/systemd/system

APP = ocean_daemon
SERVICE = ocean_daemon.service

all: ocean_daemon shipctl

ocean_daemon: ocean_daemon.cpp
	g++ -std=c++17 -o ocean_daemon ocean_daemon.cpp

shipctl: shipctl.cpp
	g++ -std=c++17 -o shipctl shipctl.cpp

clean:
	rm -f ocean_daemon shipctl

install: ocean_daemon shipctl
	install -Dm755 shipctl $(DESTDIR)$(BINDIR)/shipctl
	install -Dm755 $(APP) $(DESTDIR)$(BINDIR)/$(APP)
	install -Dm644 $(SERVICE) $(DESTDIR)$(SYSTEMD_UNIT_DIR)/$(SERVICE)
	@echo "Reloading systemd daemon..."
	systemctl daemon-reload
	@echo "Enabling and starting $(SERVICE)..."
	systemctl enable --now $(SERVICE)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(APP)
	rm -f $(DESTDIR)$(SYSTEMD_UNIT_DIR)/$(SERVICE)
	systemctl daemon-reload
	systemctl disable --now $(SERVICE)

.PHONY: all clean
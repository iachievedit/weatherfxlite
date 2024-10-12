#!/usr/bin/env bash

# Install the built binary
if [ ! -f build/weatherfxLite ]; then
  echo "Error:  build/weatherfxLite does not exist."
  echo "To build weatherfxLite, run build.sh first."
  exit 1
fi
sudo cp build/weatherfxLite /usr/local/bin/weatherfxLite

# Add this line dtoverlay=vc4-kms-dpi-hyperpixel4sq to the end of the
# /boot/firmware/config.txt if it is not present already by:
sudo grep -q '^dtoverlay=vc4-kms-dpi-hyperpixel4sq' /boot/firmware/config.txt \
  || echo 'dtoverlay=vc4-kms-dpi-hyperpixel4sq' \
  | sudo tee -a /boot/firmware/config.txt > /dev/null

# Create a lxsession profile to remove the desktop and the panel, Stop it from sleeping etc
mkdir -p ~/.config/lxsession/LXDE-pi
echo -e '@xset s off\n@xset -dpms\n@xset s noblank' > \
~/.config/lxsession/LXDE-pi/autostart

# Create a systemd service for the weatherfxlite application like this based on
# your user/group and call it something like weatherfxlite.service
# (this assumes you have your build/executable at /usr/local/bin/weatherfxLite)
USER=`whoami`
GROUP=`groups $USER | awk '{print $3}'`
sudo tee /etc/systemd/system/weatherfxlite.service > /dev/null <<EOF
[Unit]
Description = weatherfxlite
After = graphical-session.target network-online.target

[Service]
User = $USER
Group = $GROUP
Type = simple
Environment=DISPLAY=:0
Environment=XAUTHORITY=/home/$USER/.Xauthority
WorkingDirectory = /usr/local/bin/
ExecStart = /usr/local/bin/weatherfxLite
Restart = always
RestartSec = 3

[Install]
WantedBy = multi-user.target
EOF

# New for Debian Bookworm with Wayland
mkdir -p /tmp/wayfire-plugins-extra
pushd /tmp/wayfire-plugins-extra
wget --quiet https://github.com/seffs/wayfire-plugins-extra-raspbian/releases/download/v0.7.5/wayfire-plugins-extra-raspbian-aarch64.tar.xz
tar -xf wayfire-plugins-extra-raspbian-aarch64.tar.xz
sudo cp ./usr/lib/aarch64-linux-gnu/wayfire/libhide-cursor.so /usr/lib/aarch64-linux-gnu/wayfire/
sudo cp ./usr/share/wayfire/metadata/hide-cursor.xml /usr/share/wayfire/metadata/
popd

# Add the hide-cursor plugin to the wayfire.ini
sudo apt-get install -y crudini
crudini --set ~/.config/wayfire.ini core plugins hide-cursor

# Enable and start the service
sudo systemctl daemon-reload
sudo systemctl enable weatherfxlite
sudo systemctl start weatherfxlite
sudo systemctl status weatherfxlite



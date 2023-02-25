#!/usr/bin/env bash

# install Hyper pixel 4 software by reading appropriate information from this page for your particular type of display: https://github.com/pimoroni/hyperpixel4. For example, for square display with raspberry PI 4 do this and follow the prompts properly:
# curl -sSL get.pimoroni.com/hyperpixel4-legacy | bash

# Add this line dtoverlay=vc4-kms-dpi-hyperpixel4sq to the end of the /boot/config.txt if it is not present already by:
sudo grep -qxF 'dtoverlay=vc4-kms-dpi-hyperpixel4sq' /boot/config.txt || echo 'dtoverlay=vc4-kms-dpi-hyperpixel4sq' | sudo tee -a /boot/config.txt > /dev/null

# Clean up /boot/cmdline.txt so that it removes console=tty1 and adds logo.nologo vt.global_cursor_default=0 to the end of the line
sudo sed -i 's/console=tty1 //; /logo\.nologo vt\.global_cursor_default=0/! s/$/ logo.nologo vt.global_cursor_default=0/' /boot/cmdline.txt

#Remove the mouse by updating the file /etc/lightdm/lightdm.conf to include this line xserver-command=X -nocursor under the [Seat:*] section by doing:
sudo grep -q -F 'xserver-command=X -nocursor' /etc/lightdm/lightdm.conf || sudo sed -i '/^\[Seat:\*\]$/a xserver-command=X -nocursor' /etc/lightdm/lightdm.conf

#Create a lxsession profile to remove the desktop and the panel, Stop it from sleeping etc
mkdir -p ~/.config/lxsession/LXDE-pi && echo -e '@xset s off\n@xset -dpms\n@xset s noblank' > ~/.config/lxsession/LXDE-pi/autostart

# Create a systemd service for the weatherfxlite application like this based on your user/group and call it something like weatherfxlite.service (this assumes you have your build/executable at /usr/local/bin/weatherfxlite)
sudo tee /etc/systemd/system/weatherfxlite.service > /dev/null <<EOF
[Unit]
Description = weatherfxlite
After = graphical-session.target network-online.target

[Service]
User = pi
Group = pi
Type = simple
Environment=DISPLAY=:0
Environment=XAUTHORITY=/home/pi/.Xauthority
WorkingDirectory = /usr/local/bin/
ExecStart = /usr/local/bin/weatherfxLite
Restart = always
RestartSec = 3

[Install]
WantedBy = multi-user.target
EOF

#Enable and start this service and enjoy a clutter-free weather app

sudo systemctl daemon-reload
sudo systemctl enable weatherfxlite
sudo systemctl start weatherfxlite
sudo systemctl status weatherfxlite # check the status

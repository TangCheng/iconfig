#!/bin/sh

echo -e "\e[1mStarting SOFT-RESET...\e[0m"

# kill the process which is using the database
fuser -k /data/configuration.sqlite3

sed -r -f /apps/iconfig/config/soft-reset.sed \
    /apps/iconfig/config/defconfig.sql | \
    sqlite3 /data/configuration.sqlite3

echo -e "\e[32;1mSOFT-RESET complete, now rebooting system...\e[0m"

reboot

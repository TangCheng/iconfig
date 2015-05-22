#!/bin/sh

echo -e "\e[1mStarting SOFT-RESET...\e[0m"

db_file="/data/configuration.sqlite3"

# kill the process which is using the database
fuser -k ${db_file}

model=$(sqlite3 ${db_file} "select value from base_info where name='model'")

echo -e "model:\e[1m${model}\e[0m"

defconfig="/apps/iconfig/config/defconfig.${model}.sql"
if [ ! -f ${defconfig} ]; then
  defconfig="/apps/iconfig/config/defconfig.sql"
fi

sed -r -f /apps/iconfig/config/soft-reset.sed \
    ${defconfig} | \
    sqlite3 ${db_file}

echo -e "\e[32;1mSOFT-RESET complete, now rebooting system...\e[0m"

reboot

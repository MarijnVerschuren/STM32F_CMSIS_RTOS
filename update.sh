# shellcheck disable=SC2045

pio init --ide clion
rm -rf update
git clone https://github.com/MarijnVerschuren/STM32F_CMSIS.git update

for file in $(ls update/src); do
    if [ $file != "main.c" ]; then
      cp -f "update/src/$file" "src/$file"
    fi
done
for file in $(ls update/include); do
    cp -f "update/include/$file" "include/$file"
done
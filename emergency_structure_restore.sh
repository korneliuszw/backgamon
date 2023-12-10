#!/usr/bin/env bash

mkdir -p include
mkdir -p src
mkdir -p history
mkdir -p saves

move_and_uppercase() {
    local source_extension=$1
    local destination_directory=$2
    for file in *.$source_extension; do
        filename=$(basename -- "$file" ".$source_extension")
        new_filename="$(tr '[:lower:]' '[:upper:]' <<< ${filename:0:1})${filename:1}"
        mv "$file" "$destination_directory/$new_filename.$source_extension"
        echo "Moved $file to $destination_directory/$new_filename.$source_extension"
    done
}
move_and_uppercase "c" "src"
move_and_uppercase "h" "include"

move () {
    local source_extension=$1
    local destination_directory=$2
    for file in *.$source_extension; do
        filename=$(basename -- "$file" ".$source_extension")
        mv "$file" "$destination_directory/$file"
        echo "Moved $file to $destination_directory/$file"
    done
}

move save saves
move hist history
mv cmakelists.txt CMakeLists.txt

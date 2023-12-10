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
        new_filename="${filename^}"
        echo $new_filename
        mv "$file" "$destination_directory/$new_filename.$source_extension"
        echo "Moved $file to $destination_directory/$new_filename.$source_extension"
    done
}

# Move *.c files to directory 'src'
move_and_uppercase "c" "src"

# Copy *.h files to directory 'include'
move_and_uppercase "h" "include"

mv *.save saves/
mv *.hist history/
mv cmakelists.txt CMakeLists.txt
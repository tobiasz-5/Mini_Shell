#!/bin/bash

# Cartella contenente i file .c
source_folder="/home/tobiasz/ms-tobia"

# File di output
output_file="${source_folder}/output.txt"

# Pulizia del file di output, se esiste già
> "$output_file"

# Ciclo su ogni file .c nella cartella
for file in "$source_folder"/*.c; do
    # Verifica che il file esista e sia un file regolare
    if [ -f "$file" ]; then
        echo "Processing $file..." >> "$output_file"
        # Aggiungi le righe dopo la 12° al file di output
        tail -n +13 "$file" >> "$output_file"
        echo -e "\n--- End of $file ---\n" >> "$output_file"
    fi
done

echo "Operazione completata. Il contenuto è stato salvato in $output_file"

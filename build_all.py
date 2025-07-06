#!/usr/bin/env python3

import subprocess
import argparse
import sys

# ===============================
# 1) Parse des arguments
# ===============================
parser = argparse.ArgumentParser(description="Build & run simulator with dynamic config")

parser.add_argument(
    "--decoder",
    type=str,
    choices=["dec1", "dec2", "dec3", "dec4"],
    required=True,
    help="Nom du décodeur : dec1, dec2, dec3 ou dec4"
)

parser.add_argument(
    "--platform",
    type=str,
    required=True,
    help="Nom de la plateforme (chaîne de caractères)"
)

args = parser.parse_args()
decoder = args.decoder
platform = args.platform

# ===============================
# 2) Paramètres généraux
# ===============================
CONFIG_FILE = "config_code.hpp"
N_values = [2 ** i for i in range(3, 11)]  # 8 à 1024
GF = 64
simulator_exec = f"./simulator_{decoder}"

print(f"=== Script lancé avec : decoder={decoder} | platform={platform} ===")

# ===============================
# 3) Boucle sur N
# ===============================
for N in N_values:
    print(f"\n=== Génération pour N = {N} ===")

    # Génération du fichier config_code.hpp
    content = f"""#ifndef CONFIG_CODE_H
#define CONFIG_CODE_H
#include "codes/N{N}_GF{GF}.hpp"
#endif
"""
    with open(CONFIG_FILE, "w") as f:
        f.write(content)
    print(f"✅ {CONFIG_FILE} généré pour N = {N}, GF = {GF}.")

    # Compilation avec make
    print("Compilation en cours...")
    try:
        subprocess.run(["make"], check=True)
        print(f"✅ Compilation pour N = {N} terminée avec succès.")
    except subprocess.CalledProcessError:
        print(f"❌ Erreur lors de la compilation pour N = {N}. Arrêt du script.")
        sys.exit(1)

    # Exécution du simulateur + log
    output_file = f"{decoder}_N{N}_GF{GF}_{platform}.log"
    print(f"Exécution de {simulator_exec} ... (log : {output_file})")

    with open(output_file, "w") as outfile:
        try:
            subprocess.run([simulator_exec], stdout=outfile, stderr=subprocess.STDOUT, check=True)
            print(f"✅ Exécution terminée. Trace sauvegardée dans {output_file}")
        except subprocess.CalledProcessError:
            print(f"❌ Erreur lors de l'exécution pour N = {N}. Arrêt du script.")
            sys.exit(1)

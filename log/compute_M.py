import sys

def lire_fichier(nom):
    with open(nom, "r") as f:
        lignes = f.readlines()
    return lignes

def main(f1, f2, f_out):
    lignes1 = lire_fichier(f1)
    lignes2 = lire_fichier(f2)

    # Écriture du fichier de sortie
    with open(f_out, "w") as out:
        # Copier l’en-tête tel quel
        out.write(lignes1[0])

        # Parcours des données (on saute l’en-tête)
        for l1, l2 in zip(lignes1[1:], lignes2[1:]):
            c1 = l1.split()
            c2 = l2.split()

            # Conversion en float pour Coded et Info
            coded = float(c1[4]) / float(c2[4])
            info  = float(c1[5]) / float(c2[5])

            # Reconstruction de la ligne
            nouvelle_ligne = (
                f"{c1[0]:>4} {c1[1]:>4} {c1[2]:>4} {c1[3]:>4} "
                f"{coded:8.4f} {info:8.4f} "
                f"{c1[6]:>5} {c1[7]:>7} {c1[8]:>8}\n"
            )

            out.write(nouvelle_ligne)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage : python script.py fichier1 fichier2 fichier_sortie")
        sys.exit(1)

    main(sys.argv[1], sys.argv[2], sys.argv[3])

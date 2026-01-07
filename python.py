import sys
import os
import matplotlib.pyplot as plt

def lire_points(chemin):
    points = []
    with open(chemin, "r", encoding="utf-8") as fichier:
        for ligne_brute in fichier:
            ligne = ligne_brute.strip()
            if ligne == "":
                continue

            if "," in ligne:
                morceaux = ligne.split(",")
                if len(morceaux) >= 8:
                    try:
                        x = int(float(morceaux[2].strip()))
                        temps_ms = float(morceaux[4].strip()) * 1000.0
                        mem_octets = int(float(morceaux[7].strip()))
                        points.append((x, temps_ms, mem_octets))
                    except:
                        pass
                continue

            morceaux = ligne.split()
            if len(morceaux) >= 3:
                try:
                    x = int(float(morceaux[0]))
                    temps_ms = float(morceaux[1])
                    mem_octets = int(float(morceaux[2]))
                    points.append((x, temps_ms, mem_octets))
                except:
                    pass

    return points

def trier_points_par_x(points):
    points = list(points)
    n = len(points)
    i = 0
    while i < n:
        j = 0
        while j < n - 1:
            if points[j][0] > points[j + 1][0]:
                tmp = points[j]
                points[j] = points[j + 1]
                points[j + 1] = tmp
            j += 1
        i += 1
    return points

def moyenner_meme_x(points, indice):
    points = trier_points_par_x(points)

    x_final = []
    y_final = []

    i = 0
    while i < len(points):
        x = points[i][0]
        somme = 0.0
        compteur = 0

        while i < len(points) and points[i][0] == x:
            somme += float(points[i][indice])
            compteur += 1
            i += 1

        x_final.append(x)
        y_final.append(somme / compteur)

    return x_final, y_final

def nom_court(chemin):
    base = os.path.basename(chemin)
    return os.path.splitext(base)[0]

def assurer_dossier_output():
    if not os.path.isdir("output"):
        os.mkdir("output")

def tracer_comparaison(series, quel, fichier_png, titre, ylabel):
    plt.figure()

    marqueurs = ["o", "s", "^", "D", "x", "v", "*", "P"]
    k = 0

    for nom, x_t, temps, x_m, mem in series:
        if quel == "temps":
            x = x_t
            y = temps
        else:
            x = x_m
            y = mem

        marqueur = marqueurs[k % len(marqueurs)]
        k += 1

        plt.plot(x, y, marker="o", label=nom, alpha=0.6)


    plt.xlabel("Nombre de mots")
    plt.ylabel(ylabel)
    plt.title(titre)
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join("output", fichier_png), dpi=140)

def main():
    chemins = sys.argv[1:]
    if len(chemins) == 0:
        print("Usage: python plot_perf.py perf_algo1.txt perf_algo2.txt")
        return

    assurer_dossier_output()

    series = []
    for chemin in chemins:
        points = lire_points(chemin)
        if len(points) == 0:
            print("Fichier ignoré: " + chemin)
            continue

        nom = nom_court(chemin)
        x_t, temps = moyenner_meme_x(points, 1)
        x_m, mem = moyenner_meme_x(points, 2)

        series.append((nom, x_t, temps, x_m, mem))

    if len(series) == 0:
        print("Aucune donnée exploitable.")
        return

    tracer_comparaison(series, "temps", "comparaison_temps.png", "Comparaison: temps", "Temps (ms)")
    tracer_comparaison(series, "memoire", "comparaison_memoire.png", "Comparaison: mémoire", "Mémoire (octets)")

    print("OK: output/comparaison_temps.png")
    print("OK: output/comparaison_memoire.png")
    plt.show()

if __name__ == "__main__":
    main()

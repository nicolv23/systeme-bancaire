# Labo 1 — Programme Hello World en C

**Auteur :** Nicolas  
**Cours :** INF3135 — Hiver 2026  

Ce projet contient un programme C simple qui affiche « Hello, world! » dans le terminal.  
Il sert d’introduction à la compilation, à Git, à GitLab et à la documentation Markdown.

---

## 📁 Fichiers du répertoire

- `hello.c` — Code source du programme en C  
- `.gitignore` — Fichiers ignorés par Git  
- `README.md` — Documentation du projet  
- `README.html` — Version HTML générée avec Pandoc  
- `README.pdf` — Version PDF générée avec Pandoc  

---

## 🧩 Exemple de code C

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Hello, world!\n");
    return 0;
}

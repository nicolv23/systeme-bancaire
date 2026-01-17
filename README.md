``   # 🏦 Système Bancaire — Projet en C  Un système bancaire minimaliste en ligne de commande, développé en C, permettant de gérer un solde chiffré, d’enregistrer un historique des opérations et d’effectuer des transactions simples.  ---  ## ✨ Fonctionnalités  - 🔐 Solde chiffré dans `solde.enc`  - 🧾 Historique des opérations dans `historique.txt`  - ➕ Dépôt d’argent  - ➖ Retrait avec validation du solde  - 📊 Consultation du solde  - 🛡 Gestion d’erreurs (fichiers manquants, entrées invalides, etc.)  ---  ## 📁 Structure du projet   ``

systeme-bancaire/│├── banque\_secure.c # Code source principal├── solde.enc # Solde chiffré├── historique.txt # Historique des opérations└── README.md # Documentation du projet

Code

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   ---  ## 🚀 Compilation  Compiler le programme avec :  ```bash  gcc banque_secure.c -o banque   `

▶️ Exécution
------------

Lancer le programme :

bash

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   ./banque   `

Le menu interactif permet :

1.  Consulter le solde
    
2.  Déposer
    
3.  Retirer
    
4.  Quitter
    

🔒 Sécurité
-----------

Le solde est stocké dans un fichier chiffré (solde.enc) via un chiffrement simple.Ce projet a un objectif pédagogique : illustrer la persistance sécurisée de données sans complexité excessive.

📝 Historique des opérations
----------------------------

Chaque transaction est ajoutée à historique.txt sous la forme :

Code

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   [2025-01-17 14:32] Dépôt : +50$  [2025-01-17 14:35] Retrait : -20$   `

🛠 Améliorations possibles
--------------------------

*   Ajouter un Makefile
    
*   Ajouter des tests unitaires
    
*   Implémenter un chiffrement plus robuste
    
*   Ajouter une interface graphique (GTK, ncurses, etc.)
    
*   Support multi‑comptes

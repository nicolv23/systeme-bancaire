# Système bancaire sécurisé en C

Application en ligne de commande permettant de gérer un compte bancaire avec authentification, transactions sécurisées et virements programmés.  
Les données sont stockées dans une base SQLite et enregistrés en temps réel.

---

## Fonctionnalités

### Authentification
- Connexion par email et mot de passe  
- Vérification par code (2FA simplifié)

### Opérations bancaires
- Consultation du solde  
- Dépôt et retrait avec limites journalières  
- Historique complet des transactions 

### Virements
- Virement immédiat  
- Virement programmé (choix oui/non)  
- Exécution automatique des virements programmés au démarrage

### Mode administrateur
- Création d’utilisateurs via le menu  
- Création via la ligne de commande :

```bash
./compte_bancaire --add-user email motdepasse
```

---

## Structure du projet

```bash
SecureBank/
├── src/        # Code source
├── include/    # Headers
├── data/       # Base SQLite
├── build/      # Objets compilés
└── bin/        # Exécutables
```

---

## Compilation et exécution

- make clean
- make
- make run

---

## Améliorations prévues

- Programmation de virements à date future  
- Annulation de virements programmés  
- Interface administrateur étendue  
- Tests unitaires  
- Interface graphique ou web dans une version ultérieure  

# Click Indicator (Geode iOS)

Indicateur visuel ON/OFF qui reagit instantanement aux clics dans Geometry Dash.

**Cible :** GD 2.2081 · Geode 5.11.2 · iOS (arm64) · JIT-less compatible

## Fonctionnalites

- Image **ON** (carre vert par defaut) quand le joueur appuie
- Image **OFF** (carre gris) quand il relache
- Images personnalisees `on.png` / `off.png`
- Deplacement libre + sauvegarde de la position
- Reglage taille / opacite / masquage (menu Geode + mode EDIT)

## Compiler depuis l'iPhone (sans ordinateur)

1. Cree un compte sur [github.com](https://github.com) puis un **nouveau depot public**.
2. Dans Safari, sur le depot : **Add file → Create new file**.
   Tape le chemin complet dans le nom (ex. `src/hooks/PlayHooks.cpp` — les `/` creent les dossiers), colle le contenu, **Commit**.
   Fais-le pour **chaque fichier** de ce projet, y compris `.github/workflows/build.yml`.
3. Onglet **Actions** → *Build iOS* → **Run workflow**.
4. Quand c'est vert, ouvre l'execution, telecharge l'artefact `click-indicator-ios` (un .zip).
5. Decompresse-le dans l'app **Fichiers** : tu obtiens un fichier `.geode`.
6. Si le build est rouge : ouvre l'etape en erreur, copie les lignes rouges et envoie-les.

## Installer le .geode sur iPhone

### Methode recommandee : serveur web du launcher Geode

1. Ouvre le **Geode iOS Launcher**.
2. Va dans **Settings**.
3. Maintiens le texte **iOS Launcher** (section About) pendant 3 secondes → active **Developer Mode**.
4. Active **Web Server** puis redémarre le launcher.
5. Sur un navigateur (même téléphone ou autre appareil sur le même Wi-Fi) va sur `http://[IP-de-ton-iPhone]:8080`.
6. Utilise l'interface pour **uploader** le fichier `.geode`.
7. Relance Geometry Dash depuis le launcher.

### Alternative

- Si tu as un ordinateur : scp / AirDrop / iTunes File Sharing vers le dossier mods du launcher.
- Le dossier exact des mods s'affiche dans le mode EDIT du mod (bouton **Indicator** en haut a gauche du menu principal).

## Images personnalisees

- Fichiers `on.png` (clic) et `off.png` (pas de clic), PNG avec transparence, tailles libres.
- Place-les dans le **dossier de donnees du mod** (chemin affiche dans le mode EDIT).
- Alternative : mets-les dans `resources/` **avant** le build GitHub (ils seront embarques).
- Sans images, le mod affiche un carre vert (ON) et un carre gris (OFF).

## Utilisation

- Menu principal → bouton **Indicator** (haut gauche) = mode EDIT :
  - Glisse l'indicateur pour le placer
  - Boutons taille / opacite / apercu ON-OFF / masquer / reset
- Reglages aussi dans Geode → Mods → Click Indicator

## Compatibilite JIT-less

Le mod utilise uniquement des `$modify` (hooks statiques).  
Il fonctionne en mode **JIT-less** (iOS 26+, SideStore, certificats, LiveContainer, etc.).

Aucun patch dynamique ni hook runtime n'est utilise.

## Limites connues

- Pas d'indicateur pendant le test en jeu depuis l'editeur (seulement dans PlayLayer).
- Le premier build GitHub est le vrai test de compilation.

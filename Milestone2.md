# Milestone 2 : Groupe 6

## Résumé Général

Une ambiance toujours agréable au sein du groupe, on s'entend bien.

Cependant par rapport à la première Milestone, on a tous eu l'impression d'être moins efficace, d'aller moins vite.

Pour le résultat final c'est moins que l'on voulait, on a pas de jeu cette Milestone ce qui est dommage, mais c'est normal avec l'état d'Anatole, et le ralentissement général, rien d'inquiétant.

## Retour Individuel

___

### Mélie

J'ai travaillé sur la serialisation. Mon plus grand soucis c'était avec json, le temps d'apprendre comment ça marche.

Pour sauvegarder et loader les scènes, ça ne m'a pas poser de grands soucis comme je devais tout sauvegarder.

C'est surtout pour les prefabs que j'ai eu des soucis. Je ne savais pas trop où aller mais j'ai pu avoir des conseils des autres personnes du groupes.

Json m'a posé plusieurs problèmes et j'ai perdu pas mal de temps sur ça.

Pour le son, je n'ai pas eu trop de soucis c'était juste pour l'implémentation de FMOD. 

___

### Hugo

Durant cette milestone, j'ai continué à travailler sur L'UI.
De nombreux widgets/features ont été rajoutés:

- Le texture viewer/creator
- La Toolbar pour selectionner le gizmo actif, mettre en jeu, faire pause ou passer une frame (l'affichage seulement à été implémenter)
- Les Gizmos, intégrer dans le viewport pour manipuler aisément les transforms
- L'inspector peut largement interagir avec le component physic, une version ameliorée du rigidbody combinee avec reactphysic 3D
- Le file explorer peut charger des scenes et les identifier
- La hierachy peut sauvegarder des prefabs

Les plus grosses difficultés viennent de la nature des travaux de groupes, à savoir, en particulier pour L'UI, qu'il faut s'adapter et interagir avec les methodes des autres et les relier, ce qui peux devenir compliqué quand la methodologie ou syntaxe diffère.

Les problèmes de communications (bien que occasionels), peuvent aussi entraver l'efficacité à laquelle je peux créer plus d'utilitaires.
___

### Anatole

___

### Quentin

Pour cette Milestone, j'ai travaillé sur la refactorisation de l'Engine et l'intégration de l'Engine physique, pour le jeu et l'éditeur.

Plus que de réussir à utiliser l'Engine Physique, l'intégrer a été beaucoup plus difficile que prévu du fait que les allocations et l'utilisation ce fait à l'intérieur de l'Engine physique, l'utilisation est donc simple mais créer les différents objects physiques pour chaque object était bien plus difficile que prévu.

Aussi, créer une AABB pour chaque mesh pour pouvoir cliquer sur l'object quqnd on est dans l'éditeur à été difficile à faire.

Ce qui a pris le plus de temps cependant a été de refactoriser le moteur.
Il a fallu changer beaucoup de chose utilisé dans plusieurs classes de l'Engine, ce qui fut très chronophage.

Finalement rien n'a été réellement bloquant, mais plutôt tout à pris beaucoup de temps, ce qui au final ne m'a pas permis de faire du rendu avancé et d'avoir un jeu, ce qui est dommage.

___

## Pour la Suite

Le plus important va avant tout d'avoir un semblant de jeu, peu importe à ce qu'il soit bien fait ou pas mais d'avoir une base qu'on pourra peaufiner.
Les taches les plus importantes seront donc:

- L'ajout d'un "Gameport" dans lequel on pourra lancer le jeu depuis l'éditeur
- Relier les options de la ToolBar pour pouvoir lancer le jeu
- la création et mis en place des classes Gameplay ... (Unit, Building, Map, etc...)
- ... et les différentes intéractions avec elles (pouvoir déplacer plusieurs unités, placer un batiment, faire spawner une unité...)
  
Alors seulement on aura une base de jeu.
En objectif de cette milestone nous avons aussi:

- Un rendu plus avancé (Light & Materials)
- Début des VFX
- Début de L'IA (BONUS)

Nous allons donc nous répartir en:

- 3 sur le gameplay
- 1 sur les VFX
- et le rendu quand on pourra au moins lancé le jeu.

l'idée est d'avoir la structure du jeu et des VFX pour que par la suite on ne fasse que rajouter des features.

Nous sommes confiant de pouvoir faire jusqu'à la mis en place des classes Gameplay, et le Début de VFX avec Rendu avancé, les différentes intéractions serait bien si on avait le temps pour avoir un vrai "jeu jouable" mais n'est pas nécessaire pour le moment, pareil pour l'IA, un pathfinding de base serait appréciable mais pas nécessaire.

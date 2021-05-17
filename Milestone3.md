# Milestone 3 : Groupe 6

## Résumé Général

On s'entend toujours très bien, et on travaille bien ensemble.

On s'est un peu pressé pour rattraper le "retard" de la milestone précédente, on a réussi à bien avancé mais il reste encore beaucoup de chose à faire pour la Milestone 4.

Le résultat de la Milestone est très encourageant car on voit le jeu apparaitre, mais pour qu'il en soit vraiment un il reste encore beaucoup à faire.
On voit cependant la différence avec la précédente Milestone et toutes les features que l'on a rajouté sont bien intégrer, ce qui fait plaisir.

## Retour Individuel

___

### Mélie

Pendant cette milestone, j'ai continué la serialization (sauvegarder la physics).
J'ai pu ensuite commencer les particles systems.
J'ai mis du temps pour comprendre le fonctionnement des shaders sous directx 11 (j'aurais du le long du projet suivre ce que Quentin faisait), mais j'ai enfin réussit à faire l'instancing grâce à Quentin.
Ce que je voulais pour les particules, c'était de pouvoir de pouvoir tout modifier à peu près comme sur unreal.
Je me suis aussi occupée de la musique : j'ai mis la possibilité d'avoir des sons en 3D ou non qui pour moi était important pour un rts.
Les particules ne sont pas encore tout à fait finis : il manque la serialisation, et des éléments pour modifier les particules.
J'ai aussi modifié les prefabs à la demande d'Anatole : on stocke maintenant les éléments et plus les noms des éléments.

Cette milestone s'est bien passer. J'aurais aimé avoir l'ui pour la musique et les particules mais Hugo était trop occupé.
J'ai passer pas mal de temps sur le shader, ce qui m'a un peu découragé. Découragé aussi au début avec le tiny wings et les maths à faire à côté de la serialization et des shaders.
J'ai l'impression que les prochaines semaines vont être compliqué comme je dois encore faire le fog of war et continuer les particules.

___

### Hugo

___

### Anatole

Pendant cette Milestone, j'ai commencé par finir les colisions du CGP_Move, je n'en suis pas encore complétement satisfait mais pour l'instant c'est suffisant, Ensuite j'ai entreprit le CGP_Attack ce qui a nécessité de splitter les entité dans des équipes différentes pour qu'il y ait affrontement. En parallèle de cela j'ai continué de clean/amélioré les différentes parties de l'ECS. Après cela, j'ai commencé les CGP_Worker et CGP_Producer afin d'avoir déjà une petite boucle de gameplay, cependant comme nous n'avions pas encore de classe prefab complète, cela a ralenti un peu le processus et comme Hugo avait déjà fort à faire, les différents test à faire étaient plus dificile à mettre en place sans helper d'ui ce qui a également ralenti ma progression. Enfin j'ai finit l'implémentation primaire de ces classes mais je n'ai pas encore eu l'occasion de la partie instantiation de prefab. Finalement je me suis penché sur deux autres classes gameplay, le ArmyHandler comportant des référence sur toutes les army pour une prise en charge plus facile et enfin le ArmyCoordinator qui correspond à la prise en charge par une AI d'une armée, leurs implémentations sont encore primaires.
Au final je n'ai pas réussi à atteindre mon objectif de boucle de gameplay (voir le schéma) mais cependant je n'en suis pas trés loin, ce qui me rassure.

Pour ce qui est de mon ressenti, j'ai l'impression d'être en retard mais surtout de ne pas avoir accompli beaucoup de choses durant cette milestone, je l'explique par le fait qu'il a fallu travailler sur différents sujets en parallèle (DS de math, TOEIC d'anglais) mais également par le fait que j'ai été pas mal occupé par des raisons personnel tout en rajoutant la baisse de motivation entrainé par la situation sanitaire actuelle. Cependant, malgré ce ressenti assez négatif j'ai espoir que maintenant que je peut complétement me focaliser sur le projet, je vais réussir à avancer au rythme que je souhaite et ainsi rattraper mon retard.

___

### Quentin

Pour cette Milestone, J'ai enfin pu travailler sur le rendu de l'engine, j'ai donc refait le renderer pour qu'il puisse permettre plus de souplesse dans son utilisation, j'ai donc décidé d'en faire un rendu differé pour permettre de nombreuse pass de rendu. cela m'a permis de rajouter des lumières (une directionnelle et des points), des ombres (une seul pour la directionnelle). J'ai aussi pu implémenter un frustrum culling et quelques autres features comme l'affichage des différents framebuffer nécessaire à ce rendu différé.

Je suis content d'avoir pu enfin faire ce que je voulais dans cette Milestone et j'ai appris beaucoup de choses ce faisant, mais concernant la productivité j'ai été assez lent ou plutôt je n'ai pas réussi à finir.
En effet, pour les lights, il me faut encore faire les spots light, la shadow à encore un grand nombre de bugs, dû principalement au fait qu'elle ne bouge pas très bien. Surtout pour le temps qui nous était alloué pour cette Milestone je trouve que j'aurai plus produire plus. Étant donné le temps qui nous reste je pense cut plusieurs features pour pouvoir faire celle dot j'ai vraiment besoin: par exemple ne pas faire les spots lights pour finir la shadow et surtout me donner le temps de faire la map (pouvoir afficher la map sous forme de Tiles, afficher une mini-map). Je ne suis pas tout à fait confiant par rapport à si je pourrai finir à temps mais dans ce cas là, il suffit de choisir le plus important donc je ne suis pas spécialment inquiet.

___

## Pour la Suite

Il est impossible de faire un jeu complet avec ce que l'on a maintenant (bien que nous n'en soyons pas loin...), donc nous ne pouvons pas arrêter le développement des features, elles sont essentiels pour pouvoir faire le jeu, cependant tous ce qui n'est pas nécessaire ne sera pas développer. Voici une liste non-exhaustive des différentes features en cours de développement qu'il nous faudra finir:

- Le GamePort (il s'affiche et la logique est là, seulement la boucle de jeu n'étant pas encore tout à fait implémenter, l'affichage ne se fait pas bien.)
- Le Particle System (plus précisément certain type de particle pourrait être nécessaire tel que le Fog of War qui donnerait immédiatement plus de juiciness)
- La ShadowPass (elle est déjà là mais ne s'affiche pas bien lorque la caméra est trop loin, l'idée serait d'implémenter une Cascaded Shadow Map pour palier à ça)
- L'UI de Jeu (les objets tels que la mini-map, un inventaire, et un "inspector" pour pouvoir voir l'unité qu'on a selectionné qui serait personnalisable dans l'éditeur)
- Serialisation (du Jeu tout ce qui est UI de Jeu, particles, etc...)
- la boucle de jeu (l'IA, et la logique de jeu)

Toutes les taches à faire ne se trouve pas dans la liste ci-dessus mais les principales sont à l'intérieur, ce qui en fait un nombre conséquent.
Nous ne savons pas si nous arriverons à faire dans l'intégralité mais nous essaierons tout du moins de faire au mieux.

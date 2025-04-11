# KNN_algo

🧠 Objectif du projet

Remplacer ou compléter un ordonnanceur classique (comme FIFO ou Round-Robin) par un algorithme KNN qui sélectionne le meilleur processus à exécuter à chaque instant, en se basant sur son comportement historique et des caractéristiques dynamiques.

⚙️ Architecture Générale 

+----------------------+
|   Liste des process  |
| (état, priorité, etc)|
+----------+-----------+
           |
           v
+----------------------+
|   Extraction des     |
|    caractéristiques  |
+----------+-----------+
           |
           v
+----------------------+
|     Module KNN       |  <== pré-entraîné ou en ligne
+----------+-----------+
           |
           v
+----------------------+
| Sélection du process |
|      à exécuter      |
+----------------------+


Le code de l'annalyseur est reparti en 3 partie:
  - les dictionnaires predefinies
  - les fonctions prealables
  - l'implementation des protocols
------------------------------------------------
      Les dictionnaires predefinies
------------------------------------------------
Pour mieux implementer l'annalyseur, on a cree les dictionnaire pour les options ou protocols
  - Type_Eth_data correspond au champ Type d'ETHERNET
  - Type_IP_protocol correspond au champ protocol d'IP
  - Type_TCP_option correspond a des options du protocol TCP
------------------------------------------------
         Explication des fonctions
------------------------------------------------
def lire_fichier(file: str)
Utilisation: Cette fonction consiste a lire les donnee d'un fichier trace (format texte)
             contenant les octets ‘bruts' et les sauvgarde sous forme d'une liste
             de liste (ligne et pour chaque ligne, les octets)

def hexa_en_binaire(hex:str, nb_bit=0)
Utilisation: Cette fonction prend les valeurs hexadecimals en entree et les
             transforme sous forme les suites de valeur binaire

def obtenir_octets(donnee: List[List[str]], l : int, c : int, nb : int):
Utilisation: Cette fonction consiste a lire nb octets depuis les donnee et fait
             aussi avancer le curseur(offset) en retournant les octets lus

def calcul_hex_en_valeur_decimal(hex: str):
Utilisation: Cette fonction prend un valeur hexadecimal et le converti en valeut decimal

------------------------------------------------
          Implementation
------------------------------------------------
L'implementation de l'annalyseur se fait de facons intuitive en utilisation les fonctions et dictionnaires
en 4 couche:
  - ETHERNET
    - IP
    - Sinon, arreter de lire
      - TCP
      - Sinon, arreter de lire
        - HTTP

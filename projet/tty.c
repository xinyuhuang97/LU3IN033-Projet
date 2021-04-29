lui $8, 0xD020
ori $9 $0 'x'
sb $9 0($8)


la $8 __tty_regs_maps
li $9 'x'
sb $9 0($8)

#3 ca depend, format j=>imediat 26 bits ne peut pas aller vers n'import quelle @
   jr permet d'acceder aux  @instructions


#4
#5 .gloabl label=> cette etiquette est visible a l'exterieur de ce fichier
#6
________________________
while (*s!=0){
  __tty_reg_map=*s;
  s++
}______________________
.data
  text_A_afficher: .asciiz "Hello"
.text
  la $8, text_A_afficher
  la $9, _tty_regs_maps
while:
  lb $10, 0($8)
  beq $10 $0 fin_while
  sb $10 0($9)
  addiu $8 $8 1
  j while
fin_while:

#7
0x80040000
la $29 __kdata_end
si on veut utiliser les fonctions, il faut tout d'aboard d'avoir initialise $29
#processus leger != processus threads

#A3.Programmation assembleur
1) 3 utilisation: fonction,variable global et variable local
symbole local d'un fichier (fonction=> n'est pas visible a exterieur du fichier)=>pour compilateur
  variable local static=>
  int f( int a, int b){
    static int x=0;
    Dfqsdf
    return 0;
  }=> variable local est persistant, elle ne peut etre alloue a la pile, en fait elle est un variable global limite a la fonction
2) extern=>1)la fonction, variable a ete declare d'un autre fichier
           2)la fonction utilise les variables declare apres la declaration de la fonction

3)
struct test_s{
    int a;int b;
};
  struct test_s tab[2]

/usr/include/
#include <file.h>

gcc -I $HOME/my_include/ f.c
soit dans /usr/include/, soit $HOME/my_include/

5)
#Ifndef TOTO
#define TOTO 4
#endif

#ifdef TOTO
#undef TOTO
#endif

6)
#ifndef _FICHIER_H
#define _FICHIER_H
#include "fichier.h"
#endif

7)
struct tty_s{
    int write;
    int status;
    int read;
    int unused;
};
extern volatile struct tty_s __ttys_regs_map[NTTYS]
int getchar(void){
  while(__tty_regs_map[0].status==0);
  return __tty_regs_map[0].read;y
}

8)

obliger l'optimisation de int x
register int x = 0;

volatile => ne veut pas il soit associe a un registre il faut le lire a partir de memoire avec sw lw

-o =>fichier assembleur en binaire


1)
__kdata_origin
=0x80000000
2)

3)
.ktext:{
  *(.text)
}>ktext_region


-D***** 123
#define


7) @ ---->commande de shell l'affichage est disparait
   - ---->de toute facons-> erreur ou pas executer la commande




TP
B1)
1)
Dans le fichier kernel.ld se trouve la description de l'espace d'addressage, on peut trouver les taille de chaque region et l'addresse pour chaque region et les informations pour les regions(MEMORY), les informations pour les sections(SECTIONS)

2)
Le code se trouve dans le fichier hcpu.s et il est renomme ainsi parce que dans ce fichier le code a pour but d'afficher HELLO WORLD

3)
Le MIPS demarre a l'addresse __boot_end=__boot_origin+__boot_region=0xbfc01000
ou peut-on le verifier
????????

4)quand on utilise l'option -c, on a fait le preprocessing et la compilation pour transformer le code source en code objet

5)
L'editeur de liens est un etape de compilation juste apres la compilation, a pour but de unifier de
tous les fichiers objets pour produire un executable.

/*est un fichier decrit comment remplir la memoire
avec les declarations des variables, la partie (MEMORY) qui est une description des regions de l'espace d'adressage
et aussi une partie(SECTIONS) qui est une description du remplissage des sections de sortie avec les section d'entree produites par le compilateur et proses dans l'ordre et placement dans les regions*/

Il est invoquer quand on veut transformer les fichiers d'objets en executable

6)
L'editeur de lien a besoins des fichier d'objet et d'un fichier .ld pour fonctionner

7)le code de boot est dans la section .text pour le compilateur

8)le message est aussi dans la section .text

9)
Le code de boot se trouve dans la section .boot

10)
le code de boot est place dans le ROM

11)
On peut trouver l'addresse du premier TTY dans le fichier .ld, il suiffit d'ajouter qulques bits pour acceder au case voulu

12)
Quand on lire un /0

13)
on fait une boucle infinie parce qu'on veut que le processus soit vivant pour faire les exec

Q:Exécutez le programme en lançant le simulateur avec make exec, qu'observez-vous ?
R:
  J'ai observe qu'un terminal a apparu avec l'affichage HelloWorld
  Aussi on a seulement initialise la section boot dans la pile

Q:Exécutez le programme en lançant le simulateur avec make debug
R:
  On obtient un fichier contenant les informations des 9999 lignes de code d'assembleur

Q:Que voyez-vous dans trace.log.s ?
R:
  Je vois l'information de boucle plus en detaille
    9996 :	<dead>: ----------------------------------------------------------------
    9996 :	0xbfc00020	0x0bf00008 	j	bfc00020 <dead>
    9997 :	0xbfc00024	0x00000000 	nop
    9998 :	<dead>: ----------------------------------------------------------------
    9998 :	0xbfc00020	0x0bf00008 	j	bfc00020 <dead>
    9999 :	0xbfc00024	0x00000000 	nop

Les chiffre avant les deux points representent lignes de code d'assembleur

le premier valeur hexadecimal correspond a l'addresse de code
le deuxieme valeur hexadecimal correspond au format I/J/R pour l'instruction

    .text                                   // section where all code are put

    boot:
        la      $4, hello                   // $4 <- address of string: "hello..."
        la      $5, __tty_regs_map          // $5 <- address of tty's registers map (cf. ldscript)
        la      $6, bye

    print:
        lb      $8, 0($4)                   // get current char
        sb      $8, 0($5)                   // send the current char to the tty
        addiu   $4, $4, 1                   // point to the next char
        bne     $8, $0, print               // check that it is not null, if yes it must be printed

    print2:
        lb      $8, 0($6)
        sb      $8, 0($5)
        addiu   $6, $6, 1
        bne     $8, $0, print2

    dead:                                   // infinite loop
        j       dead

    hello:
        .asciiz "Hello World!\n"            // string to print (put for now in .text section)

    bye:
        .asciiz "Au revoir!\n"              // string to print

B2)
1)
Le code de boot est desormais dans la section .boot avec une declaration de section: .section .boot,"ax",@progbits

2)
kinit est defini dans le fichier kinit.S
??? le code de boot connait cette adresse apres l'etape "l'editeur de lien" car les codes seront regoupes dans un meme fichier executable donc accessible par l'addresse de l'instruction
on ne peut pas utilise j init cat j est sous le format I avec lequel on n'a que 26 bits pour reprensenter l'adresse
jr permet d'acceder au addresse des instructions

3)
*(.*data*) signfie la concatenation de tous les sections contenant "."et "data" trouve dans tous les fichiers objets

4)
__kdata_end=__kdata_origin+__kdata_length=0x80020000+0x003E0000
                                         =0x80400000
Mettre 2<_> au debut des variables est une convention donc il est interdit aux variables ou fonctions en C de prefixer par <__>

Q:Exécutez le programme sur le simulateur. Est-ce différent de l'étape 1 ?
R:
  Le meme affichage "Hello World!"
  Et cette fois-ci, on a initialise le boot et la partie kernel "kdata ktext"

B3)
1)
Il faut initialiser la pile avant l'intialisation de kernel car l'execution de fonction a besoin de pile
Il est dans le fichier hcpu.S, la valeur du pointeur initial est
__kdata_end=0x80400000
2)
Le mot cle volatile est utilise dans le fichier kinit.c, voltile indique qu'on veut pas suvgarder le variable dans les registre mais les sauvgarder dans la memoire

Q:Exécutez le programme sur le simulateur. Est-ce différent de l'étape 1 ?
R:
  Le meme affichage "Hello World!"
  Et cette fois-ci, on a initialise le boot et la partie kernel "kdata ktext"

Q:Notez l'adresse de kinit dans les deux fichiers, sont-ce les mêmes ? Sont-elles dans les mêmes sections ? Expliquez pourquoi.
R:
  L'adresse de kinit dans le fichier kernel.x.s est 80000000 et dans le fichier kinit.o.s est 00000000
  ???Cela vient du fait que pour faire appeler a la fonction kinit, le kernel doit le sauvgarder dans la section ktext
  ???et pour le deuxieme cas, pour la faire appeler, il suffit de le sauvgarder dans la section text
  Ils ne sont pas dans la meme section, celle de kernel.x.s dans la section ktext et l'autre dans la section text

Q:Modifiez le code de kinit.c, et comme pour l'étape 1, afficher un second message ?
R:
    extern volatile int __tty_regs_map;     // declare tty's registers map defined in ldscript
                                            // volatile means that the compiler must write in memory
                                            // each time and cannot use a register to optimize

    char hello[] = "Hello World!\n";        // define a global string
    char bye[] = "Au revoir\n";
    void kinit (void)
    {
        char *s;

        for (s = hello; *s; s++) {          // for all char in hello string
            __tty_regs_map = *s;            // write it to the tty output register
        }
    	for (s = bye; *s; s++){
    		__tty_regs_map = *s;
    	}
        while (1);                          // infinite loop at the end
    }

B4)
1）
?? Ca sert a detecter si il y a un changement de valeur pour le valeur "status" et on attend imediatement a lire depuis le clavier
2)
Vu que tty est intialise a 0, donc le modulo ne sert a grande chose, tty%NTTY donne toujours 0
3)
Deux fenetres ont apparu, une fenetre avec l'affichage "Hello World" et l'autre vide
La fenetre vide nous interdit de saisir par clavier
Pourtant l'autre fenetre nous permet de saisir par clavier et afficher les caracteres saisies en ecran

Q:Modifiez le code pour afficher un message sur le second terminal, il y a toujours une attente sur le premier terminal.
R:
    char * hello_0 = "Hello World!\n";
    char * bye_1 = "Au revoir\n";

    void kinit (void)
    {
        char *s;
        char c;
        int tty = 0;

        for (s = hello_0; *s; s++) {                  // for all char in hello string
            __tty_regs_map[ tty%NTTYS ].write = *s;   // write it to the tty output register
        }

    	tty=1;
    	for (s=bye_1; *s; s++) {
    		__tty_regs_map[tty%NTTYS].write = *s;
    	}
    	tty=0;
        while (1) {
            while (__tty_regs_map[ tty%NTTYS ].status == 0);  // wait for a char typed on keyboard
            c = __tty_regs_map[ tty%NTTYS ].read;             // read the char
            __tty_regs_map[ tty%NTTYS ].write = c;            // print it (that is a software loopback)
        }

        while (1);                                  // infinite loop at the end
    }

Q:Modifiez le code pour que le programme affiche les touches tapées au clavier sur les deux terminaux.
R:

    char * hello_0 = "Hello World!\n";
    char * bye_1 = "Au revoir\n";

    void kinit (void)
    {
        char *s;
        char c;
        int tty = 0;
    	int i=0;
    	int j=0;
        for (s = hello_0; *s; s++) {                  // for all char in hello string
            __tty_regs_map[ tty%NTTYS ].write = *s;   // write it to the tty output register
        }

    	tty=1;
    	for (s = bye_1; *s; s++) {
    		__tty_regs_map[ tty%NTTYS ].write = *s;
    	}
    	tty=0;
        while (1) {
    		i=1;
    		j=1;
    		//printf("hi");
            while ( (i=(__tty_regs_map[0].status == 0)) && (j=(__tty_regs_map[1].status == 0)));  // wait for a char typed on keyboard
            if (i==0/*__tty_regs_map[0].status==0*/){
    			c = __tty_regs_map[ 0 ].read;             // read the char
            	__tty_regs_map[ 0 ].write = c;            // print it (that is a software loopback)
    		}
    		else if(j==0){
    			c = __tty_regs_map[ 1 ].read;             // read the char
            	__tty_regs_map[ 1 ].write = c;            // print it (that is a software loopback)
    		}
    		else;
        }
        while (1);                                  // infinite loop at the end
    }

B5)
1)
Parce qu'on veut le prototype des registres soit inacessible pour les autre fichiers
2)
??
Le fait d'avoir mis .gloabl est pour que cette etiquette soit visible a l'exterieur de ce fichier
3)
80000000 <clock>:
clck() est place a 0x80000000
???  Non , parce qu'on peut lire directement @ de kinit
boot a besoin de connaitre l'adresse de kinit()


Q:
R:Lire les fichiers en piece-joint pour plus de detailles.
  #include <harch.h>
  #include <hcpu.h>
  //extern volatile struct tty_s __tty_regs_map[NTTYS];
  char ask2[]="Deviner un chiffre!\n";
  char petit[]="Trop petit!\n";
  char grand[]="Trop grand!\n";
  char win[]="Gagne!\n";

  void guess(int tty){
    char c;
    while (tty_read(0,&c, 1) == 0);
    int t=clock();
    while(t>10){
      t=t%10;
    }
    int bool=0;
    do{
      tty_write(0, ask2 ,sizeof(ask2));
      tty_read(0, &c, 1);
      if ((int)(c)-48==t){
        tty_write(0, win ,sizeof(win));
        bool=1;
      }
      else if ((int)(c)-48>t){
        tty_write(0, grand ,sizeof(grand));
      }
      else{
        tty_write(0, petit ,sizeof(petit));
      }
    }while(bool==0);
  }

# Arduino et timers


L'initialisation du timer est fournie.

À chaque cycle de l'horloge, un compteur est incrémenté et il est
comparé à la valeur se trouvant dans un registre. Si ils sont égaux,
une interruption est générée. Le programme est alors dérouté pour
exécuter la fonction correspondante et le compteur est réinitialisé.

Un exemple http://www.avrbeginners.net/architecture/timers/timers.html


# Interruptions en arduino?

Les fonctions d'interruptions, dans le cas d'un micro-contrôleur AVR
et de l'utilisation de avr-gcc sont nommées ISR et prennent en
paramètre le vecteur d'interruption correspondant (dans notre cas, il
s'agit de TIMER2_OVF_vect). 

Pour activer les interruptions
vous devez appeler la fonction \verb!sei()!. Pour désactiver les
interruptions, la fonction est \verb!cli()!. 

Doc : http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html


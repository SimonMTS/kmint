# kmint

Problemen:
Andre kan nooit worden behaald door de tanks
Mensen worden niet verwijderd als de ufo op een tank jaagt
Force driven entities kunnen buiten kaart
Kansen zijn per tank
Tank kiest actie en wordt aangevallen voor het beindigen van zijn actie, dan krijgt de actie al + of -

Done:
 * Andre begint op een willekeurige plek in de stad
 * Andre rijdt langs alle knoop-punten
 * Ufo's "wanderen"
 * Als een ufo een mens ziet wordt dat zijn doelwit, tot dat het mens is gevangen, of tot dat de ufo een tank ziet

Seems done?:
 * Ufo's bewegen niet over de randen van het speelveld

Todo:
 DONE* Ufo's beginnen op een willekeurige plek in de stad

Not tested:
 DONE * De ufo houdt rekening met de snelheid van het mens dat hij achtervolgt 
 DONE * Als een ufo een tank ziet stopt hij met de huidige actie en gaat hij de tank achtervolgen  

 DONE * Tanks beginnen op een willekeurige "T" knoop met 0 schade     
 DONE * Tanks "wanderen" 
 DONE * Als een tank een ufo ziet dan doet hij een van de 3 acties; naar emp gaan, vluchten, naar schild gaan
 DONE * Een tank kiest een actie dmv een kans berekening
 DONE * Een tank's kansen beginnen op 33%
 DONE * Een tank gebruikt de uitkomst van zijn keuze om de kansen aan te passen
 DONE * Als een tank 100 schade heeft dan gaat hij naar andre

 DONE * Als een tank en een ufo botsen, en de tank heeft niks bij: dan krijgt de tank 50 schade
 FIXED * Als een tank en een ufo botsen, en de tank heeft een emp bij: dan kan de ufo 20 beurten niet meer bewegen  
 DONE * Als een tank en een ufo botsen, en de tank heeft een schild bij: dan krijgt de tank 20 schade

 DONE * Er beginnen 100 mensen op willekeurige plaatsen in de stad, niet op gebouwen
 FAIL * Mensen kunnen niet over gebouwen lopen en niet van de rand van het scherm af
 FAIL * Als een mens een ufo aanraakt dan gaat hij dood
 DONE * Als een mens de groene tank aanraakt dan is hij veilig
 DONE * Als een mens de rode tank aanraakt dan gaat hij dood
 DONE * Als een mens een deur aanraakt dan is hij veilig
 DONE * Mensen hebben aantrekking/afstooting tot de rode tank
 DONE * Mensen hebben aantrekking/afstooting tot de groene tank
 DONE * Mensen hebben aantrekking/afstooting tot deuren
 DONE * Mensen hebben cohesion
 DONE * Mensen hebben sepreration
 DONE * Mensen hebben alignment

 DONE * Een ronde duurt 200 beurten
 DONE * Er worden elke ronde 3 emps en 3 schilden op de graaf geplaatst
 DONE * Een tank pakt een emp/schild alleen op als hij er naar opzoek is
 DONE * Een tank mag op een modderweg maar 1x per 4 beurten bewegen
 DONE * Ufo's vertonen geen groeps gedrag

 TODO  * Document met verantwoording

 ALMOST DONE * Er kunnen met a* bewegende doelen worden bereikt    (A* zorgt er voor dat er naar het doel wordt gelopen, maar het doel blijft altijd weglopen)
 DONE  * Maak zichtbaar in de simulatie wat de visited nodes zijn en het kortste pad zelf  (Visited nodes alleen bij Andre)

 DONE * Ufo's zijn fst's
 DONE * Tank's zijn pfst's
 DONE * Er is een "global state"
 * Maak zichtbaar door middel van een overlay kleur welke staat een entiteit heeft

 DONE * Mensen en Ufo's worden alleen door krachten bewogen
 DONE * krachten worden gecombineerd op een manier die recht doet aan de waarde van hun eigenschappen

 ALMOST DONE * Extra: De mensenpopulatie wordt elke ronde samengesteld door een genetisch algoritme met alle stappen uit de evolutiecycle  (Uitdunning welke variant?)
 * Extra: Q-learning
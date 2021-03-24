# kmint

Todo:
 * Andre kan nooit worden behaald door de tanks
 * Force driven entities kunnen buiten kaart
 * Kansen zijn per tank
 * Tank kiest actie en wordt aangevallen voor het beindigen van zijn actie, dan krijgt de actie al + of -
 * Mensen kunnen niet over gebouwen lopen en niet van de rand van het scherm af
 * Document met verantwoording
 * Er kunnen met a* bewegende doelen worden bereikt    (A* zorgt er voor dat er naar het doel wordt gelopen, maar het doel blijft altijd weglopen)
 * Extra: De mensenpopulatie wordt elke ronde samengesteld door een genetisch algoritme met alle stappen uit de evolutiecycle  (Uitdunning welke variant?)


Done:
 * Andre begint op een willekeurige plek in de stad
 * Andre rijdt langs alle knoop-punten
 * Ufo's "wanderen"
 * Als een ufo een mens ziet wordt dat zijn doelwit, tot dat het mens is gevangen, of tot dat de ufo een tank ziet
 * Ufo's beginnen op een willekeurige plek in de stad
 * De ufo houdt rekening met de snelheid van het mens dat hij achtervolgt 
 * Als een ufo een tank ziet stopt hij met de huidige actie en gaat hij de tank achtervolgen  
 * Tanks beginnen op een willekeurige "T" knoop met 0 schade     
 * Tanks "wanderen" 
 * Als een tank een ufo ziet dan doet hij een van de 3 acties; naar emp gaan, vluchten, naar schild gaan
 * Een tank kiest een actie dmv een kans berekening
 * Een tank's kansen beginnen op 33%
 * Een tank gebruikt de uitkomst van zijn keuze om de kansen aan te passen
 * Als een tank 100 schade heeft dan gaat hij naar andre
 * Als een tank en een ufo botsen, en de tank heeft niks bij: dan krijgt de tank 50 schade
 * Als een tank en een ufo botsen, en de tank heeft een emp bij: dan kan de ufo 20 beurten niet meer bewegen  
 * Als een tank en een ufo botsen, en de tank heeft een schild bij: dan krijgt de tank 20 schade
 * Er beginnen 100 mensen op willekeurige plaatsen in de stad, niet op gebouwen
 * Als een mens een ufo aanraakt dan gaat hij dood
 * Als een mens de groene tank aanraakt dan is hij veilig
 * Als een mens de rode tank aanraakt dan gaat hij dood
 * Als een mens een deur aanraakt dan is hij veilig
 * Mensen hebben aantrekking/afstooting tot de rode tank
 * Mensen hebben aantrekking/afstooting tot de groene tank
 * Mensen hebben aantrekking/afstooting tot deuren
 * Mensen hebben cohesion
 * Mensen hebben sepreration
 * Mensen hebben alignment
 * Een ronde duurt 200 beurten
 * Er worden elke ronde 3 emps en 3 schilden op de graaf geplaatst
 * Een tank pakt een emp/schild alleen op als hij er naar opzoek is
 * Een tank mag op een modderweg maar 1x per 4 beurten bewegen
 * Ufo's vertonen geen groeps gedrag
 * Maak zichtbaar in de simulatie wat de visited nodes zijn en het kortste pad zelf  (Visited nodes alleen bij Andre)
 * Ufo's zijn fst's
 * Tank's zijn pfst's
 * Er is een "global state"
 * Mensen en Ufo's worden alleen door krachten bewogen
 * krachten worden gecombineerd op een manier die recht doet aan de waarde van hun eigenschappen
 * Ufo's bewegen niet over de randen van het speelveld

Not tested:
 * Maak zichtbaar door middel van een overlay kleur welke staat een entiteit heeft
 * Extra: Q-learning
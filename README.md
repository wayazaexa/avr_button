## Uppgift

Koppla in en LED till en arduino pin och en knapp till en annan. Konfigurera knappen att vara hög som standard och gå låg när den trycks in.

LEDn ska vara släckt initialt, och när knappen trycks ner ska den börja blinka varje 300 ms.

## Wokwi

Väldigt enkel koppling, men bara för sakens skull

https://wokwi.com/projects/456746316584114177


## Branches

Detta repo innehåller flera grenar för de olika situationer vi kommer titta på


## make kommandon

* ```make``` - kompilerar programmet till bin-mappen, användbart om man vill ladda upp hex-filen till wokwi
* ```make isp``` - kompilerar programmet och flashar det till en inkopplad Arduino, förutsatt att man har kopplat in en Arduino till sin dator och satt rätt port (se kommentarer i Makefile)
* ```make clean``` - rensar genom att ta bort obj-mappen
* ```make fresh``` - rensar genom att ta bort alla obj-mappen samt bin-mappen

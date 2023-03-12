# Slimme Baken voor PoAB

## LoRaWan communicatie

### Wat is LoRaWan
LoRaWan is een protocol dat is ontworpen voor weinig verbuik en een groot bereik. LoRa staat voor "Long Range Radio" en is bedoeld voor IoT devices. Het kan draadloos communiceren met batterij gestuurde "things". Het wordt meestal gebruikt in toepassingen van IoT, het kan in beide richtingen communiceren.

![LoRaWan example digikey](https://www.digikey.be/-/media/MakerIO/Images/blogs/introduction-to-lora-technology/introduction-to-lora-technology-figure-1.jpg?ts=21c2c3bb-0661-415b-b58e-c4f12837e104)

### The things network
Voor ons project op te bouwen gaan we gebruik maken van "the things network". Deze organisatie heeft een aantal gateways geplaats die openbaar gebruikt kunnen worden na registratie. Ze hebben ook een "heatmap" waarop er gezien kan worden waar de gateways zijn en/of er verbinding is op bepaalde plaatsen.

1. Aanmaken van een application. In één application kunnen er verschillende devices worden toegevoegd en beheerd worden.
2. Aanmaken van een device. Dit is nodig om onze microcontroller toegang te geven tot de gateways van "the things network".

[The things network](https://www.thethingsnetwork.org/)

[HeatMap ttn](https://ttnmapper.org/heatmap/)

### Verbinding opzetten
We gaan hier verbinding opzetten tussen de microcontroller en "the things network" gateways.

**Aanmaken device**
1. Ga naar applications op de website van ttn
2. Klik op "end devices" en/of "register end device"
3. Vul de volgende instelling in:
![TheThingsNetwork device setup](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/TheThingsNetwork_device_setup.JPG)
4. Bij "JoinEUI" kan je de waarden invullen die de seriële monitor print bij het uploaden van de voorbeeld code
5. Hierna kan je alle andere keys automatisch generen

**Connecteren**
1. Upload het voorbeeldprogramm in de (MKR WAN 1300)
2. Pas in het programma volgende details aan naar je eigen waarden:
    - devAddr
    - nwkSkey
    - appSkey
3. Hierna zal er op de seriële monitor verschijnen of deze verbinding kan maken
4. Als alles gelukt is kan je op de website zien dat er data wordt ontvangen

**Payload formateren**
Doordat de data die wordt verstuurd over LoRaWan zo klein mogelijk moet zijn is deze gecodeerd. We kunnen instellen hoe deze data gedecodeerd moet worden.
1. Ga naar "End Devices" en selecteer het juiste device
2. Ga naar "Payload formatters"
3. Kies bij "Formatter type": Custom javascript formatter
4. Vul de volgende code in en druk op "save changes"
![Formatter code](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/FormatterCode.JPG)
5. Bij live data zie je het volgende:
![Data voorbeeld](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/DataExample.JPG)

### Node-red applicatie
Om onze data te visualiseren/zenden gaan we werken met node-red over een mqtt server. Het installeren van dit programma kan via onderstaande link gebeuren. Je kan het programma nadien starten met "node-red" in je cmd, de applicatie vindt je in je browser "http://127.0.0.1:1880".

[Node-RED docs](https://nodered.org/docs/getting-started/local)

### Data µC -> Node-red
Hiervoor zal de data die wordt ontvangen op ttn moeten worden doorgestuurd naar de node-red applicatie. Dit gebeurd via een mqqt server, om deze in te stellen moet je de juiste server configureren in het mqqt blok. Als dit correct gebeurd is zal er connected staan onderaan de blok.

![Node-RED applicatie](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/Node_red_app.JPG)

Schema verduidelijking:
- De paarse blok is de ontvanger "mqqt in"
- Alle groene blokken laten data zien in het "debug" venster
- De gele blok zet data om naar een JavaScript Object
- De filter data block zal deze data filteren en duidelijk weergeven

![Node-RED debug vb](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/Node_red_debug.JPG)

### Data Node-red -> µC
Hiervoor zal de data vanaf node-red naar de microcontroller worden gestuurd. Dit zal gebeuren via een mqtt blok in node-red applicatie. je moet de juiste properties instellen die overeen komen met de ttn pagina.

![Node-RED applicatie](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/Node_red_app_send.JPG)

Schema verduidelijking:
- De paarse blok is de ontvanger "mqqt out"
- De groene blok laat data zien in het "debug" venster
- De gele blok zet data om naar een downlink Object
- De blauwe blokken zijn knoppen die data doorgeven

![Node-RED applicatie](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/Node_red_send_debug.JPG)

### LoRaWan data analyze
Voor het verzenden van data over het LoRaWan netwerk is het de bedoeling dat deze berichten zo klein mogelijk zijn. In de code van het project stuur je zogezegd een string door, maar deze wordt geconverteerd naar bytes. Hierdoor moeten we aan de kant van ttn een payload formatter toevoegen die dit terug omzet. Ook hoe groter de string die we sturen hoe langer de "uplink" tijd zal zijn. Hierdoor hebben wij volgende keuzes gemaakt omtrent onze data.

- Berichten opdelen in 3 delen vanaf de microcontroller
    - Device id
        - Bevat het id van de device. We sturen dit mee wanneer de microcontroller opstart zodat deze kan worden toegevoegd in een databank aan de ontvanger.
    - Lamp data
        - Bevat of de lampen aan of uit zijn.
        - Bevat foutmeldingen vanuit de baken.
    - GPS locatie
        - De locatie waar de baken zich bevindt moet maar 1 keer worden doorgestuurd zodat er geen overload is.

Alle berichten die worden verzonden vanaf de microcontroller worden vooraf gegaan met een identiefier. Hierdoor kan er een onderscheid worden gemaakt tussen de 3 soorten berichten.

### LoRaWan klassen analyze
Binnen LoRaWan bestaan er verschillende klassen. Elke klassen heeft verschillende voordelen en nadelen, we overlopen de 3 klassen hieronder.

1. Klasse A
    - Enkel de end devices kunnen berichten initiëren over het netwerk. Nadat een end device een uplink bericht heeft verzonden worden er 2 recieve windows geopend waarop het end device kan ontvangen.
    
    ![Klasse A](https://www.thethingsnetwork.org/docs/lorawan/classes/class-a.png)
2. Klasse B
    - Klasse B heeft de zelfde functionaliteiten als klasse A. Ook is het mogelijk om bij klasse B beacons in te stellen via de gateway. Deze gateway zal dan om de zoveel tijd een signaal sturen naar het end device wat hierop een recieve window opent.
    
    ![Klasse B](https://www.thethingsnetwork.org/docs/lorawan/classes/class-b.png)
3. Klasse C
    - Klasse C is een verdere versie van klasse A doordat de recieve windows open blijven. Hierdoor is het mogelijk om naar het end device te sturen zonder dat deze eerst een uplink moet sturen.
    
    ![Klasse C](https://www.thethingsnetwork.org/docs/lorawan/classes/class-c.png)

Vergelijking
- Klasse B
    - Deze klasse wordt niet ondersteund door de MKR WAN 1300. We gaan deze niet gebruiken
- Klasse A vs C
    - Klasse C laat ons toe om berichten te sturen vanuit node-red zonder dat de microcontroller eerst een uplink moet sturen. Het nadeel hiervan is dat klasse C een hoger verbruik heeft dan klasse a.
    - Verbruik
        - Klasse A = 0.025A
        - Klasse B = 0.036A

[TTN klassen](https://www.thethingsnetwork.org/docs/lorawan/classes/)


## Bestellijst
In onderstaande lijst zijn alle componeten opgelijst die we nodig hebben voor de uitwerking van dit project. (V1 = enkel componenten die nodig zijn om de werking verder te onderzoeken, geen mechanische comonenten. Batterij kan worden besteld vanaf dat er een stroom analyze is gemaakt.) 

[Bestellijst V1 exel](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Materiaal/BestellijstV1.xlsx)

## Referentie links
[FLWSB-SAMDaaNo21](https://github.com/DaanDekoningKrekels/FLWSB-SAMDaaNo21)

[ArduinoCore-samd](https://github.com/DaanDekoningKrekels/ArduinoCore-samd)

[The things network docs](https://www.thethingsindustries.com/docs/)

[Node-RED LoRaWan](https://www.thethingsindustries.com/docs/integrations/node-red/)

[Markdown guide](https://www.markdownguide.org/cheat-sheet/)
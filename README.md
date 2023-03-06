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
1. Upload het voorbeeldprogramm in de (ESP32/MKR WAN 1300)
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
ToDo

[Node-RED LoRaWan](https://www.thethingsindustries.com/docs/integrations/node-red/)

[Datasheet RN2483 LoRaWan chip](https://ww1.microchip.com/downloads/en/DeviceDoc/50002346C.pdf)

## Bestellijst
In onderstaande lijst zijn alle componeten opgelijst die we nodig hebben voor de uitwerking van dit project. (V1 = enkel componenten die nodig zijn om de werking verder te onderzoeken, geen mechanische comonenten. Batterij kan worden besteld vanaf dat er een stroom analyze is gemaakt.) 

[Bestellijst V1 exel](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Materiaal/BestellijstV1.xlsx)

## Referentie links
[FLWSB-SAMDaaNo21](https://github.com/DaanDekoningKrekels/FLWSB-SAMDaaNo21)

[ArduinoCore-samd](https://github.com/DaanDekoningKrekels/ArduinoCore-samd)

[The things network docs](https://www.thethingsindustries.com/docs/)

[Markdown guide](https://www.markdownguide.org/cheat-sheet/)
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

- Berichten opdelen in 3 mode's vanaf de microcontroller
    - Device id:0
        - Bevat het id van de device. We sturen dit mee wanneer de microcontroller opstart zodat deze kan worden toegevoegd in een databank aan de ontvanger.
    - Lamp data:1
        - Bevat of de lampen aan of uit zijn.
        - Bevat foutmeldingen vanuit de baken.
    - GPS locatie:2
        - De locatie waar de baken zich bevindt moet maar 1 keer worden doorgestuurd zodat er geen overload is.

Alle berichten die worden verzonden vanaf de microcontroller worden vooraf gegaan met een identiefier en hierna de mode. Hierdoor kan er een onderscheid worden gemaakt tussen de 3 soorten berichten.

Voorbeeld berichten:

0. DEVid:0
1. DEVid:1:L1:L2:L3:Status
2. DEVid:2:Lat:Lon

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

### Node-red data ophalen/invoegen vanuit een database
Om de data te kunnen opslaan gaan we dit doen met een SQL database. De database die we gebruiken vindt u hier:

[Database bakens](https://github.com/LaheyKevin/Slimme_Baken_PoAB/tree/main/LoRaWan/Node-red/DB.sql)

Doordat we verschillende data formaten doorsturen kunnen we dit opdelen in 3 gedeeltes.
- Identifier toevoegen aan database
    - Bij het opstarten van een baken zal er altijd een bericht worden gestuurd met de identiefier in. Hierop kan men ook zien als een baken niet opstart.
    - `"INSERT INTO bakens (id, last_ms) VALUES ('" + id + "','" + Date.now() + "') ON DUPLICATE KEY UPDATE id='" + id + "',last_ms='" + Date.now() + "'"`
    - In bovestaande lijn zal er in de SQL database worden geken of er al een id bestaat. Als dit bestaat wordt deze geupdate met de huidige tijd anders wordt er een nieuw statement aangemaakt.
- Data updaten in de database
    - Dit wordt om een bepaalde tijd verstuurd. Dit bevat de data over de lampen en eventuele errors
    - `"UPDATE bakens SET lamp1=" + L1 + ",lamp2=" + L2 + ",lamp3=" + L3 + ",status_code=" + Error + ",last_ms=" + Date.now() + " WHERE id='"+id+"'"`
    - In bovestaande lijn zal er in de SQL databse de lijn worden geupdate waar het id = id.
- GPS updaten in de database
    - Aangezien de bakens op een statische plaats staan moet dit niet geupdate worden. We kiezen ervoor om dit toch elke dag te updaten.
    - `"UPDATE bakens SET lat=" + Lat + ", lon=," + Lon + " last_ms=" + Date.now() + " WHERE id='" + id + "'"`
    - In bovestaande lijn zal er in de SQL database de lijn worden geupdate waar het id = id.
- ![Node-RED DB](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/DB_update.JPG)

Data ophalen uit de database.
- We halen alle data uit de tabel op
    - `"SELECT * from bakens"`
    - ![Node-RED DB](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/DB_get.JPG)

### Node-red data weergeven vanuit de DB a.d.h.v. een dropdown
Om de data die opgehaald is vanuit de database te visualiseren maken we gebruik van het node-red dashboard.

We maken gebruik van een dropdown menu. Alle data die hieraan wordt meegegeven via "msg.options" zal een item voorstellen.
- Omdat we enkel uit de id's willen kunnen kiezen van de bakens gaan we deze in een list plaatsen.
    - ![Node-RED dropdown](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/DB_dropdown_ids.JPG)
    - We maken een variable waarin we alle id's toevoegen doordat we door de data loopen en elke keer het id eruit halen. Hierna voegen we ze toe aan de options key van het msg object.
- Het dropdown menu geeft het gekozen id weer in "msg.payload". Hiermee kunnen we de aanvullende data uit de database halen.
    - `"SELECT * FROM bakens WHERE id = '" + msg.payload + "'"`
    - We selecteren de correcte rij uit de databse waarin het id overeenkomt met het gekozen id.
- Als we de data hebben uit de DB kunnen we deze in text node's plaatsen zodat deze zichtbaar worden in het dashboard.
    - `{{msg.payload[0].id}}`
    - De bovenstaande lijn laat zien hoe we in een text node data uit de payload kunnen halen.

- ![Node-RED dropdown](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/DB_dropdown.JPG)

### Node-red status codes creëren
Om aan te geven of de baken nog correct werkt zullen we gebruik maken van status codes.

Doordat we 3 leds hebben zouden we per situatie een ander getal kunnen doorsturen. Maar als we gebruik maken van een binaire code die elke led voorstel gaat dit met één byte. Hierdoor moeten we ook minder data versturen over LoRaWan.

- Een byte bestaat uit 4 bits: 0000
    - De eerste 3 bits gebruiken we om de status van de lampen door te geven. 
        - Als één van deze bits hoog is dan wilt dit zeggen dat deze baken niet brand.
    - De laatste bit gebruiken we om aan te geven of er al een controle is uitgevoerd.
        - Als dit 0 is wilt dit zeggen dat de baken nog geen controle heeft uitgevoerd.
        - Als dit 1 is wilt dit zeggen dat de controle is uitgevoerd op de baken.
    - Voorbeeld:
        - 0000 = Er is nog geen controle uitgevoerd op de baken
        - 1001 = De eerste lamp zal niet branden en er is een controle uitgevoerd

- Node-red
    - We krijgen via LoRaWan nog steeds een int doorgestuurd. Deze zetten we om naar een binair getal waaruit we dan per karakter kunnen kijken of er iets mis is.
    - ![Node-red status](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/Error_code.JPG)

### Dashboard opstellen
Met Node-red gaan we onze data visualiseren. Hieronder ziet u een voorbeeld van het huidig dashboard.

![Node-red dashboard](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/LoRaWan/Dashboard.JPG)

[Node-red flow.json](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/LoRaWan/Node-red/flows.json)

## Programma
Het programma voor de arduino MKR WAN 1300

[Arduino code](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/LoRaWan/Programma/Programma.ino)

## Lampen aansturen

### Onderzoek relais/ssr/mosfet
Hier gaan we na welke component we het beste kunnen gebruiken voor het aansturen van de lampen.

- Relais
    - Een relais is een elektromagneetische schakelaar. Deze bestaat uit een spoel, het anker en een plaat ijzer dat wordt aangetrokken door de magneet.
- SSR (solid state relais)
    - Een SSR of halfgeleiderrerlais is principeel gezien geen relais want er zijn geef bewegende delen. Deze bestaat uit een interne led, fototransisor en transistor/scr/triac.
- Mosfet
    - De mosfet is een bepaald type veldeffecttransistor (FET). Deze bestaat uit verschillende lagen (metaal, oxide, semiconductor).
    
![Relais_SSR_Mosfet](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/Ssr_relais_mosfet.JPG)

Vergelijking
- Relais vs SSR
    - Een relais maakt gebruik van mechanische contacten waardoor hier bewegende delen zijn en vonken kunnen onstaan. Bij een srr is dit een logische schakeling waardoor hier geen vonken of bewegende delen zijn. Hierdoor is de levensduur van de srr langer dan een relais.
- SSR vs Mosfet
    - Bij een mosfet moet er nog een extra weerstanden worden geplaats voor de aansturing ervan, dit is niet nodig bij een ssr.

Besluit
- We kienzen voor een SSR
    - Dit doordat deze geen bewegende delen heeft en een relais wel. Ook moet hier minder extra componenten worden toegevoegd om deze aan te sturen.

[Relais wiki](https://nl.wikipedia.org/wiki/Relais)

[SSR wiki](https://nl.wikipedia.org/wiki/Halfgeleiderrelais)

[Mosfet wiki](https://nl.wikipedia.org/wiki/MOSFET)

### Aansturen verbruiker
De module die we gaan aansturen is de [AQY211EHAZ](https://be.farnell.com/panasonic/aqy211ehaz/mosfet-relay-spst-1a-30v-dip-4/dp/3881433). Deze kan worden aangestuurd door onze µC en laat hoge stromen door. 

![Schema AQY](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Pictures/AQY.JPG)

Pinout:
1. Led input
2. Led output
3. Vermogen input
4. Vermogen output

De led kan maximaal 50mA aan en een reverse voltage van 5V. De vermogen uitgang kan 30V en 1A aan.

Berekening voor de weerstand van da ding
3.3V
Typical 1.2mA-3mA

2.21mA met 1K
3.18mA met 680

https://hydrosphere.co.uk/products/sabik-sc-110/
https://octopart.com/lxm2-pl01-0000-lumileds-11546034?utm_source=google&utm_medium=cpc&utm_campaign=g_cpc_intl_search_performancemax_english_en_usd&gclid=Cj0KCQjwk7ugBhDIARIsAGuvgPYhbZd3LKjoZlZQ9cnGBASQgdLEpTC9dYHxNIW09sQW2HKVSXqPvgMaAmKREALw_wcB
https://octopart.com/datasheet/lxm2-pl01-0000-lumileds-11546034

## Metingen die uitgevoerd worden

### Stroom meting ACS712

### Spanning/stroom meting INA219
Met deze component gaan we de spanning en stroom meten over de gehele schakeling. Het bordje beschikt over I2C communicatie waarmee de MKR WAN 1300 kan comuniceren. Om de component uit te lezen maken we gebruik van de [Adafruit_INA219](https://github.com/adafruit/Adafruit_INA219) library. Deze library laat ons toe om snel data op te halen vanuit de sensor.

Hoe library te gebruiken:
1. Library defineren bovenaan de code

    `#include <Adafruit_INA219.h>`

    `Adafruit_INA219 ina219;`
2. De sensor initialiseren in de setup

    `ina219.begin()`
3. De waardes uitlezen in de loop

    `shuntvoltage = ina219.getShuntVoltage_mV();`

    `busvoltage = ina219.getBusVoltage_V();`

    `current_mA = ina219.getCurrent_mA();`

    `power_mW = ina219.getPower_mW();`

    `loadvoltage = busvoltage + (shuntvoltage / 1000);`
    
4. Resultaat

![COM poort resultaat](https://www.electroniclinic.com/wp-content/uploads/2021/06/ina219-current-sensor-value-on-serial-monitor.jpg)

![INA219 breakout bordje](https://m.media-amazon.com/images/I/61gSXe5TcVL._SL1010_.jpg)

## Bestellijst
In onderstaande lijst zijn alle componeten opgelijst die we nodig hebben voor de uitwerking van dit project. In de bestellijst V2 zitten alle componeten dubbel omdat dit voor 2 groepen is.

[Bestellijst V1 exel](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Materiaal/BestellijstV1.xlsx)
[Bestellijst V2 exel](https://github.com/LaheyKevin/Slimme_Baken_PoAB/blob/main/Materiaal/BestellijstV2.xlsx)

### Connectoren
IP 68, connectoren om de bak te verbinden met de lampen. Ze verschillen in diameter. Connectie stukken staat in vaak samen gekocht:
tabel met specificaties moet hier komen enzo

1. https://www.conrad.be/nl/p/weipu-sp2110-p-2-i-ronde-connector-stekker-recht-serie-ronde-connectors-sp21-totaal-aantal-polen-2-1-stuk-s-736559.html
2. https://www.conrad.be/nl/p/weipu-sp2112-s-2-ronde-connector-bus-inbouw-serie-ronde-connectors-sp21-totaal-aantal-polen-2-1-stuk-s-738454.html

https://www.visser-assen.nl/blog-en-nieuws/aantal-amperes-en-draaddikte-van-krachtstroomkabels


https://datasheetspdf.com/datasheet/AQY211EHAZ.html

## PCB
1. Schema
2. PCB

## Referentie links
[FLWSB-SAMDaaNo21](https://github.com/DaanDekoningKrekels/FLWSB-SAMDaaNo21)

[ArduinoCore-samd](https://github.com/DaanDekoningKrekels/ArduinoCore-samd)

[The things network docs](https://www.thethingsindustries.com/docs/)

[Node-RED LoRaWan](https://www.thethingsindustries.com/docs/integrations/node-red/)

[Markdown guide](https://www.markdownguide.org/cheat-sheet/)

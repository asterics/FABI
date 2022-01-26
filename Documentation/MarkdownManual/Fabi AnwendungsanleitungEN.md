![FabiLogo](./Bilder/fabi1.PNG)

# FABI - User Manual

![FabiFoto](./Bilder/fabi2.PNG)

____

# Welcome to FABI

FABI - the "Flexible Assistive Button Interface" - allows you to connect up to eight buttons to a computer or tablet/smartphone to trigger mouse or keyboard actions and perform other actions. A configured FABI module can be used with any computer (Windows, Linux or Mac), without installing special software, because when connected to the computer, the FABI module behaves like an ordinary computer mouse or keyboard. People for whom conventional input devices are not suitable can thus play computer games, surf the Internet, write e-mails and much more.

The FABI system can be used with commercially available switches, inexpensive arcade buttons, or even homemade electrical contacts. FABI consists of a hardware module (an inexpensive microcontroller that acts as a computer mouse or keyboard), other optional modules and sensors (Bluetooth add-on, sip/puff pressure sensor), and a graphical configuration interface for setting desired functions ("FABI configuration manager").

FABI is available as an open source kit (including corresponding assembly instructions for the hardware) and was developed as part of the AsTeRICS Academy project at the FH Technikum Wien. In 2017, the non-profit organization AsTeRICS Foundation was founded to further develop such technologies and systems and to make them available at low cost: [www.asterics-foundation.org](https://asterics-foundation.org/).

All software modules, hardware design files, and how-to documents are available under free open source licenses and can be used and modified for free. We have made every effort to select the lowest cost components, yet not compromise on functionality and flexibility - making FABI an affordable pushbutton interface with extensive capabilities!

#### About this manual

This manual is used to explain the configuration interface and the possible settings and functions of the FABI system. The building instruction can be found in a separate document.

## Introduction

The FABI configuration manager is required to be able to define the functions of the buttons. Once a configuration has been saved in the FABI system, it remains there and the FABI system can be used to control different devices (e.g. Windows PC, Mac computer, tablet or smart phone with USB or Bluetooth connection). The configuration manager is only needed again if settings are changed.

The configuration manager has to be opened in the Chrome web browser and can be found at the following address: [fabi.asterics.eu](https://fabi.asterics.eu/index_fabi.htm). Below in figure 1 you can see the welcome page. If you click on CONNECT TO FABI CONNECTED VIA USB, no communication port (COM port) will be selectable in the selection box yet, unless you have connected the FABI module to the computer using the USB plug.

![Abbildung1](./Bilder/ENabb1.png)

*Figure 1*: Welcome page of the configuration manager

#### Connecting the USB micro cable

![Abbildung2](./Bilder/ENabb2.PNG)

*Figure 2*: Connecting the USB micro cable

#### Connecting the FABI device

To use the web configuration functions, follow the steps below. 1:

1. Make sure that the FABI device is connected to the computer.
2. After that, a communication port (COM port) with a certain number (e.g. COM4) should be selectable in a pop-up window. Click on this COM port and then click on *Connect*. (After unplugging the FABI module the COM port should disappear again).
3. **If you cannot see the user interface of the FABI configuration manager as in Figure 3, open the website again and select another COM port in the selection box that pops up. Then click again on *Connect*.**

## Using the configuration manager

After connecting to the communication port of the FABI system, you will now see the user interface of the FABI Configuration Manager (Figure 3).
![Abbildung3](./Bilder/ENabb3.PNG) 

*Figure 3*: FABI Configuration Manager User Interface

#### Port Status

The port status at the top right shows whether the FABI device is currently connected to the configuration manager. The set functions of the buttons can only be transferred to the device if "Connected" is displayed as the port status. If the device is no longer connected, you will return to the start page.

#### Save settings

The settings made are automatically saved and are immediately active in the FABI device. **The configurations are retained even if the FABI system is disconnected from the USB cable or power supply.**

### Using the memory locations (SLOTS tab)

Various function assignments of the buttons can be stored in up to 10 memory locations (***configuration slots***). These memory slots can also be changed during operation, e.g. via any button. As soon as the FABI device is supplied with power via the USB cable, the first memory location (slot) is automatically loaded and activated.

![Abbildung4](./Bilder/ENabb4.PNG) 

*Figure 4*: Creating, changing and deleting memory locations ("Slots").

A new memory slot is created in the **SLOTS** tab by clicking **CREATE SLOT**. The settings of the currently active slot are taken over (copied). With a preceding click in the text field, any name can be assigned to the new slot. In the top center there is a dropdown menu with the heading **"Select slot "**. If you click on the arrow on the right side of the text field, the dropdown menu opens showing slots that have already been saved and they can be selected here. In the **Slot configuration** list you can see all the available slots, with the active slot displayed in bold letters. Here you can also select the color, that is displayed when a slot is activated on the FABI device. The slots can be deleted individually or downloaded to the computer if required.

#### Selecting a color for the configuration

By clicking on the "Set color" field to the right of the corresponding slot in the **SLOTS** tab, a color selection dialog appears on the screen. A desired color can be selected here. The color light-emitting diode in the FABI housing assumes this color as soon as the corresponding configuration is activated. In this way, characteristic colors can be assigned to all configuration slots. **Note**: This function is only available in the new version (PCB version) of the FABI system.

#### Laden, Speichern und Übertragen von Konfigurationen

Wenn Sie im Reiter **SLOTS** weiter nach unten scrollen, finden Sie die Funktionen für das Slot-Backup. Hier Sie etwa Slots aus einer Datei hochladen: Dabei kann nach dem Anklicken von „Datei auswählen“ eine lokale Datei am Computer ausgewählt werden, die Konfigurationseinstellungen beinhaltet. Diese Einstellungen werden durch klicken von "Alle Slots hochladen und ersetzen" gesamt ins FABI-Gerät übertragen, dabei wird die aktuelle Konfiguration gelöscht. Es können jedoch auch einzelne Slots aus der Konfigurationsdatei zur aktiven FABI-Konfiguration hinzugefügt werden. Dafür werden nach dem Einblenden der erweiterten Optionen gewünschte Slots ausgewählt und durch klicken von "Gewählte(n) Slots hochladen" ins Gerät übertragen.

Das Speichern der aktuellen Konfigurationseinstellungen des FABI-Systems in eine Datei wird durch klicken von „Alle Slots herunterladen“ ermöglicht (siehe Abbildung 5). Hierbei wird die gesamte Konfiguration als Datei (.set) auf dem Computer gespeichert. Diese Einstellungsdatei kann anschließend auf dasselbe oder ein anderes FABI-Gerät übertragen werden. Dadurch können mehrere Setups (zum Beispiel für unterschiedliche AnwenderInnen oder Anwendungsfälle) auf einem Computer gespeichert werden und durch einen Klick aktiviert werden. 

**Achtung**: Beim Herunterladen einzelner Slots oder der gesamten Einstellungen werden die Dateien im Download-Verzeichnis des Web-Browsers abgelegt. Es ist sinnvoll, die Dateien anschließend in ein anderes Verzeichnis zu legen, damit sie leicht auffindbar sind und im Download-Verzeichnis nicht versehentlich gelöscht werden.

#### Demo-Voreinstellungen

Im Reiter **SLOTS** können ganz unten im Bereich "Demo-Voreinstellungen" (siehe Abbildung 5) vorbereitete Konfigurationseinstellungen ausgewählt werden. Diese bieten einen Überblick auf die Möglichkeiten des FABI-Systems und dienen als Anregung für eigene Anwendungen, etwa eine 1-Tasten-Maussteuerung, eine 2-Button-Steuerung von Computer und Smart-Phone oder eine Verwendung von Maus- und Cursortasten mit mehreren Buttons. Eine Beschreibung für die Demo-Konfigurationen kann angezeigt werden und bietet Kurzinformationen über die Funktionen bzw. Tastenbelegung.

![Abbildung5](./Bilder/ENabb5.PNG) 

*Abbildung 5*: Laden und Speichern von Konfigurationseinstellungen 

### Zuweisung der Taster-Funktionen (Reiter "AKTIONEN")

Im Reiter **AKTIONEN** können bis zu 8 Taster mit verschiedenen Funktionen belegt werden. Die Funktionen beinhalten etwa verschiedene Mausklicks, Mausbewegungen, Betätigung des Maus-Scrollrads oder von Keyboard-Tasten. Sie können die Aktion für den jeweiligen Taster ändern, indem Sie rechts vom gewünschten Button auf die unterstrichene (momentan aktive) Aktion klicken - siehe rotes Viereck in Abbildung 6. Dann erscheint ein Fenster, wo die gewünschte Aktions-Kategorie (hier: Maus) und die gewünschte Aktion ausgewählt werden kann (hier: Linke Maustaste halten). *Anmerkung:* Sollten mehr als 8 Taster benötigt werden, können mehrere FABI-Module parallel betrieben werden.

![Abbildung6.1](./Bilder/ENabb6.svg) 

*Abbildung 6.1*: Aktion für Taster auswählen/ändern

![Abbildung6.2](./Bilder/ENabb6.PNG) 

*Abbildung 6.2*: Aktion für Taster auswählen (hier: Button 1)

#### Aktionskategorie Maus

##### Linke/Rechte/Mittlere Maustaste halten (für Dauer der Eingabe-Aktion)

Durch diese Funktionen bleibt die linke, rechte oder mittlere Maustaste gedrückt, solange der Taster gehalten wird (zum Beispiel, um eine Datei zu verschieben, ist es nötig, die linke Maustaste gedrückt zu halten). 

##### Klick linke/rechte/mittlere Maustaste

Mit diesen Funktionen kann einen Klick der linken, rechten oder mittleren Maustaste durch die Betätigung des Tasters ausgeführt werden. **Anmerkung**: ein Klick besteht aus drücken & loslassen der entsprechenden Maustaste, beides passiert kurz hintereinander nach der Betätigung des Buttons!

##### Doppelklick linke Maustaste

Ein Doppelklick der linken Maustaste ist zum Beispiel zum Öffnen einer Datei notwendig. Das Ausführen von schnellen Mausklicks kann jedoch für manche NutzerInnen schwierig sein. Durch das Zuweisen dieser Funktion kann ein Doppelklick durch einen einfachen Druck auf den Taster durchgeführt werden. 

##### Drücken oder Loslassen linke/rechte/mittlere Maustaste (wechseln)

Mit diesen Funktionen wird der Zustand der linken, rechten oder mittleren Maustaste geändert, wenn der Taster gedrückt wird. Beachten Sie, dass die Maustaste solange gedrückt bleibt, bis der Taster ein weiteres Mal gedrückt wird!

##### Nach unten / nach oben scrollen

Die Funktionen „Nach oben scrollen” und „nach unten scrollen” erzeugen durch den Taster Aktivitäten mit dem ein Scrollrad der Computermaus. Das Auslösen der “Nach oben scrollen” Funktion führt dazu, dass aufwärts gescrollt wird, bei der „Nach unten scrollen“ Funktion wird abwärts gescrollt (nützlich etwa zum Lesen von Dokumenten oder Webseiten).

##### Maus horizontal / vertikal bewegen – Mausbewegung in x- oder y-Richtung

Die „Maus horizontal bewegen (x-Achse)” und „Maus vertikal bewegen (y-Achse)” Funktionen erzeugen Computermausbewegungen entlang der gewählten Achsen. Für diese Funktionen können im darunter erscheinenden Feld Geschwindigkeitsparameter festgelegt werden. Beim Drücken des Tasters wird der Mauszeiger bis zu dieser maximalen Geschwindigkeit (zb. 20 oder -20) beschleunigt.

*Ein positiver Wert für die X-Richtung bewegt den Mauszeiger nach rechts.
Ein negativer Wert für die X-Richtung bewegt den Mauszeiger nach links.
Ein positiver Wert für die Y-Richtung bewegt den Mauszeiger nach unten.
Ein negativer Wert für die Y-Richtung bewegt den Mauszeiger nach oben.*

![Abbildung7](./Bilder/ENabb7.PNG) 

*Abbildung 7*: Screenshot der "Maus horizontal bewegen (x-Achse)"-Funktion bzw. Geschwindigkeitseinstellung

#### Aktionskategorie Tastatur

##### Taste(n) halten (für Dauer der Eingabe-Aktion)

Mit der Funktion "Taste(n) halten (für Dauer der Eingabe-Aktion)" können gewünschte Tasten der Computertastatur gehalten werden, sobald der Taster gedrückt wird. Die vorher ausgewählte Tastaturtaste(n) werden so lange gedrückt, wie der Taster gehalten wird. Die Tastenauswahl funktioniert wie bei dem Befehl "Tasten drücken" beschrieben.

##### Taste(n) drücken + wieder loslassen

Die „Taste(n) drücken + wieder loslassen ”-Funktion ermöglicht es, gewünschte Tasten (auch Kombinationen) des Computerkeyboards auszulösen, sobald der Taster gedrückt wird. Die Keyboard-Tasten werden gedrückt und sofort wieder losgelassen (also nicht so lange gehalten, wie der Taster gehalten bleibt). 

##### Taste(n) drücken oder auslassen (wechseln)

Mit der Funktion "Taste(n) drücken oder auslassen (wechseln)" können gewünschte Tasten auf der Computertastatur ihren Zustand ändern (also gedrückt werden bzw. losgelassen werden), sobald die Taste gedrückt wird. Beachten Sie, dass die Tastaturtaste solange gedrückt bleibt, bis die Taste ein weiteres Mal gedrückt wird! Die Tastenauswahl funktioniert wie bei dem Befehl "Tasten drücken" beschrieben.

![Abbildung8](./Bilder/ENabb8.PNG) 

*Abbildung 8*: Screenshot der "Press Key"-Funktion

Die gewünschte Taste kann jeweils aus der Auswahlbox gewählt werden, die rechts neben „Hinzufügen“ mit dem Pfeil aufklappbar ist. Danach klicken Sie auf „Hinzufügen“ und die Eingabe erscheint auch in der nächsten Zeile („Eingabe Tasten“). Das Beispiel in Abbildung 8 zeigt die Verwendung der Keyboard-Taste “KEY_E” – es wird dadurch bei jedem Druck des Tasters ein kleines „e“ geschrieben. Es ist möglich, auch mehrere Tasten für Tastenkombinationen auszuwählen, die dann gleichzeitig gedrückt/gehalten werden. Alternativ zur einzelnen Auswahl von Tasten aus der Auswahlbox können Tasten bzw. Tastenkombinationen auch automatisch erkannt werden, wenn das Feld "Tasten mit Tastatur eingeben" aktiviert wird. 
Bereits zugewiesene Tasten können durch klicken von „Löschen“ entfernt werden.

**Anmerkung:** Mit „KEY_SHIFT“ können Großbuchstaben verwendet werden. Alle weiteren unterstützen Keyboard-Tastenkürzel finden Sie im Anhang.

##### Schreibe Wort

Die „Write Text”-Funktion ermöglicht es, jedes Mal einen bestimmten Text zu schreiben, wenn der ausgewählte Taster gedrückt wird. Die kann z.B. für die Eingabe von Passwörtern nützlich sein. Wenn Sie „Schreibe Wort“ auswählen, scheint unter dem Dropdown-Menü ein leeres Textfeld auf, klicken Sie dann auf das Textfeld und geben Sie den gewünschten Text ein:
![Abbildung9](./Bilder/ENabb9.PNG) 

*Abbildung 9*: Screenshot der "Schreibe Wort"-Funktion

#### Aktionskategorie Gerät

##### Keine Funktion

Wenn „Keine Funktion” im Funktionsmenü ausgewählt wird, dann wird der entsprechede Taster deaktiviert.

##### Nächsten Slot laden – zur nächsten Konfiguration wechseln

Sobald der Taster gedrückt wird, wird der nächste Speicherplatz (Slot) aktiviert. Dadurch können die einzelnen Slots über einen Taster nach der Reihe durchgeschalten werden. Nach der letzten Konfiguration wird automatisch die erste Konfiguration aktiviert. (Diese Aktion ist nur relevant, wenn Sie Konfigurationen in mehreren Speicherplätzen abgelegt haben.)

##### Slot per Name laden – zu Konfiguration mit bestimmtem Namen wechseln

Sobald der Taster gedrückt wird, wird die Konfiguration mit dem angegebenen Namen aktiviert. (Diese Aktion ist nur relevant, wenn Sie Konfigurationen in mehreren Speicherplätzen abgelegt haben.)

#### Aktionskategorie Makro

##### Benutzerdefiniertes Makro – Makrokommandos ausführen

Diese fortgeschrittene Funktion ermöglicht die Ausführung mehrerer Kommandos über entsprechende Kommando-Kürzel, was eine hohe Flexibilität ermöglicht. Die einzelnen Kommandos werden im Textfeld durch Strichpunkte getrennt eingetragen. Kommandokürzel können auch mittels „Befehl hinzufügen“ ausgesucht und dann mit „HINZUFÜGEN“ im Makro hinzugefügt werden. Werden mehrere Befehle hintereinander hinzugefügt, so werden die Strichpunkte automatisch eingetragen.
Beispiel: Das Makrokommando MX 10; WA 500; KP KEY_A; bewegt den Mauscursor 10 Punkte nach rechts, wartet dann 500 Millisekunden und drückt dann die Keyboardtaste „A“.
**Anmerkung**: Die möglichen Befehle, die hinzugefügt werden können, werden aufgelistet, sobald der Pfeil rechts gedrückt wird. Die möglichen Kommando- und Tastenkürzel können aber auch händisch in der Zeile „Makro“ eingegeben werden, und sind auch am Ende dieser Dokumentation zu finden.

### Verwendung eines Drucksensors (Sip-/Puff) - (Reiter "SAUG-PUSTE-STEUERUNG")

Sofern ein analoger Drucksensor wie z.B. der Sensortyp MPXV7007GP mit dem FABI-System verbunden wird, können Aktionen für Sip und Puff (Ansaugen und Pusten) definiert werden. Der analoge Spannungswert wird dabei mit dem Lötkontakt A0 am Mikrocontroller Board verbunden. Weiters muss der Sensor mit Spannung versorgt werden (5V und GND richtig verbinden).

Daraufhin können im Reiter „**SAUG-PUSTE-STEUERUNG**“ entsprechende Schwellwerte für die Stärke des Ansaugens bzw. Hineinblasens eingestellt werden. Durch diese Aktivitäten können weitere Funktionen ausgelöst werden. Der Ruhewert des Sensors (wenn weder angesaugt noch gepustet wird) liegt in der Mitte des Wertebereiches, bei ca. 512.

Im Reiter „**AKTIONEN**“ kann, wie auch bei den Buttons, eine entsprechende Aktion ausgewählt werden, die beim Ansaugen bzw. beim Pusten ausgeführt werden soll.

![Abbildung10](./Bilder/ENabb10.PNG) 

*Abbildung 10*: Screenshot der Einstellungen für Saug- und Pusteschwellenwerte

### Einstellmöglichkeiten im Reiter „TIMINGS“

Über den Reiter **TIMINGS** können weitere Parameter für den Betrieb des FABI Systems eingestellt werden. In den Antitremor-Einstellungen können Schwellenwerte für Drücken und Loslassen bestimmt werden. Diese Antitremor Einstellungen dienen dazu, um versehentliches Drücken oder Loslassen durch Zittern der Hand zu vermeiden. Weiters können noch Schwellenwerte für langes Drücken, für Slot-Weiterschalten durch doppeltes Drücken und für den automatischen Linksklick bestimmt werden. All diese Einstellungen werden untenstehend näher erklärt.
**Hinweis**: diese Einstellungen beziehen sich nur auf den aktiven Slot (Speicherplatz).

![Abbildung11](./Bilder/ENabb11.PNG) 

*Abbildung 11*: Screenshot der weiteren Einstellungen im Reiter "Timings"

#### Antitremor-Einstellungen

Durch die Anti-Tremor Parameter können unterschiedliche Zeitspannen festgelegt werden, die beim Auslösen eines Tasters vom FABI-System überprüft werden. So kann das unwillkürliche Auslösen von Tasten bei Tremor oder Problemen der Feinmotorik minimiert werden:

* „Antitremor Schwellenwert für Drücken“ legt die minimale Zeitspanne fest, die ein Taster gedrückt werden muss, damit die Aktion durchgeführt wird. 
* „Antitremor Schwellenwert für Loslassen“ legt die minimale Zeitspanne fest, die ein Taster ausgelassen werden muss, damit das Auslassen erkannt wird.
* „Antitremor Wartezeit“ legt die minimale Zeitspanne fest, die zwischen hintereinander folgende Betätigungen eines Tasters vergehen muss.

#### Schwellenwerte für Spezialfunktionen

##### Schwellenwert für langes Drücken:

Der Schwellwert „**Schwellenwert für langes Drücken**“ legt eine Zeitspanne in Millisekunden fest, ab der das Drücken eines Tasters als „Langes Drücken“ interpretiert wird und folglich eine alternative Aktion ausgeführt werden kann. Dies ist vor allem dann sinnvoll, wenn eine Person nur eine geringe Anzahl von Tastern verwenden kann. Mit einem langen Tastendruck kann dann eine alternative Funktion (z.B. das Umschalten der Konfiguration) durchgeführt werden. Die entsprechenden Funktionen können im Reiter „AKTIONEN“ unter „Button x lange drücken“ definiert werden kann.
Die Lange-Drücken-Funktion ist deaktiviert, wenn eine Zeitspanne von 0 Millisekunden eingestellt ist.
Derzeit unterstützen nur Tasten, die bei den Anschlüssen 1, 2 oder 3 angesteckt werden, die Lange-Drücken Funktion. 

##### Verwendung der "Doppeltes Drücken" Funktion

Der Wert "**Schwellenwert für Slot-Weiterschalten durch doppeltes Drücken**" definiert die maximale Dauer eines "doppelten Tastendrucks" in Millisekunden. Wird ein schnelles zweimaliges Drücken einer Taste erkannt, erfolgt ein **automatischer Slotwechsel zum nächsten Slot**. Dies ist besonders dann sinnvoll, wenn eine Person nur eine einzige Taste benutzen kann: Durch zwei schnelle Tastendrücke kann die Funktion der Taste geändert werden. So könnten zum Beispiel mehrere Tastaturtasten abwechselnd gedrückt werden (z. B. zur Spielsteuerung) oder der Mauszeiger kann mit einem einzigen Schalter in verschiedene Richtungen bewegt werden.

##### Schwellenwert für automatischen Linksklick

Diese Einstellung ermöglicht es, einen linken Mausklick nach erfolgter Mausbewegung zu erzeugen. Verstreicht die gewählte Zeitspanne (in Millisekunden) ohne weitere Mausbewegungen, dann wird der Mausklick automatisch erzeugt. Dies erlaubt die vollständige Steuerung eines Mauszeigers mit einer geringen Anzahl von Tastern oder (in Kombination mit der automatischen Slotwechsel-Funktion) mit nur einem Taster. Ein Wert von 0 Millisekunden schaltet die automatische Klick-Funktion ab.

### Einstellmöglichkeiten im Reiter „ALLGEMEIN“

Im Reiter **ALLGEMEIN** können die Bluetooth-Einstellungen angepasst werden, sowie die Firmware und die Bluetooth-Firmware überschrieben / aktualisiert werden.

![Abbildung12](./Bilder/ENabb12.PNG) 

*Abbildung 12*: Screenshot der weiteren Einstellungen im Reiter "ALLGEMEIN"

#### USB Modus / Bluetooth Modus

Das optionale Bluetooth-Modul (Add-On) ermöglicht die Aussendung von Tastatur- und Maus-Aktionen zur einem verbundenen ("gepairten") bluetoothfähigen Gerät (etwa zu einem Smartphone oder Tablet-Computer). Ist ein Bluetooth-Modul im FABI-Gehäuse installiert, so kann in der Selektionsbox pro Slot definiert werden, ob die Maus- und Tastaturaktionen über USB oder über Bluetooth (oder über beides) erzeugt werden sollen (Abbildung 13):  

![Abbildung13](./Bilder/abb13.PNG) 

*Abbildung 13*: Bluetooth-Modus bzw. USB/Bluetooth Auswahl

**Hinweis**: Die Auswahl USB/Bluetooh-Modus ist nur sinnvoll, wenn das optionale Bluetooth-AddOn-Modul am FABI-Gerät angeschlossen ist (siehe “Verwendung des Bluetooth Moduls”). 
Da diese Einstellung pro Speicherplatz (Slot) definiert wird, kann sie für verschiedene Speicherplätze unterschiedlich sein. So kann mit demselben FABI-Gerät z.B. ein Laptop über USB und ein Smartphone/Tablet über Bluetooth angesteuert werden. Das Umschalten erfolgt einfach durch den Wechsel des aktiven Slots.

#### Firmware überschreiben / aktualisieren

Durch Drücken dieses Buttons wird versucht, die aktuellste Software (Firmware) für FABI Hauptmodul (den Mikrocontroller) herunterzuladen und zu installieren. Dieser Vorgang kann einige Minuten dauern. Links neben dem Button werden die Versionsnummer der derzeit installierten Firmware und der für das Update verfügbaren Version dargestellt.  

#### Bluetooth-Firmware überschreiben / aktualisieren

Durch Drücken dieses Buttons wird versucht, die aktuellste Software (Firmware) für das Bluetooth-Modul herunterzuladen und am Modul zu installieren. Dieser Vorgang kann einige Minuten dauern und ist nur dann möglich, wenn ein Bluetooth Modul mit dem FABI-System verbunden ist. Links neben dem Button werden die Versionsnummer der derzeit installierten Firmware und der für das Update verfügbaren Version dargestellt. Hier kann auch festgestellt werden, ob ein Bluetooth-Modul installiert ist bzw. ob das Modul erkannt wird.  

#### Rücksetzen auf Defaulteinstellungen, Tastenkombinationen

Am unteren Ende des Reiters finden Sie den Button für das Zurücksetzen des FABI-Gerätes auf die Defaulteinstellungen. Dies ist sinnvoll, wenn die aktuellen Einstellungen Probleme mit der Verwendung der Gerätes verursachen. Weiters sehen Sie die Tastenkombinationen für das schnelle Wechseln zwischen den Reitern (Tabs).

![Abbildung14](./Bilder/ENabb14.PNG) 

*Abbildung 14*: Gerät zurücksetzen, Tastenkombinationen

**Viel Spaß beim Ausprobieren und beim Erstellen eigener FABI-Konfigurationen!**

### Verwendung des Bluetooth Moduls

Das FABI System erlaubt die Verwendung eines optionalen Bluetooth “AddOn”-Moduls für drahtlose Geräteverbindungen zu Mobiltelefonen, Tablets und Computern mit Bluetooth-Funktion. Das Bluetooth-Modul ist seperat über die AsTeRICS Foundation erhältlich bzw. in der entsprechenden Version des FABI-Bausatzes enthalten. 

#### Installation des Bluetooth Moduls

Das Bluetooth Modul lässt sich sehr einfach mit der Platinen-Version des FABI-Systems (Bausatz) verwenden. (Eine Verwendung mit einem einzelnen Mikrocontroller ist auch möglich und wird in den weiterführenden Informationen im [Github Repository](https://github.com/asterics/esp32_mouse_keyboard) des Bluetooth Moduls genauer beschrieben.
Das Bluetooth Modul wird in der auf der Platine eingezeichneten Orientierung auf den 10-poligen Konnektor gesteckt. Öffnen Sie dafür das FABI-Gehäuse und drücken Sie den das Modul bis zum Anschlag auf die dafür vorgesehene Stiftleiste (siehe Abbildung 15):

![Abbildung15](./Bilder/abb15.PNG) 

*Abbildung 15*: Bluetooth Modul, aufgesteckt auf die FABI-Platine

#### Verbinden mit einem Bluetooth-Gerät (pairing)

Das Bluetooth Modul zeigt durch schnelles Blinken (ca. 2 mal pro Sekunde) an, dass es bereit ist, mit einem Zielgerät eine Verbindung aufzunehmen. (Das Blinken des Moduls kann auf der Unterseite des FABI-Gehäuses wahrgenommen werden.) Das Zielgerät für die Verbindung (für das pairing) kann z.B. ein Mobiltelefon oder Tablet sein. In den Bluetooth-Einstellungen des Zielgerätes muss ein neues Gerät hinzugefügt werden. Hier sollte es möglich sein, das “FABI” Gerät auszuwählen. Bei erfolgreicher Verbindung sollte die Leuchtdiode des Bluetooth-Modules langsam blinken (ca. 1 Mal pro Sekunde). Das Zielgerät kann nun parallel zu dem über USB verbunden Gerät über das FABI System angesteuert werden. Die Entscheidung, ob eine Aktion über USB oder über  Bluetooth ausgeführt wird, erfolgt in den Einstellungen im Reiter **ALLGEMEIN** (siehe Abbildung 13).

### Liste der unterstützten Makrokommandos

| **Kürzel**  | **Funktion**                                                                                                                   | **Beispiel**                                                                                   |
| ----------- | ------------------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------- |
| CL          | Klick linke Maustaste                                                                                                          |                                                                                                |
| CR          | Klick rechte Maustaste                                                                                                         |                                                                                                |
| CM          | Klick mittlere Maustaste (Zahnrad)                                                                                             |                                                                                                |
| CD          | Doppelklick linke Maustaste                                                                                                    |                                                                                                |
| HL          | Linke Maustaste halten                                                                                                         |                                                                                                |
| HR          | Rechte Maustaste halten                                                                                                        |                                                                                                |
| HM          | Mittlere Maustaste halten                                                                                                      |                                                                                                |
| TL          | Drücken oder Loslassen linke Maustaste (wechseln)                                                                              | Ändert: gedrückt <-> nicht gedrückt                                                            |
| TM          | Drücken oder Loslassen mittlere Maustaste (wechseln)                                                                           |                                                                                                |
| TR          | Drücken oder Loslassen rechte Maustaste (wechseln)                                                                             |                                                                                                |
| RL          | Linke Maustaste loslassen                                                                                                      |                                                                                                |
| RR          | Rechte Maustaste loslassen                                                                                                     |                                                                                                |
| RM          | Mittlere Maustaste loslassen                                                                                                   |                                                                                                |
| WU          | Nach unten scrollen                                                                                                            |                                                                                                |
| WD          | Nach oben scrollen                                                                                                             |                                                                                                |
| MX <int>    | Maus horizontal bewegen (x-Achse)                                                                                              | MX 4 -> bewegt Cursor 4 Pixel nach rechts                                                      |
| MY <int>    | Maus vertikal bewegen (y-Achse)                                                                                                | MY -10 -> bewegt Cursor 10 Pixel nach oben                                                     |
| KW <string> | keyboard write string: Schreibe Wort                                                                                           | KW Hallo! -> schreibt "Hallo!“ am Keyboard                                                     |
| KP <string> | key press:  Keyboard-Tasten drücken (und wieder loslassen). Tasten werden durch Tastenkürzel identifiziert (siehe Liste unten) | KP KEY_UP -> drückt die "Cursor-Up" Taste; KP KEY_CTRL KEY_ALT KEY_DELETE drückt alle 3 Tasten |
| KH <string> | key hold: Keyboard-Tasten drücken (und halten)                                                                                 | siehe KP                                                                                       |
| KT <string> | key toggle: Zustand der Keyboard-Tasten ändern (drücken wenn nicht gedrückt, loslassen wenn gedrückt)                          | siehe KP                                                                                       |
| KR <string> | key release: Spezifische Tasten auslassen; Tasten werden durch Tastenkürzel identifiziert (siehe Liste unten)                  | KR KEY_UP -> lässt die „Cursor-Up“ Taste los                                                   |
| RA          | release all = alle Tasten und Mausbuttons loslassen                                                                            |                                                                                                |
| WA <int>    | Warten (Millisekunden), bestimmte Anzahl Millisekunden warten                                                                  | WA 100 -> wartet 100 Millisekunden                                                             |
| NE          | next slot: nächsten Slot laden                                                                                                 |                                                                                                |
| LO <string> | load slot: Slot per Name laden; wechselt zum angegebenen Slot                                                                  | LO mouse                                                                                       |
| NC          | No command: keine Funktion                                                                                                     |                                                                                                |

### Liste der Kürzel für Keybord-Tasten

| **Unterstützte Kürzel für Tastaturtasten**                                                                                                                                                                             |
| ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Buchstaben**                                                                                                                                                                                                         |
| KEY_A  KEY_B  KEY_C  KEY_D  KEY_E  KEY_F  KEY_G  KEY_H  <br/>KEY_I  KEY_J  KEY_K  KEY_L  KEY_M  KEY_N  KEY_O  KEY_P  <br/>KEY_Q  KEY_R  KEY_S  KEY_T  KEY_U  KEY_V  KEY_W  KEY_X KEY_Y KEY_Z                           |
| **Ziffern**                                                                                                                                                                                                            |
| KEY_1  KEY_2  KEY_3  KEY_4  KEY_5  KEY_6  KEY_7  KEY_8  KEY_9  KEY_0                                                                                                                                                   |
| **Funktionstasten**                                                                                                                                                                                                    |
| KEY_F1  KEY_F2  KEY_F3  KEY_F4  KEY_F5  KEY_F6  KEY_F7  KEY_F8  KEY_F9  KEY_F10  <br/>KEY_F11  KEY_F12  KEY_F13  KEY_F14  KEY_F15  KEY_F16  KEY_F17  KEY_F18  KEY_F19 <br/>KEY_F20  KEY_F21  KEY_F22  KEY_F23  KEY_F24 |
| **Navigationstasten**                                                                                                                                                                                                  |
| KEY_UP  KEY_DOWN  KEY_LEFT  KEY_RIGHT  KEY_TAB  KEY_PAGE_UP  KEY_PAGE_DOWN <br/>KEY_HOME  KEY_END                                                                                                                      |
| **Spezielle Tasten**                                                                                                                                                                                                   |
| KEY_ENTER  KEY_SPACE  KEY_BACKSPACE  KEY_DELETE  KEY_INSERT  KEY_ESC  KEY_NUM_LOCK<br/>KEY_SCROLL_LOCK  KEY_CAPS_LOCK  KEY_PAUSE<br/>                                                                                  |
| **Tasten für alternative Funktionen**                                                                                                                                                                                  |
| KEY_SHIFT  KEY_CTRL  KEY_ALT  KEY_RIGHT_ALT  KEY_GUI  KEY_RIGHT_GUI                                                                                                                                                    |

### Weiterführende Links und Software-Empfehlungen

Das FABI Button Interface eignet sich als alternatives Eingabesystem für verschiedenste Anwendungszwecke – von Computer/Smartphone-Kontrolle bis zu Verwendung von Spielen und Lernsoftware. Je nach Anzahl der verwendbaren Taster können hier auch Standard-Programme mit Maus / Keyboard ohne weitere Anpassung genutzt werden.

Falls die motorischen Fähigkeiten bzw. die Anzahl der verwendbaren Tasten-Funktionen sehr stark eingeschränkt sind, bieten speziell angepasste Applikationen auch Möglichkeiten für eine Verwendung mit nur einem Taster. Im folgenden werden einige interessante Ressourcen vorgestellt:

#### AsTeRICS und AsTeRICS Grid

Die weiteren Open Source Entwicklungen der [AsTeRICS Foundation](https://www.asterics-foundation.org) erlauben eine vielseitige Verwendung von Taster-Schnittstellen. Das [AsTeRICS](https://www.asterics.eu)-System ist ein Baukasten für Assistierende Technologien mit dem 1-Tasten Verfahren zur Computer-Kontrolle erstellt werden können. [AsTeRICS Grid](https://grid.asterics.eu) ist ein flexibles System für die Unterstütze Kommunikation (UK, AAC), das auch mit einzelnen Tastern verwendet werden kann.

#### SpecialEffect.org

Die gemeinnützige Organisation SpecialEffect ([www.specialeffect.org.uk](https://www.specialeffect.org.uk)) widmet sich der Verbreitung von barrierefreiem Spielen in Großbritannien und führt spezielle Anpassungen für Menschen mit Behinderung durch.

#### BLTT.org

Die Webseite Better Living Through Technology ([bltt.org/introduction-to-switch-access](https://bltt.org/introduction-to-switch-access)) bietet nützliche Informationen für Tasten-basierte Verwendung von Computern und Links zu vielen Software-Tools.

#### OneSwitch.org

Unter der Adresse [www.oneswitch.org.uk](https://www.oneswitch.org.uk/) hat Barrie Ellis eine Fülle von Tipps und Informationen für Single-Switch Gaming und spezielle Adaptierungen für Tasten-Steuerung von Computerspielen gesammelt. Besonders interessant sind die Spiele-Bibliothek, das One-Switch-Pulse System und die Verwendung von Spiele-Konsolen mittels Controller-Adaptern wie dem „Titan-Two“.

#### Click2Speak

[Click2Speak](https://www.click2speak.net) ist eine flexible Bildschirmtastatur (On-Screen Keyboard) für Windows, die zusätzliche nützliche Funktionen bietet- etwa eine Maus-Click-Auswahlbox.

## Kontaktinformationen

**AsTeRICS Foundation**

Webpage: <https://www.asterics-foundation.org>
Email: <office@asterics-foundation.org> 

## Haftungsausschluss

Die Fachhochschule Technikum Wien und die AsTeRICS Foundation übernehmen keinerlei Gewährleistung oder Haftung für die Funktionsfähigkeit der Hardware-/Softwaremodule oder die Richtigkeit der Dokumentation.

Weiters haften die FH Technikum Wien und die AsTeRICS Foundation nicht für etwaige Gesundheitsschäden durch eine Verwendung der bereitgestellten Hardware-/Softwaremodule. 
Die Verwendung der bereitgestellten Module und Informationen erfolgt auf eigenes Risiko!

## Danksagung

Wir danken Miriam Brenner, Fabian Schiegl und Fanny Peternell für ihre Unterstützung bei der Erstellung dieser Anleitung und Andreas Fußthaler für die Entwicklung der FABI-PCB-Version.

Dieses Projekt wurde von der Stadt Wien (Magistratsabteilung 23 für Wirtschaft, Arbeit und Statistik, MA 23) finanziell unterstützt (Projektnummer 14-02, 18-04).

![Abbildung18](./Bilder/abb18.PNG)

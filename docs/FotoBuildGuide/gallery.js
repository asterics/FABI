// Keep image order and both language versions together so the gallery is easy to maintain.
const descriptions = [
  { de: 'Bauen wir den FABI-3-Bausatz!', en: "Let's build the FABI 3 construction set!" },
  { de: 'Pack den Inhalt des Bausatzes aus.', en: 'Unpack the contents of the construction kit.' },
  { de: 'Stecke die Stiftleisten in die dafür vorgesehenen Buchsen.', en: 'Insert the pin headers into their sockets.' },
  { de: 'Setze den Raspberry Pi Pico wie abgebildet auf die Stiftleisten (auf die richtige Ausrichtung achten!). Achte vor dem Löten darauf, dass der RP Pico gerade und fest sitzt.', en: 'Place the Raspberry Pi Pico on the pin headers as shown (check the correct orientation!). Make sure the RP Pico sits straight and firmly before soldering.' },
  { de: 'Löte den Raspberry Pi Pico an die Stiftleisten.', en: 'Solder the Raspberry Pi Pico onto the pin headers.' },
  { de: 'Stecke die Klinkenbuchsen ein. Überprüfe nochmals, dass sie vollständig eingesteckt sind und gerade auf der Platine sitzen.', en: 'Insert the jack plug connectors. Double-check that they are fully inserted and sit straight on the board.' },
  { de: 'Löte die Klinkenbuchsen auf der Rückseite der Platine fest. Du kannst sie zum Löten mit Klebeband fixieren.', en: 'Solder the jack plug connectors on the backside of the board. You could use adhesive tape to hold them in place for soldering.' },
  { de: 'Stecke die RJ25-Buchse ein.', en: 'Insert the RJ25 connector.' },
  { de: 'Löte die RJ25-Buchse auf der Rückseite der Platine fest.', en: 'Solder the RJ25 connector on the backside of the board.' },
  { de: 'Setze den IR-Empfänger wie abgebildet ein (auf die richtige Ausrichtung achten!).', en: 'Insert the IR receiver as shown (check the correct orientation!).' },
  { de: 'Löte den IR-RJ25-Stecker auf der Rückseite der Platine fest. Kürze die Anschlussdrähte mit einem Seitenschneider.', en: 'Solder the IR RJ25 connector on the backside of the board. Cut the leads using side-cutting pliers.' },
  { de: 'Setze den Batterieanschluss wie abgebildet ein (auf die richtige Ausrichtung achten!).', en: 'Insert the battery connector as shown (check the correct orientation!).' },
  { de: 'Löte den Batterieanschluss auf der Rückseite der Platine fest.', en: 'Solder the battery connector on the backside of the board.' },
  { de: 'Löte die Neopixel-LED vorsichtig auf der Rückseite der Platine fest (auf die richtige Ausrichtung achten!).', en: 'Carefully solder the Neopixel LED on the backside of the board (check the correct orientation!).' },
  { de: 'Setze das LC-Display wie abgebildet auf.', en: 'Place the LC display as shown.' },
  { de: 'Verwende ein Stück Schaumstoff, Papier oder Ähnliches, um das LCD während des Lötens gerade auszurichten.', en: 'Use a piece of foam, paper, or similar material to level the LCD while soldering.' },
  { de: 'Löte das LC-Display auf der Oberseite der Platine fest.', en: 'Solder the LC display on the top side of the board.' },
  { de: 'Entferne den Schaumstoff bzw. das Papier und die Schutzfolie vom LCD.', en: 'Remove the foam or paper and the protective film from the LCD.' },
  { de: 'Klebe den Mini-Lautsprecher mit Heißkleber auf die Rückseite der Platine. Achte auf die genaue Positionierung.', en: 'Use hot glue to attach the mini loudspeaker to the backside of the board. Take care with the exact positioning.' },
  { de: 'Kürze die Lautsprecherkabel und entferne wie abgebildet die Isolierung.', en: 'Cut the loudspeaker wires and remove the insulation as shown.' },
  { de: 'Löte das schwarze und das rote Lautsprecherkabel an die richtigen Lötpads auf der Rückseite der Platine.', en: 'Solder the black and red loudspeaker leads to the correct pads on the backside of the board.' },
  { de: 'Verbinde die Batterie mit dem Anschluss.', en: 'Attach the battery to the connector.' },
  { de: 'Platziere die Batterie wie abgebildet. Befestige sie mit doppelseitigem Klebeband oben auf den Klinkenbuchsen.', en: 'Place the battery as shown. Use double-sided adhesive tape to fix it on top of the jack plug connectors.' },
  { de: 'Jetzt wird geprüft, ob alles funktioniert! Verbinde deinen FABI3 über ein USB-Kabel mit einem Computer. Verwende ein Micro-USB-Kabel mit Datenleitungen, kein reines Ladekabel. Der Computer sollte einen Ordner für den Raspberry Pi Pico als USB-Laufwerk öffnen. Öffne die angezeigte GitHub-Seite mit den veröffentlichten FABI3-Firmwareversionen (https://github.com/asterics/FabiWare/releases). Lade die neueste Version der Datei FABI_RP2350.uf2 herunter und ziehe sie auf das USB-Laufwerk.', en: 'Now it is time to check if everything works! Plug your FABI3 into a USB port of a computer. Use a USB Micro cable with data pins, not a charging-only cable. The computer should open a folder for the Raspberry Pi Pico as a USB drive. Visit the GitHub page with the released FABI3 firmware versions (https://github.com/asterics/FabiWare/releases). Download the latest version of FABI_RP2350.uf2 and drag it to the USB drive.' },
  { de: 'Nach der Installation der Firmware auf dem Raspberry Pi Pico sollte das FABI3-Gerät eine Tonfolge abspielen und den Standardnamen des Slots auf dem LCD anzeigen.', en: 'After the firmware has been installed on the Raspberry Pi Pico, the FABI3 device should start with a tone sequence and show the default slot name on the LCD screen.' },
  { de: 'Wenn alles funktioniert, setze die FABI3-Platine in das 3D-gedruckte Gehäuse ein. Achte darauf, dass der kleine Ein-/Ausschalter auf der rechten Seite in die vorgesehene Öffnung der oberen Gehäusehälfte passt, bevor du alles festziehst.', en: 'If everything works, put the FABI3 board into the 3D-printed enclosure. Make sure that the small on/off switch on the right side fits into its slot on the top half of the enclosure before tightening everything.' },
  { de: 'Herzlichen Glückwunsch! Du hast deinen FABI3-Bausatz erfolgreich zusammengebaut! Sieh dir nun das Benutzerhandbuch an (https://github.com/asterics/FABI/raw/master/Documentation/UserManual/Fabi%20User%20Manual.pdf), in dem erklärt wird, wie du die Gerätefunktionen konfigurieren kannst. Bei Problemen mit dem Aufbau kannst du uns per E-Mail unter office@asterics-foundation.org kontaktieren.', en: 'Congratulations! You have successfully built your FABI3 construction kit. Now have a look at the user manual (https://github.com/asterics/FABI/raw/master/Documentation/UserManual/Fabi%20User%20Manual.pdf), which explains how to configure the device functions. If you have problems with the build, please contact us by email at office@asterics-foundation.org.' }
];

const photos = descriptions.map((description, index) => {
  const number = String(index + 1).padStart(2, '0');
  const extension = number === '24' ? 'png' : 'jpg';

  return {
    file: `img/fabi3Build_${number}.${extension}`,
    description
  };
});

const gallery = document.querySelector('#gallery');
const photoCount = document.querySelector('#photo-count');
const lightbox = document.querySelector('#lightbox');
const lightboxImage = document.querySelector('#lightbox-image');
const lightboxStep = document.querySelector('#lightbox-step');
const lightboxTitle = document.querySelector('#lightbox-title');
const lightboxDescription = document.querySelector('#lightbox-description');
const languageToggle = document.querySelector('#language-toggle');
const languageMeta = document.querySelector('meta[name="description"]');
let activePhoto = 0;
let language = 'de';

const interfaceText = {
  de: {
    pageTitle: 'FABI-Bauanleitung',
    pageDescription: 'Anleitung für den Bau des FABI3 Construction Sets!',
    eyebrow: 'AsTeRICS Foundation / Assistronik / Bauanleitung FABI3',
    intro: 'Anleitung für den Zusammenbau des FABI3-Bausatzes.',
    galleryDetails: 'Galerie-Details',
    photoCountLabel: 'einfache Schritte!',
    sequenceNote: ' ',
    processLabel: 'und nun ...',
    galleryTitle: 'Los gehts!',
    sectionNote: 'Für detailierte Bilder und Instruktionen, clicke die Übersichtsfotos an!',
    open: 'Öffne',
    step: 'Schritt',
    stageCount: 'Schritt',
    close: 'Bildansicht schließen',
    previous: 'Vorheriges Bild',
    next: 'Nächstes Bild',
    switchLanguage: 'Switch language to English',
    languageButton: 'English'
  },
  en: {
    pageTitle: 'FABI build journal',
    pageDescription: 'A step-by-step photo record of building the Flexible Assistive Button Interface.',
    eyebrow: 'AsTeRICS Foundation / Assistronik / FABI3 build journal',
    intro: 'A visual walkthrough of the FABI construction kit, from the first components to the finished interface.',
    galleryDetails: 'Gallery details',
    photoCountLabel: 'easy build stages',
    sequenceNote: ' ',
    processLabel: 'and now ...',
    galleryTitle: 'Lets start!',
    sectionNote: 'For detailled pictures and descriptions, click the overview images!',
    open: 'Open',
    step: 'Step',
    stageCount: 'Stage',
    close: 'Close image viewer',
    previous: 'Previous image',
    next: 'Next image',
    switchLanguage: 'Sprache auf Deutsch umschalten',
    languageButton: 'Deutsch'
  }
};

photoCount.textContent = photos.length;

function getDescriptionPreview(description, wordLimit = 8) {
  const words = description.trim().split(/\s+/);
  return words.length > wordLimit ? `${words.slice(0, wordLimit).join(' ')}...` : description;
}

function linkifyDescription(description) {
  const escaped = description
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;')
    .replace(/'/g, '&#39;');

  return escaped.replace(/https?:\/\/[^\s<]+/g, (url) => {
    const trailing = url.match(/[.,!?;:)]+$/)?.[0] || '';
    const cleanUrl = trailing ? url.slice(0, -trailing.length) : url;
    return `<a href="${cleanUrl}" target="_blank" rel="noopener noreferrer">${cleanUrl}</a>${trailing}`;
  });
}

function renderGallery() {
  const text = interfaceText[language];

  gallery.innerHTML = photos.map((photo, index) => {
    const step = String(index + 1).padStart(2, '0');
    const title = `${text.step} ${step}`;
    const description = photo.description[language];

    return `
      <article class="photo-card" style="animation-delay: ${Math.min(index * 35, 600)}ms">
        <button class="photo-frame" type="button" data-photo-index="${index}" aria-label="${text.open} ${title}">
          <span class="step-number">${step}</span>
          <img src="${photo.file}" alt="${title}" loading="lazy">
        </button>
        <div class="photo-caption">
          <h3 class="photo-title">${title}</h3>
          <p class="photo-description">${linkifyDescription(getDescriptionPreview(description))}</p>
        </div>
      </article>
    `;
  }).join('');
}

function updateInterface() {
  const text = interfaceText[language];
  document.documentElement.lang = language;
  document.title = text.pageTitle;
  languageMeta.content = text.pageDescription;
  document.querySelector('#eyebrow').textContent = text.eyebrow;
  document.querySelector('#intro').textContent = text.intro;
  document.querySelector('#gallery-details').setAttribute('aria-label', text.galleryDetails);
  document.querySelector('#photo-count-label').textContent = text.photoCountLabel;
  document.querySelector('#sequence-note').textContent = text.sequenceNote;
  document.querySelector('#process-label').textContent = text.processLabel;
  document.querySelector('#gallery-title').textContent = text.galleryTitle;
  document.querySelector('#section-note').innerHTML = text.sectionNote;
  languageToggle.textContent = text.languageButton;
  languageToggle.setAttribute('aria-label', text.switchLanguage);
  document.querySelector('.lightbox-close').setAttribute('aria-label', text.close);
  document.querySelector('.lightbox-prev').setAttribute('aria-label', text.previous);
  document.querySelector('.lightbox-next').setAttribute('aria-label', text.next);
  renderGallery();
  if (lightbox.open) showPhoto(activePhoto);
}

function showPhoto(index) {
  activePhoto = (index + photos.length) % photos.length;
  const photo = photos[activePhoto];
  const step = String(activePhoto + 1).padStart(2, '0');
  const text = interfaceText[language];
  const title = `${text.step} ${step}`;

  lightboxImage.src = photo.file;
  lightboxImage.alt = title;
  lightboxStep.textContent = `${text.stageCount} ${step} / ${String(photos.length).padStart(2, '0')}`;
  lightboxTitle.textContent = title;
  lightboxDescription.innerHTML = linkifyDescription(photo.description[language]);
}

languageToggle.addEventListener('click', () => {
  language = language === 'de' ? 'en' : 'de';
  updateInterface();
});

updateInterface();

gallery.addEventListener('click', (event) => {
  const trigger = event.target.closest('[data-photo-index]');
  if (!trigger) return;

  showPhoto(Number(trigger.dataset.photoIndex));
  lightbox.showModal();
});

document.querySelector('.lightbox-close').addEventListener('click', () => lightbox.close());
document.querySelector('.lightbox-prev').addEventListener('click', () => showPhoto(activePhoto - 1));
document.querySelector('.lightbox-next').addEventListener('click', () => showPhoto(activePhoto + 1));

lightbox.addEventListener('click', (event) => {
  if (event.target === lightbox) lightbox.close();
});

document.addEventListener('keydown', (event) => {
  if (!lightbox.open) return;
  if (event.key === 'ArrowLeft') showPhoto(activePhoto - 1);
  if (event.key === 'ArrowRight') showPhoto(activePhoto + 1);
});

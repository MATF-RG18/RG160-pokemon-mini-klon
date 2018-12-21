# RG160-pokemon-mini-klon
Pokemon mini klon

v0.1
- Osnova programa
- Funkcija za crtanje trave za sad poligonom
- Struktura PokemonField koja oznacava celo polje trave u kojoj pokemoni mogu da se stvore
- Smanjivanje ambicioznosti projekta jer su najednostavnije stvari prekomplikovane u OpenGL-u i limitiranosti C-a(pravljenje mape koja je vam viewporta jer nema vizualnog engina, kolizije/trigeri, kretanje po gridu, korutine, animacije, postavljanje spritova, objektna orijentisanost ne postoji u c-u, a neki pokusaj kopiranja se radi strukturama sto opet komplikuje kod bez potrebe zbog baratanja memorijom i pointerima, itd...)

v0.2
- Ubacen igrac (kvadratic za sada) sa grid-based kretanjem (na klik nekog od wasd dugmeta pomera se 40unita u odgovarajucem smeru i ne moze da prima novi input tek kada je zavrsio kretanje.)
- Escape za gasenje aplikacije
- Pojavio se bag da se iscrtava neka fantomska linija od koordinata 0,0 (levo, dole) do trece kockice trave, nije ocigledno zasto, mozda OpenGL lose reaguje na 60fps iscrtavanja stvari u petlji

v0.2.1
- Mini update
- Boja kockice promenjena
- Igrac se crta posle svega, da bi bio najblizi kameri (2d igrica je pa nema potrebe baratati pozicijom na Z)
- Ubacena fja za kontrolisanje specijalnog inputa koja omogucava kretanje i na strelice

v0.2.2
- Popravljen bag pojavljivanja fantomske linije zamenjivanjem GL_POLYGON crtanja sa GL_QUADS crtanjem, iako ne bi smelo to da pravi ikakvu razliku.
- Promenjen naziv windowa u ime igrice

v0.3
- Napravljena klasa (ne bas posto je C) Pokemon
- 32 Pokemona ubacena u igricu raspodeljenih po retkosti (Common, Uncommon, Rare, Legendary)
- Napravljene funkcije za napad pokemona, generisanje random pokemona (Legendary 1%, rare 9%, Uncommon 20%, Common 70%), proveru dal je objekat u PokemonField promenljivoj (poziva se na kraju hodanja igraca)
- Leveling sistem Pokemona i njihovi statovi u odnosu na sam level
- Sekcija za testiranje napravljenih funkcija u inicijalizaciji programa, ispis u konzoli funkcionalan trenutno

v0.3.1
- Funkcija za printovanje statova pokemona na standardni izlaz (bice pretvorena u iscrtavanje informacija o pokemonu kada dodjem do grafickog predstavljanja borbe)
- Dodata promenljiva koja pokazuje retkost pokemona

v0.3.2
- Poboljsana kolizija sa krajem mape
- Popravljen bag gde se proveravalo dal je igrac u PokemonField-u bez kretanja ako je igrac na ivicama ekrana (sad ne ulazi u hodanje uopste ako je to slucaj, tako da ne dolazi ni do provere kad se hodanje zavrsi)

v0.4
- Funkcije za loadovanje BMP slika
- Sprite trave i glavnog lika
- Bagovi sa BMP fajlovima exportovanim iz GIMP-a iz nekoga razloga (redosled kanala boja drugaciji, offset drugaciji)

v0.5
- Napravljene BMP slike svih pokemona u igrici trenutno
- Pokemon ima promenljivu "sprite" koja sadrzi njegovu sliku
- Borbe pokemona, njena vizuelna predstava i funkcionalnost napada, na dugme space (treba helte iscrtavati, i napraviti na dnu ekrana prikaz mogucih akcija)
- Idalje bag sa BMP fajlovima napravljenim u GIMP-u ............................................

TODO:
- Importovanje spritova za igraca, travu, pokemone √√√
- Borba, drugacije iscrtavanje za borbu, graficko predstavljanje pokemona u borbi i njihovih statova (Ime,helti,exp,level) √√√
- Promena trenutnog pokemona koj se koristi za nekog od uhvacenih
- Pecanje (tu se dodaje onda jos pokemona)
- Pokecentar za lecenje pokemona
- Balansiranje i menjanje statova za svakog pokemona ponaosob (za sad se statovi razlikuju samo po retkosti pokemona) (mozda skidanje formula sa bulbapedije, sajta sa svim informacijama o pokemon igricama)
- Zvukovi
- Evolucije(?)

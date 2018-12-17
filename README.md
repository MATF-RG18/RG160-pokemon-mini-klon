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

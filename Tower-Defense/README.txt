//Oprea Ionut 334CB

Utilizare aplicatie:

	->Mouse: -Left-click(lansare proiectile)
	  		 -Right-click(orientare camera)

	->Taste: -W (miscarea inainte a playerului)
			 -A (miscarea in stanga a playerului)
			 -S (miscarea in spate a playerului)
			 -D (miscarea in dreapta a playerului)

			 -1 (selectia pistolului)
			 -2 (selectia pustii)
			 -3 (selectia aruncatorului de grenade)

Detalii de implementare:
	Pentru implementarea temei am folosit Laboratorul5 si clasele: Enemy, Weapon si Tower. Fiecare clasa a fost folosita pentru instantiere inamicilor, gloantelor
	pentru arme si a celor 3 turnuri. Am generat gloantele la apasarea pe butonul 
	Right-click al mouse-ului si le-am bagat in vectorul armei(weaponVector) pentru a putea fi randate in functia Update. Inamicii apar la fiecare 5 secunde si sunt adaugati de asemenea in vectorul de inamici enemyVector(definit in Laborator5.h) fiind randati mai apoi. Orientarea playerului se foloseste de un unghi rotateObj asemanator temei 1, doar ca de data aceasta se face orientarea dupa camera. De asemenea coliziunea este realizata asemanator cu cea din tema 1.

Probleme aparute:
	Probleme de implementare au aparut la realizarea minimap-ului.

Continutul arhivei:
	- Fisierul Framework-EGC-master(ce contine toate fisierele necesare temei)
	- README.txt

Status implementare functionalitati:
	Au fost implementate functionalitatile precizate in prezentarea temei, fara niciun bonus.
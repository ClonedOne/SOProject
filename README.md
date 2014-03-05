SOProject
=========

Internet based "Talk" system



Talk

Realizzazione di un sistema di talk via internet. Un processo su una
macchina server gestisce un elenco di tutti i client pronti a ricevere una
connessione.
I processi client (residenti, in generale, su macchine diverse), dopo
essersi connessi consultano l'elenco e lo propongono all'utente il quale potra'
collegarsi a un altro client disponibile iniziando cosi' una "conversazione".
Due client collegati tra loro permetteranno ai relativi utenti di scambiarsi
messaggi finche' uno dei due non interrompe la "conversazione", e saranno
indisponibili ad altre "conversazioni". Al termine di una "conversazione" i
client ritorneranno disponibili fino a che non si scollegano dal server.

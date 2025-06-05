# K-Deauther - Terminal Edition
K-Deauther è uno strumento da riga di comando per Linux che permette di eseguire attacchi di deautenticazione Wi-Fi utilizzando una scheda di rete compatibile con modalità monitor.

Scritto interamente in C++, il tool guida l’utente passo-passo nell’individuazione della rete target, selezione di dispositivi collegati e lancio dell’attacco. È pensato per scopi educativi e di test sulla sicurezza delle proprie reti wireless.

🛠️ Funzionalità principali:
Scansione delle interfacce Wi-Fi disponibili

Attivazione della modalità monitor

Scansione delle reti e dei dispositivi collegati

Attacco di deautenticazione:

contro un singolo dispositivo

contro l’intera rete

Tutto gestito da un’interfaccia testuale interattiva

⚠️ Avviso legale
Questo software è stato creato a scopo educativo. Utilizzalo solo su reti di tua proprietà o con autorizzazione esplicita. Qualsiasi uso improprio è illegale e non è responsabilità dell’autore.

# Requisiti
Linux (o shell compatibile: Bash, Zsh, WSL...)

aircrack-ng installato (viene installato automaticamente al primo avvio)

Interfaccia Wi-Fi compatibile con modalità monitor

# Avvio rapido

g++ k-deauther.cpp -o k-deauther
sudo ./k-deauther

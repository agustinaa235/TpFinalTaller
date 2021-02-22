#ifndef _SERVER_EVENT_RECEIVER
#define _SERVER_EVENT_RECEIVER

#include "thread.h"
#include "protected_queue.h"
#include "socket.h"
#include "comandos/comando.h"
#include "protocolo.h"
#include <atomic>

class Server_Event_Receiver : public Thread {
public:
    Server_Event_Receiver(ProtectedQueue<Comando *> &comandos, Protocolo *protocolo) :
            comandos(comandos), protocolo(protocolo), corriendo(true) {}

    ~Server_Event_Receiver() {
    /*  for (int i = 0; i < this->comandos.size(); i++){
        Comando* c = comandos.obtener_dato();
        delete c;
      }*/
    }

    void run() override;

    void cerrar();

    bool estaCorriendo();


private:
    ProtectedQueue<Comando *> &comandos;
    Protocolo *protocolo;
    std::atomic<bool> corriendo;

};

#endif

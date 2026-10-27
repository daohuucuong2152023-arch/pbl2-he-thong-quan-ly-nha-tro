#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

#include "data_structures/DoublyLinkedList.h"
#include "data_structures/BST.h"
#include "data_structures/Queue.h"
#include "data_structures/Stack.h"
#include "models/Room.h"
#include "models/Tenant.h"
#include "models/Contract.h"
#include "models/Invoice.h"
#include "models/Service.h"
#include "io/RoomIO.h"
#include "io/TenantIO.h"
#include "io/ContractIO.h"
#include "io/InvoiceIO.h"
#include "io/QueueIO.h"
#include "io/ServiceIO.h"
#include "ui/admin.h"

int main() {
    cout << fixed << setprecision(2);

    BST<Room> rooms;
    {
        DoublyLinkedList<Room> dsPhong = docRoomFile("data/room.txt");
        dsPhong.traverseForward([&rooms](const Room& r) {
            rooms.insert(r.getMaPhong(), r);
        });
    }
    DoublyLinkedList<Tenant> tenants = docTenantFile("data/tenant.txt");
    DoublyLinkedList<Contract> contracts = docContractFile("data/contract.txt");
    DoublyLinkedList<Invoice> invoices = docInvoiceFile("data/invoice.txt");
    DoublyLinkedList<Service> services = docServiceFile("data/service.txt");
    Queue<Tenant> checkinQueue;
    docQueueFile(checkinQueue, "data/queue.txt");

    AdminMenu menu(rooms, tenants, contracts, invoices, checkinQueue, services);
    bool canLuu = menu.run();

    if (canLuu) {
        DoublyLinkedList<Room> dsPhong;
        rooms.inorderTraverse([&dsPhong](const Room& r) {
            dsPhong.insertBack(r);
        });
        luuRoomFile(dsPhong, "data/room.txt");
        luuTenantFile(tenants, "data/tenant.txt");
        luuContractFile(contracts, "data/contract.txt");
        luuInvoiceFile(invoices, "data/invoice.txt");
        luuQueueFile(checkinQueue, "data/queue.txt");
        luuServiceFile(services, "data/service.txt");
        cout << "Da luu du lieu. Tam biet!" << endl;
    }
    return 0;
}

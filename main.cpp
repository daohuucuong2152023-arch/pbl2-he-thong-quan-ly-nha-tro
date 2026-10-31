#include <iostream>
#include <iomanip>
#include <string>

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
#include "services/UndoService.h"
#include "services/RoomService.h"
#include "services/TenantService.h"
#include "services/ContractService.h"
#include "services/InvoiceService.h"
#include "services/CheckinService.h"
#include "services/DichVuService.h"
#include "ui/admin.h"

using namespace std;

int main() {
    cout << fixed << setprecision(2);

  //1.DAL

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

    //2.BLL

    UndoService undoService(rooms, tenants, contracts, invoices);
    RoomService roomService(rooms, undoService);
    TenantService tenantService(tenants, undoService);
    ContractService contractService(contracts, tenants, undoService);
    InvoiceService invoiceService(invoices, undoService);
    CheckinService checkinService(checkinQueue, rooms, tenants, contracts, invoices, undoService);
    DichVuService dichVuService(services);


    //3.UI

    AdminMenu menu(roomService,
                  tenantService,
                  contractService,
                  invoiceService,
                  checkinService,
                  dichVuService,
                  undoService);

    bool canLuu = menu.chay();

    //luu giữ liệu khi thoát

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

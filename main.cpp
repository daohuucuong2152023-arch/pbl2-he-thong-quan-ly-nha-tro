
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
#include "io/RoomIO.h"
#include "io/TenantIO.h"
#include "io/ContractIO.h"
#include "io/InvoiceIO.h"

static void tieuDe(const string& s) { cout << "\n===== " << s << " =====" << endl; }

template <class T>
void inDanhSach(const DoublyLinkedList<T>& ds) {
    ds.traverseForward([](const T& x) { x.display(); });
}

bool laP102(const Room& r)       { return r.getMaPhong() == "P102"; }
bool laP999(const Room& r)       { return r.getMaPhong() == "P999"; }
bool laPhongTrong(const Room& r) { return r.isTrong(); }
void datDangThue(Room& r)        { r.setTrangThai("Dang thue"); }
void inRoom(const Room& r)       { r.display(); }

bool laHD001(const Contract& c)  { return c.getMaHopDong() == "HD001"; }
void ketThucHopDong(Contract& c) { c.ketThuc(); }

bool laTN002(const Tenant& t)    { return t.getMaNguoiThue() == "TN002"; }
void traPhong(Tenant& t)         { t.checkOut(); }

static void kiemTra(const string& ten, int truoc, int sau) {
    cout << ten << ": " << truoc << " -> " << sau << (truoc == sau ? "  [OK]" : "  [LOI]") << endl;
}

int main() {
    cout << fixed << setprecision(0); 
   
    tieuDe("1. DOC FILE TU data/");
    DoublyLinkedList<Room>     rooms     = docRoomFile("data/room.txt");
    DoublyLinkedList<Tenant>   tenants   = docTenantFile("data/tenant.txt");
    DoublyLinkedList<Contract> contracts = docContractFile("data/contract.txt");
    DoublyLinkedList<Invoice>  invoices  = docInvoiceFile("data/invoice.txt");

    cout << "Phong: " << rooms.size() << " | Nguoi thue: " << tenants.size()
         << " | Hop dong: " << contracts.size() << " | Hoa don: " << invoices.size() << endl;
    if (rooms.isEmpty()) {
        cout << "Khong doc duoc du lieu! Hay chay chuong trinh tu thu muc pbl2 (co thu muc data/)." << endl;
        return 1;
    }

    tieuDe("Danh sach phong");     inDanhSach(rooms);
    tieuDe("Danh sach nguoi thue"); inDanhSach(tenants);
    tieuDe("Danh sach hop dong");  inDanhSach(contracts);
    tieuDe("Danh sach hoa don");   inDanhSach(invoices);

    double tongChuaThu = 0;
    invoices.traverseForward([&tongChuaThu](const Invoice& i) {
        if (!i.isDaThanhToan()) tongChuaThu += i.tinhTongTien();
    });
    cout << "Tong tien hoa don chua thanh toan: " << tongChuaThu << " VND (ky vong 9509500)" << endl;

    tieuDe("2. DOUBLY LINKED LIST (phong)");
    Room r;
    if (rooms.findIf(laP102, r)) { cout << "findIf P102: "; r.display(); }
    if (rooms.findIf(laPhongTrong, r)) { cout << "Phong trong dau tien: "; r.display(); }

    rooms.updateIf(laPhongTrong, datDangThue);
    cout << "Sau updateIf (phong trong dau tien -> Dang thue):" << endl;
    if (rooms.findIf(laPhongTrong, r)) { cout << "Phong trong dau tien gio la: "; r.display(); }

    rooms.insertFront(Room("P000", 15, 1500000));
    rooms.insertBack(Room("P999", 40, 5000000));
    cout << "Sau insertFront P000 + insertBack P999: size = " << rooms.size() << endl;

    cout << "removeIf P999: " << (rooms.removeIf(laP999) ? "da xoa" : "khong thay") << " | size = " << rooms.size() << endl;
    cout << "removeIf P999 lan 2: " << (rooms.removeIf(laP999) ? "da xoa" : "khong thay (dung)") << endl;

    cout << "-- Duyet xuoi --" << endl;
    inDanhSach(rooms);
    cout << "-- Duyet nguoc --" << endl;
    rooms.traverseBackward([](const Room& x) { x.display(); });

    tieuDe("3. BST (tim phong theo ma)");
    BST bst;
    rooms.traverseForward([&bst](const Room& x) { bst.insert(x.getMaPhong(), x); });

    Room found;
    cout << "search P102: ";
    if (bst.search("P102", found)) found.display(); else cout << "khong thay" << endl;
    cout << "search P999: ";
    if (bst.search("P999", found)) found.display(); else cout << "khong thay (dung, da xoa)" << endl;

    bst.remove("P203");
    cout << "Da xoa P203 khoi BST. Duyet inorder (ma tang dan):" << endl;
    bst.inorderTraverse(inRoom);

    tieuDe("4. QUEUE (hang doi check-in)");
    Queue<Tenant> hangDoi;
    tenants.traverseForward([&hangDoi](const Tenant& t) {
        if (t.getTrangThai() == "Dang o") hangDoi.enqueue(t);
    });
    cout << "So khach trong hang doi: " << hangDoi.size() << endl;
    Tenant t;
    if (hangDoi.front(t)) cout << "Dau hang doi: " << t.getHoTen() << endl;
    while (hangDoi.dequeue(t)) cout << "Xu ly: " << t.getMaNguoiThue() << " - " << t.getHoTen() << endl;
    cout << "Hang doi rong? " << (hangDoi.isEmpty() ? "co" : "khong") << endl;

    tieuDe("5. STACK (hoan tac thao tac)");
    Stack<string> lichSu;
    lichSu.push("Them phong P000");
    lichSu.push("Them phong P999");
    lichSu.push("Xoa phong P203 khoi BST");
    string hd;
    while (lichSu.pop(hd)) cout << "Hoan tac: " << hd << endl;

    tieuDe("6. HOP DONG / NGUOI THUE");
    contracts.updateIf(laHD001, ketThucHopDong);
    Contract hd1;
    if (contracts.findIf(laHD001, hd1)) {
        hd1.display();
        cout << "isConHieuLuc: " << (hd1.isConHieuLuc() ? "true" : "false (dung)") << endl;
    }
    tenants.updateIf(laTN002, traPhong);
    Tenant tn2;
    if (tenants.findIf(laTN002, tn2)) tn2.display();

    tieuDe("7. GHI FILE -> DOC LAI (ghi ra data/test_*.txt, khong de len file goc)");
    luuRoomFile(rooms, "data/test_room.txt");
    luuTenantFile(tenants, "data/test_tenant.txt");
    luuContractFile(contracts, "data/test_contract.txt");
    luuInvoiceFile(invoices, "data/test_invoice.txt");

    kiemTra("Phong    ", rooms.size(),     docRoomFile("data/test_room.txt").size());
    kiemTra("Nguoi thue", tenants.size(),  docTenantFile("data/test_tenant.txt").size());
    kiemTra("Hop dong ", contracts.size(), docContractFile("data/test_contract.txt").size());
    kiemTra("Hoa don  ", invoices.size(),  docInvoiceFile("data/test_invoice.txt").size());

    Contract hdLai;
    DoublyLinkedList<Contract> contractsLai = docContractFile("data/test_contract.txt");
    if (contractsLai.findIf(laHD001, hdLai))
        cout << "HD001 sau khi doc lai: " << hdLai.getTrangThai() << " (ky vong: Da ket thuc)" << endl;

    cout << "\nXong." << endl;
    return 0;
}
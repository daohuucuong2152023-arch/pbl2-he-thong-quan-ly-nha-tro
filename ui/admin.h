#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <conio.h>

#include "../data_structures/BST.h"
#include "../data_structures/DoublyLinkedList.h"
#include "../data_structures/Queue.h"
#include "../data_structures/Stack.h"
#include "../models/Room.h"
#include "../models/Tenant.h"
#include "../models/Contract.h"
#include "../models/Invoice.h"

using namespace std;

namespace ui {
    void clearScreen();
    void pause();
    string trim(const string& s);
    string toUpper(string s);
    bool isAllDigits(const string& s);
    bool isValidMonth(const string& s);
    bool isValidDate(const string& s);

    string readLine(const string& prompt);
    string readText(const string& prompt, bool allowEmpty);
    int readInt(const string& prompt);
    int readPositiveInt(const string& prompt);
    double readDouble(const string& prompt, double minValue, bool strictlyGreater);
    double readPositiveDouble(const string& prompt);
    double readNonNegativeDouble(const string& prompt);
    double readOptionalPositiveDouble(const string& prompt, double current);
    string readDigits(const string& prompt, size_t length, bool allowEmpty);
    string readDate(const string& prompt);
    string readMonth(const string& prompt);
    bool confirm(const string& prompt);
    string readPassword(const string& prompt);
}

struct RoomAction {
    string loai;
    Room truoc;
    Room sau;

    RoomAction();
    RoomAction(const string& l, const Room& t, const Room& s);
};

class AdminMenu {
private:
    BST& rooms;
    DoublyLinkedList<Tenant>& tenants;
    DoublyLinkedList<Contract>& contracts;
    DoublyLinkedList<Invoice>& invoices;
    Queue<Tenant>& checkinQueue;
    Stack<RoomAction> undoStack;
    int tenantCounter;

    bool login();
    void showMainMenu() const;

    template <class T>
    int nextNumber(const DoublyLinkedList<T>& ds, const string& prefix) const;
    string taoMa(const string& prefix, int number) const;
    bool capNhatTrangThaiPhong(const string& maPhong, const string& trangThai);
    void xoaLichSuHoanTac();
    void hoanTac();

    void roomMenu();
    void themPhong();
    void suaPhong();
    void xoaPhong();
    void timPhong();
    void xemDanhSachPhong() const;
    int inPhongTrong() const;
    void xemPhongTrong() const;

    void checkinMenu();
    void inKhachCho(const Tenant& t) const;
    template <class Func>
    void duyetHangDoi(Func visit);
    void capNhatBoDemKhach();
    bool cccdDangCho(const string& cccd);
    void xemHangDoi();
    void dangKyCheckin();
    void xuLyCheckin();

    void tenantMenu();
    void xemKhach(bool chiDangO) const;
    void timKhach();
    void suaKhach();
    void traPhong();

    void contractMenu();
    void xemHopDong(bool chiConHieuLuc) const;
    void timHopDong();
    void giaHanHopDong();

    void invoiceMenu();
    void lapHoaDon();
    void xemHoaDon(bool chiChuaThu) const;
    void xemHoaDonTheoPhong() const;
    void thanhToanHoaDon();

public:
    AdminMenu(BST& r,
              DoublyLinkedList<Tenant>& t,
              DoublyLinkedList<Contract>& c,
              DoublyLinkedList<Invoice>& i,
              Queue<Tenant>& q);

    bool run();
};

inline void ui::clearScreen() {
    system("cls");
}

inline void ui::pause() {
    cout << "\nNhan Enter de tiep tuc...";
    string s;
    getline(cin, s);
}

inline string ui::trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

inline string ui::toUpper(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        s[i] = (char)toupper((unsigned char)s[i]);
    }
    return s;
}

inline bool ui::isAllDigits(const string& s) {
    if (s.empty()) return false;
    for (size_t i = 0; i < s.size(); i++) {
        if (!isdigit((unsigned char)s[i])) return false;
    }
    return true;
}

inline bool ui::isValidMonth(const string& s) {
    if (s.size() != 7 || s[4] != '-') return false;
    if (!isAllDigits(s.substr(0, 4)) || !isAllDigits(s.substr(5, 2))) return false;
    int m = stoi(s.substr(5, 2));
    return m >= 1 && m <= 12;
}

inline bool ui::isValidDate(const string& s) {
    if (s.size() != 10 || s[7] != '-') return false;
    if (!isValidMonth(s.substr(0, 7))) return false;
    if (!isAllDigits(s.substr(8, 2))) return false;
    int d = stoi(s.substr(8, 2));
    return d >= 1 && d <= 31;
}

inline string ui::readLine(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) {
        cout << "\nKhong doc duoc du lieu nhap. Thoat chuong trinh." << endl;
        exit(0);
    }
    return s;
}

inline string ui::readText(const string& prompt, bool allowEmpty) {
    while (true) {
        string s = trim(readLine(prompt));
        if (s.find('|') != string::npos) {
            cout << "Khong duoc nhap ky tu '|'!" << endl;
        } else if (s.empty() && !allowEmpty) {
            cout << "Khong duoc de trong!" << endl;
        } else {
            return s;
        }
    }
}

inline int ui::readInt(const string& prompt) {
    while (true) {
        string s = trim(readLine(prompt));
        try {
            size_t pos = 0;
            int v = stoi(s, &pos);
            if (pos == s.size()) return v;
        } catch (...) {
        }
        cout << "Vui long nhap so nguyen!" << endl;
    }
}

inline int ui::readPositiveInt(const string& prompt) {
    while (true) {
        int v = readInt(prompt);
        if (v > 0) return v;
        cout << "Gia tri phai lon hon 0!" << endl;
    }
}

inline double ui::readDouble(const string& prompt, double minValue, bool strictlyGreater) {
    while (true) {
        string s = trim(readLine(prompt));
        try {
            size_t pos = 0;
            double v = stod(s, &pos);
            if (pos == s.size()) {
                if (strictlyGreater ? (v > minValue) : (v >= minValue)) return v;
                cout << "Gia tri phai " << (strictlyGreater ? "lon hon " : "tu ") << minValue << " tro len!" << endl;
                continue;
            }
        } catch (...) {
        }
        cout << "Vui long nhap mot so!" << endl;
    }
}

inline double ui::readPositiveDouble(const string& prompt) {
    return readDouble(prompt, 0, true);
}

inline double ui::readNonNegativeDouble(const string& prompt) {
    return readDouble(prompt, 0, false);
}

inline double ui::readOptionalPositiveDouble(const string& prompt, double current) {
    while (true) {
        string s = trim(readLine(prompt));
        if (s.empty()) return current;
        try {
            size_t pos = 0;
            double v = stod(s, &pos);
            if (pos == s.size() && v > 0) return v;
        } catch (...) {
        }
        cout << "Vui long nhap so lon hon 0 (hoac Enter de giu nguyen)!" << endl;
    }
}

inline string ui::readDigits(const string& prompt, size_t length, bool allowEmpty) {
    while (true) {
        string s = trim(readLine(prompt));
        if (s.empty() && allowEmpty) return s;
        if (isAllDigits(s) && s.size() == length) return s;
        cout << "Phai gom dung " << length << " chu so!" << endl;
    }
}

inline string ui::readDate(const string& prompt) {
    while (true) {
        string s = trim(readLine(prompt));
        if (isValidDate(s)) return s;
        cout << "Ngay khong hop le (dinh dang YYYY-MM-DD, VD: 2026-10-24)!" << endl;
    }
}

inline string ui::readMonth(const string& prompt) {
    while (true) {
        string s = trim(readLine(prompt));
        if (isValidMonth(s)) return s;
        cout << "Thang khong hop le (dinh dang YYYY-MM, VD: 2026-10)!" << endl;
    }
}

inline bool ui::confirm(const string& prompt) {
    string s = trim(readLine(prompt + " (y/n): "));
    return s == "y" || s == "Y";
}

inline string ui::readPassword(const string& prompt) {
    cout << prompt;
    string pw;
    char c;
    while ((c = _getch()) != '\r') {
        if (c == '\b') {
            if (!pw.empty()) {
                pw.pop_back();
                cout << "\b \b";
            }
        } else {
            pw += c;
            cout << '*';
        }
    }
    cout << endl;
    return pw;
}

inline RoomAction::RoomAction() {}

inline RoomAction::RoomAction(const string& l, const Room& t, const Room& s)
    : loai(l), truoc(t), sau(s) {}

inline AdminMenu::AdminMenu(BST& r,
                            DoublyLinkedList<Tenant>& t,
                            DoublyLinkedList<Contract>& c,
                            DoublyLinkedList<Invoice>& i,
                            Queue<Tenant>& q)
    : rooms(r), tenants(t), contracts(c), invoices(i), checkinQueue(q), tenantCounter(0) {
    tenantCounter = nextNumber(tenants, "TN") - 1;
    capNhatBoDemKhach();
}

inline bool AdminMenu::login() {
    const string ADMIN_USER = "admin";
    const string ADMIN_PASS = "123456";
    const int MAX_ATTEMPTS = 3;

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        ui::clearScreen();
        cout << "==============================" << endl;
        cout << "   QUAN LY NHA TRO - ADMIN" << endl;
        cout << "==============================" << endl;
        string user = ui::trim(ui::readLine("Tai khoan: "));
        string pass = ui::readPassword("Mat khau : ");
        if (user == ADMIN_USER && pass == ADMIN_PASS) return true;
        cout << "Sai tai khoan hoac mat khau (" << attempt << "/" << MAX_ATTEMPTS << ")" << endl;
        ui::pause();
    }
    return false;
}

inline void AdminMenu::showMainMenu() const {
    int tongPhong = 0, phongTrong = 0;
    rooms.inorderTraverse([&tongPhong, &phongTrong](const Room& r) {
        tongPhong++;
        if (r.isTrong()) phongTrong++;
    });

    int khachDangO = 0;
    tenants.traverseForward([&khachDangO](const Tenant& t) {
        if (t.getTrangThai() == "Dang o") khachDangO++;
    });

    int hoaDonChuaThu = 0;
    invoices.traverseForward([&hoaDonChuaThu](const Invoice& h) {
        if (!h.isDaThanhToan()) hoaDonChuaThu++;
    });

    ui::clearScreen();
    cout << "========== QUAN LY NHA TRO ==========" << endl;
    cout << "Phong: " << tongPhong << " (trong: " << phongTrong << ")"
         << " | Khach dang o: " << khachDangO << endl;
    cout << "Cho check-in: " << checkinQueue.size()
         << " | Hoa don chua thu: " << hoaDonChuaThu << endl;
    cout << "-------------------------------------" << endl;
    cout << "1. Quan ly phong" << endl;
    cout << "2. Quan ly khach thue" << endl;
    cout << "3. Quan ly hop dong" << endl;
    cout << "4. Quan ly hoa don" << endl;
    cout << "5. Check-in (hang doi)" << endl;
    cout << "6. Hoan tac thao tac phong gan nhat (" << undoStack.size() << ")" << endl;
    cout << "0. Luu va thoat" << endl;
    cout << "-------------------------------------" << endl;
}

template <class T>
int AdminMenu::nextNumber(const DoublyLinkedList<T>& ds, const string& prefix) const {
    int maxNo = 0;
    ds.traverseForward([&maxNo, &prefix](const T& x) {
        string id = x.getId();
        if (id.size() > prefix.size() && id.compare(0, prefix.size(), prefix) == 0) {
            string rest = id.substr(prefix.size());
            if (ui::isAllDigits(rest) && rest.size() <= 9) {
                int n = stoi(rest);
                if (n > maxNo) maxNo = n;
            }
        }
    });
    return maxNo + 1;
}

inline string AdminMenu::taoMa(const string& prefix, int number) const {
    string s = to_string(number);
    while (s.size() < 3) s = "0" + s;
    return prefix + s;
}

inline bool AdminMenu::capNhatTrangThaiPhong(const string& maPhong, const string& trangThai) {
    Room r;
    if (!rooms.search(maPhong, r)) return false;
    r.setTrangThai(trangThai);
    rooms.insert(maPhong, r);
    return true;
}

inline void AdminMenu::xoaLichSuHoanTac() {
    while (!undoStack.isEmpty()) undoStack.pop();
}

inline void AdminMenu::hoanTac() {
    RoomAction a;
    if (!undoStack.pop(a)) {
        cout << "Khong co thao tac nao de hoan tac." << endl;
        ui::pause();
        return;
    }

    if (a.loai == "THEM") {
        rooms.remove(a.sau.getMaPhong());
        cout << "Da hoan tac: them phong " << a.sau.getMaPhong() << " (phong da bi go bo)." << endl;
    } else if (a.loai == "SUA") {
        rooms.insert(a.truoc.getMaPhong(), a.truoc);
        cout << "Da hoan tac: sua phong " << a.truoc.getMaPhong() << " (tra ve gia tri cu)." << endl;
    } else if (a.loai == "XOA") {
        rooms.insert(a.truoc.getMaPhong(), a.truoc);
        cout << "Da hoan tac: xoa phong " << a.truoc.getMaPhong() << " (phong da duoc khoi phuc)." << endl;
    }
    ui::pause();
}

inline void AdminMenu::roomMenu() {
    int choice;
    do {
        ui::clearScreen();
        cout << "======== QUAN LY PHONG ========" << endl;
        cout << "1. Them phong" << endl;
        cout << "2. Sua phong" << endl;
        cout << "3. Xoa phong" << endl;
        cout << "4. Tim phong theo ma" << endl;
        cout << "5. Xem danh sach (theo ma tang dan)" << endl;
        cout << "6. Xem phong trong" << endl;
        cout << "0. Quay lai" << endl;
        choice = ui::readInt("Chon: ");

        switch (choice) {
            case 1: themPhong(); break;
            case 2: suaPhong(); break;
            case 3: xoaPhong(); break;
            case 4: timPhong(); break;
            case 5: xemDanhSachPhong(); break;
            case 6: xemPhongTrong(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::pause();
        }
    } while (choice != 0);
}

inline void AdminMenu::themPhong() {
    string ma = ui::toUpper(ui::readText("Ma phong (VD: P301): ", false));
    Room tonTai;
    if (rooms.search(ma, tonTai)) {
        cout << "Ma phong " << ma << " da ton tai!" << endl;
        ui::pause();
        return;
    }
    double dienTich = ui::readPositiveDouble("Dien tich (m2): ");
    double giaCoBan = ui::readPositiveDouble("Gia co ban (VND/thang): ");

    Room moi(ma, dienTich, giaCoBan, "Trong");
    rooms.insert(ma, moi);
    undoStack.push(RoomAction("THEM", Room(), moi));

    cout << "Da them phong:" << endl;
    moi.display();
    ui::pause();
}

inline void AdminMenu::suaPhong() {
    string ma = ui::toUpper(ui::readText("Nhap ma phong can sua: ", false));
    Room r;
    if (!rooms.search(ma, r)) {
        cout << "Khong tim thay phong " << ma << endl;
        ui::pause();
        return;
    }
    r.display();
    cout << "(Nhan Enter de giu nguyen gia tri cu)" << endl;

    Room truoc = r;
    r.setDienTich(ui::readOptionalPositiveDouble("Dien tich moi: ", r.getDienTich()));
    r.setGiaCoBan(ui::readOptionalPositiveDouble("Gia co ban moi: ", r.getGiaCoBan()));

    if (r.getTrangThai() == "Dang thue") {
        cout << "Phong dang co nguoi thue nen khong doi duoc trang thai." << endl;
    } else {
        cout << "Trang thai moi: 1. Trong  2. Dang sua chua" << endl;
        string tt = ui::readText("Chon (Enter de giu nguyen): ", true);
        if (tt == "1") r.setTrangThai("Trong");
        else if (tt == "2") r.setTrangThai("Dang sua chua");
        else if (!tt.empty()) cout << "Lua chon khong hop le, giu nguyen trang thai." << endl;
    }

    rooms.insert(ma, r);
    undoStack.push(RoomAction("SUA", truoc, r));

    cout << "Da cap nhat phong:" << endl;
    r.display();
    ui::pause();
}

inline void AdminMenu::xoaPhong() {
    string ma = ui::toUpper(ui::readText("Nhap ma phong can xoa: ", false));
    Room r;
    if (!rooms.search(ma, r)) {
        cout << "Khong tim thay phong " << ma << endl;
        ui::pause();
        return;
    }
    r.display();
    if (r.getTrangThai() == "Dang thue") {
        cout << "Phong dang co nguoi thue, khong the xoa. Hay lam thu tuc tra phong truoc." << endl;
        ui::pause();
        return;
    }
    if (ui::confirm("Ban co chac muon xoa phong " + ma + "?")) {
        rooms.remove(ma);
        undoStack.push(RoomAction("XOA", r, Room()));
        cout << "Da xoa phong " << ma << endl;
    } else {
        cout << "Da huy." << endl;
    }
    ui::pause();
}

inline void AdminMenu::timPhong() {
    string ma = ui::toUpper(ui::readText("Nhap ma phong can tim: ", false));
    Room r;
    if (!rooms.search(ma, r)) {
        cout << "Khong tim thay phong " << ma << endl;
        ui::pause();
        return;
    }
    r.display();
    if (r.getTrangThai() == "Dang thue") {
        Tenant t;
        bool co = tenants.findIf([&ma](const Tenant& x) {
            return x.getMaPhong() == ma && x.getTrangThai() == "Dang o";
        }, t);
        if (co) {
            cout << "Nguoi dang thue:" << endl;
            t.display();
        }
    }
    ui::pause();
}

inline void AdminMenu::xemDanhSachPhong() const {
    cout << "------ DANH SACH PHONG ------" << endl;
    if (rooms.isEmpty()) {
        cout << "(Chua co phong nao)" << endl;
    } else {
        rooms.inorderTraverse([](const Room& r) { r.display(); });
    }
    ui::pause();
}

inline int AdminMenu::inPhongTrong() const {
    int dem = 0;
    cout << "------ PHONG TRONG ------" << endl;
    rooms.inorderTraverse([&dem](const Room& r) {
        if (r.isTrong()) {
            r.display();
            dem++;
        }
    });
    if (dem == 0) cout << "(Khong con phong trong)" << endl;
    return dem;
}

inline void AdminMenu::xemPhongTrong() const {
    inPhongTrong();
    ui::pause();
}

inline void AdminMenu::inKhachCho(const Tenant& t) const {
    cout << "[Khach cho] " << t.getMaNguoiThue()
         << " | " << t.getHoTen()
         << " | CCCD: " << t.getCccd()
         << " | SDT: " << t.getSoDienThoai() << endl;
}

inline void AdminMenu::checkinMenu() {
    int choice;
    do {
        ui::clearScreen();
        cout << "======== CHECK-IN (HANG DOI) ========" << endl;
        cout << "So khach dang cho: " << checkinQueue.size() << endl;
        Tenant dauHang;
        if (checkinQueue.front(dauHang)) {
            cout << "Khach dau hang: ";
            inKhachCho(dauHang);
        }
        cout << "-------------------------------------" << endl;
        cout << "1. Dang ky khach vao hang doi" << endl;
        cout << "2. Xu ly khach dau hang (xep phong + tao hop dong)" << endl;
        cout << "3. Xem danh sach khach dang cho" << endl;
        cout << "0. Quay lai" << endl;
        choice = ui::readInt("Chon: ");

        switch (choice) {
            case 1: dangKyCheckin(); break;
            case 2: xuLyCheckin(); break;
            case 3: xemHangDoi(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::pause();
        }
    } while (choice != 0);
}

template <class Func>
void AdminMenu::duyetHangDoi(Func visit) {
    int n = checkinQueue.size();
    for (int i = 0; i < n; i++) {
        Tenant t;
        checkinQueue.dequeue(t);
        visit(t);
        checkinQueue.enqueue(t);
    }
}

inline void AdminMenu::capNhatBoDemKhach() {
    duyetHangDoi([this](const Tenant& t) {
        string id = t.getMaNguoiThue();
        if (id.size() > 2 && id.compare(0, 2, "TN") == 0) {
            string rest = id.substr(2);
            if (ui::isAllDigits(rest) && rest.size() <= 9) {
                int v = stoi(rest);
                if (v > tenantCounter) tenantCounter = v;
            }
        }
    });
}

inline bool AdminMenu::cccdDangCho(const string& cccd) {
    bool co = false;
    duyetHangDoi([&cccd, &co](const Tenant& t) {
        if (t.getCccd() == cccd) co = true;
    });
    return co;
}

inline void AdminMenu::xemHangDoi() {
    cout << "------ KHACH DANG CHO CHECK-IN ------" << endl;
    if (checkinQueue.isEmpty()) {
        cout << "(Khong co khach nao dang cho)" << endl;
    }
    int stt = 0;
    duyetHangDoi([this, &stt](const Tenant& t) {
        cout << ++stt << ". ";
        inKhachCho(t);
    });
    ui::pause();
}

inline void AdminMenu::dangKyCheckin() {
    string ten = ui::readText("Ho ten: ", false);
    string cccd = ui::readDigits("CCCD (12 so): ", 12, false);

    Tenant daO;
    bool trung = tenants.findIf([&cccd](const Tenant& x) {
        return x.getCccd() == cccd && x.getTrangThai() == "Dang o";
    }, daO);
    if (trung) {
        cout << "CCCD nay dang thue phong " << daO.getMaPhong() << " (khach " << daO.getMaNguoiThue() << ")!" << endl;
        ui::pause();
        return;
    }

    if (cccdDangCho(cccd)) {
        cout << "CCCD nay da co trong hang doi!" << endl;
        ui::pause();
        return;
    }

    string sdt = ui::readDigits("So dien thoai (10 so): ", 10, false);

    string ma = taoMa("TN", ++tenantCounter);
    Tenant t(ma, ten, cccd, sdt, "", 0, "");
    checkinQueue.enqueue(t);

    cout << "Da them vao hang doi. Ma khach: " << ma << " (vi tri: " << checkinQueue.size() << ")" << endl;
    ui::pause();
}

inline void AdminMenu::xuLyCheckin() {
    Tenant t;
    if (!checkinQueue.front(t)) {
        cout << "Khong co khach nao dang cho." << endl;
        ui::pause();
        return;
    }
    cout << "Khach dau hang: ";
    inKhachCho(t);

    if (inPhongTrong() == 0) {
        ui::pause();
        return;
    }

    string maPhong = ui::toUpper(ui::readText("Chon ma phong: ", false));
    Room r;
    if (!rooms.search(maPhong, r)) {
        cout << "Khong tim thay phong " << maPhong << endl;
        ui::pause();
        return;
    }
    if (!r.isTrong()) {
        cout << "Phong " << maPhong << " khong trong (" << r.getTrangThai() << ")!" << endl;
        ui::pause();
        return;
    }

    string ngay = ui::readDate("Ngay check-in (YYYY-MM-DD): ");
    int thoiHan = ui::readPositiveInt("Thoi han hop dong (thang): ");
    double coc = ui::readOptionalPositiveDouble("Tien coc (Enter = bang gia phong): ", r.getGiaCoBan());

    if (!ui::confirm("Xac nhan xep khach " + t.getHoTen() + " vao phong " + maPhong + "?")) {
        cout << "Da huy." << endl;
        ui::pause();
        return;
    }

    Tenant chinhThuc(t.getMaNguoiThue(), t.getHoTen(), t.getCccd(), t.getSoDienThoai(),
                     maPhong, coc, ngay);
    string maHD = taoMa("HD", nextNumber(contracts, "HD"));
    Contract hd(maHD, maPhong, chinhThuc.getMaNguoiThue(), ngay, thoiHan, coc);

    tenants.insertBack(chinhThuc);
    contracts.insertBack(hd);
    r.setTrangThai("Dang thue");
    rooms.insert(maPhong, r);

    Tenant boQua;
    checkinQueue.dequeue(boQua);
    xoaLichSuHoanTac();

    cout << "Check-in thanh cong!" << endl;
    chinhThuc.display();
    hd.display();
    ui::pause();
}

inline void AdminMenu::tenantMenu() {
    int choice;
    do {
        ui::clearScreen();
        cout << "======== QUAN LY KHACH THUE ========" << endl;
        cout << "1. Xem tat ca khach" << endl;
        cout << "2. Xem khach dang o" << endl;
        cout << "3. Tim khach theo ma" << endl;
        cout << "4. Sua thong tin khach" << endl;
        cout << "5. Tra phong (check-out)" << endl;
        cout << "0. Quay lai" << endl;
        choice = ui::readInt("Chon: ");

        switch (choice) {
            case 1: xemKhach(false); break;
            case 2: xemKhach(true); break;
            case 3: timKhach(); break;
            case 4: suaKhach(); break;
            case 5: traPhong(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::pause();
        }
    } while (choice != 0);
}

inline void AdminMenu::xemKhach(bool chiDangO) const {
    cout << "------ " << (chiDangO ? "KHACH DANG O" : "TAT CA KHACH THUE") << " ------" << endl;
    int dem = 0;
    tenants.traverseForward([chiDangO, &dem](const Tenant& t) {
        if (!chiDangO || t.getTrangThai() == "Dang o") {
            t.display();
            dem++;
        }
    });
    if (dem == 0) cout << "(Khong co khach nao)" << endl;
    ui::pause();
}

inline void AdminMenu::timKhach() {
    string ma = ui::toUpper(ui::readText("Nhap ma khach can tim: ", false));
    Tenant t;
    if (tenants.findIf([&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; }, t)) {
        t.display();
    } else {
        cout << "Khong tim thay khach " << ma << endl;
    }
    ui::pause();
}

inline void AdminMenu::suaKhach() {
    string ma = ui::toUpper(ui::readText("Nhap ma khach can sua: ", false));
    Tenant t;
    if (!tenants.findIf([&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; }, t)) {
        cout << "Khong tim thay khach " << ma << endl;
        ui::pause();
        return;
    }
    t.display();
    cout << "(Nhan Enter de giu nguyen gia tri cu)" << endl;

    string ten = ui::readText("Ho ten moi: ", true);
    string cccd = ui::readDigits("CCCD moi (12 so): ", 12, true);
    string sdt = ui::readDigits("SDT moi (10 so): ", 10, true);

    tenants.updateIf(
        [&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; },
        [&ten, &cccd, &sdt](Tenant& x) {
            if (!ten.empty()) x.setHoTen(ten);
            if (!cccd.empty()) x.setCccd(cccd);
            if (!sdt.empty()) x.setSoDienThoai(sdt);
        });

    cout << "Da cap nhat." << endl;
    ui::pause();
}

inline void AdminMenu::traPhong() {
    string ma = ui::toUpper(ui::readText("Nhap ma khach tra phong: ", false));
    Tenant t;
    if (!tenants.findIf([&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; }, t)) {
        cout << "Khong tim thay khach " << ma << endl;
        ui::pause();
        return;
    }
    if (t.getTrangThai() != "Dang o") {
        cout << "Khach nay da tra phong roi." << endl;
        ui::pause();
        return;
    }

    string phong = t.getMaPhong();
    int chuaThu = 0;
    invoices.traverseForward([&phong, &chuaThu](const Invoice& h) {
        if (h.getMaPhong() == phong && !h.isDaThanhToan()) chuaThu++;
    });
    t.display();
    if (chuaThu > 0) {
        cout << "CANH BAO: phong " << phong << " con " << chuaThu << " hoa don chua thanh toan!" << endl;
    }

    if (!ui::confirm("Xac nhan tra phong " + phong + " cho khach " + t.getHoTen() + "?")) {
        cout << "Da huy." << endl;
        ui::pause();
        return;
    }

    tenants.updateIf(
        [&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; },
        [](Tenant& x) { x.checkOut(); });
    contracts.updateIf(
        [&ma](const Contract& c) { return c.getMaNguoiThue() == ma && c.isConHieuLuc(); },
        [](Contract& c) { c.ketThuc(); });
    capNhatTrangThaiPhong(phong, "Trong");
    xoaLichSuHoanTac();

    cout << "Tra phong thanh cong. Phong " << phong << " da tro ve trang thai Trong." << endl;
    cout << "Tien coc can hoan lai: " << t.getTienCoc() << " VND" << endl;
    ui::pause();
}

inline void AdminMenu::contractMenu() {
    int choice;
    do {
        ui::clearScreen();
        cout << "======== QUAN LY HOP DONG ========" << endl;
        cout << "1. Xem tat ca hop dong" << endl;
        cout << "2. Xem hop dong con hieu luc" << endl;
        cout << "3. Tim hop dong theo ma" << endl;
        cout << "4. Gia han hop dong" << endl;
        cout << "0. Quay lai" << endl;
        choice = ui::readInt("Chon: ");

        switch (choice) {
            case 1: xemHopDong(false); break;
            case 2: xemHopDong(true); break;
            case 3: timHopDong(); break;
            case 4: giaHanHopDong(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::pause();
        }
    } while (choice != 0);
}

inline void AdminMenu::xemHopDong(bool chiConHieuLuc) const {
    cout << "------ " << (chiConHieuLuc ? "HOP DONG CON HIEU LUC" : "TAT CA HOP DONG") << " ------" << endl;
    int dem = 0;
    contracts.traverseForward([chiConHieuLuc, &dem](const Contract& c) {
        if (!chiConHieuLuc || c.isConHieuLuc()) {
            c.display();
            dem++;
        }
    });
    if (dem == 0) cout << "(Khong co hop dong nao)" << endl;
    ui::pause();
}

inline void AdminMenu::timHopDong() {
    string ma = ui::toUpper(ui::readText("Nhap ma hop dong can tim: ", false));
    Contract c;
    if (contracts.findIf([&ma](const Contract& x) { return x.getMaHopDong() == ma; }, c)) {
        c.display();
    } else {
        cout << "Khong tim thay hop dong " << ma << endl;
    }
    ui::pause();
}

inline void AdminMenu::giaHanHopDong() {
    string ma = ui::toUpper(ui::readText("Nhap ma hop dong can gia han: ", false));
    Contract c;
    if (!contracts.findIf([&ma](const Contract& x) { return x.getMaHopDong() == ma; }, c)) {
        cout << "Khong tim thay hop dong " << ma << endl;
        ui::pause();
        return;
    }
    if (!c.isConHieuLuc()) {
        cout << "Hop dong da ket thuc, khong gia han duoc." << endl;
        ui::pause();
        return;
    }
    c.display();
    int them = ui::readPositiveInt("Gia han them (thang): ");
    int thoiHanMoi = c.getThoiHan() + them;

    contracts.updateIf(
        [&ma](const Contract& x) { return x.getMaHopDong() == ma; },
        [thoiHanMoi](Contract& x) { x.setThoiHan(thoiHanMoi); });

    cout << "Da gia han. Thoi han moi: " << thoiHanMoi << " thang." << endl;
    ui::pause();
}

inline void AdminMenu::invoiceMenu() {
    int choice;
    do {
        ui::clearScreen();
        cout << "======== QUAN LY HOA DON ========" << endl;
        cout << "1. Lap hoa don thang moi" << endl;
        cout << "2. Xem tat ca hoa don" << endl;
        cout << "3. Xem hoa don chua thanh toan" << endl;
        cout << "4. Xem hoa don theo phong" << endl;
        cout << "5. Thanh toan hoa don" << endl;
        cout << "0. Quay lai" << endl;
        choice = ui::readInt("Chon: ");

        switch (choice) {
            case 1: lapHoaDon(); break;
            case 2: xemHoaDon(false); break;
            case 3: xemHoaDon(true); break;
            case 4: xemHoaDonTheoPhong(); break;
            case 5: thanhToanHoaDon(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::pause();
        }
    } while (choice != 0);
}

inline void AdminMenu::lapHoaDon() {
    string maPhong = ui::toUpper(ui::readText("Ma phong: ", false));
    Room r;
    if (!rooms.search(maPhong, r)) {
        cout << "Khong tim thay phong " << maPhong << endl;
        ui::pause();
        return;
    }
    if (r.getTrangThai() != "Dang thue") {
        cout << "Phong " << maPhong << " khong co nguoi thue, khong lap hoa don." << endl;
        ui::pause();
        return;
    }

    string thang = ui::readMonth("Thang (YYYY-MM): ");
    bool trung = false;
    invoices.traverseForward([&maPhong, &thang, &trung](const Invoice& h) {
        if (h.getMaPhong() == maPhong && h.getThangNam() == thang) trung = true;
    });
    if (trung) {
        cout << "Phong " << maPhong << " da co hoa don thang " << thang << "!" << endl;
        ui::pause();
        return;
    }

    double dienCu = 0, nuocCu = 0;
    bool coHoaDonCu = false;
    string thangCu;
    invoices.traverseForward([&](const Invoice& h) {
        if (h.getMaPhong() == maPhong && (!coHoaDonCu || h.getThangNam() > thangCu)) {
            coHoaDonCu = true;
            thangCu = h.getThangNam();
            dienCu = h.getChiSoDienMoi();
            nuocCu = h.getChiSoNuocMoi();
        }
    });

    if (coHoaDonCu) {
        cout << "Chi so cu lay tu hoa don thang " << thangCu
             << " (dien: " << dienCu << ", nuoc: " << nuocCu << ")" << endl;
    } else {
        dienCu = ui::readNonNegativeDouble("Chi so dien cu: ");
        nuocCu = ui::readNonNegativeDouble("Chi so nuoc cu: ");
    }

    double dienMoi = ui::readDouble("Chi so dien moi: ", dienCu, false);
    double nuocMoi = ui::readDouble("Chi so nuoc moi: ", nuocCu, false);
    double giaDien = ui::readOptionalPositiveDouble("Don gia dien (Enter = 3500): ", 3500);
    double giaNuoc = ui::readOptionalPositiveDouble("Don gia nuoc (Enter = 15000): ", 15000);

    string maHD = taoMa("HDN", nextNumber(invoices, "HDN"));
    Invoice hd(maHD, maPhong, thang, dienCu, dienMoi, nuocCu, nuocMoi,
               giaDien, giaNuoc, r.getGiaCoBan());
    invoices.insertBack(hd);

    cout << "Da lap hoa don:" << endl;
    hd.display();
    ui::pause();
}

inline void AdminMenu::xemHoaDon(bool chiChuaThu) const {
    cout << "------ " << (chiChuaThu ? "HOA DON CHUA THANH TOAN" : "TAT CA HOA DON") << " ------" << endl;
    int dem = 0;
    double tong = 0;
    invoices.traverseForward([chiChuaThu, &dem, &tong](const Invoice& h) {
        if (!chiChuaThu || !h.isDaThanhToan()) {
            h.display();
            dem++;
            if (!h.isDaThanhToan()) tong += h.tinhTongTien();
        }
    });
    if (dem == 0) {
        cout << "(Khong co hoa don nao)" << endl;
    } else {
        cout << "Tong tien chua thu: " << tong << " VND" << endl;
    }
    ui::pause();
}

inline void AdminMenu::xemHoaDonTheoPhong() const {
    string maPhong = ui::toUpper(ui::readText("Nhap ma phong: ", false));
    cout << "------ HOA DON PHONG " << maPhong << " ------" << endl;
    int dem = 0;
    invoices.traverseForward([&maPhong, &dem](const Invoice& h) {
        if (h.getMaPhong() == maPhong) {
            h.display();
            dem++;
        }
    });
    if (dem == 0) cout << "(Phong nay chua co hoa don nao)" << endl;
    ui::pause();
}

inline void AdminMenu::thanhToanHoaDon() {
    string ma = ui::toUpper(ui::readText("Nhap ma hoa don: ", false));
    Invoice h;
    if (!invoices.findIf([&ma](const Invoice& x) { return x.getMaHoaDon() == ma; }, h)) {
        cout << "Khong tim thay hoa don " << ma << endl;
        ui::pause();
        return;
    }
    h.display();
    if (h.isDaThanhToan()) {
        cout << "Hoa don nay da thanh toan roi." << endl;
        ui::pause();
        return;
    }
    if (ui::confirm("Xac nhan khach da thanh toan " + to_string((long long)h.tinhTongTien()) + " VND?")) {
        invoices.updateIf(
            [&ma](const Invoice& x) { return x.getMaHoaDon() == ma; },
            [](Invoice& x) { x.danhDauDaThanhToan(); });
        cout << "Da ghi nhan thanh toan." << endl;
    } else {
        cout << "Da huy." << endl;
    }
    ui::pause();
}

inline bool AdminMenu::run() {
    if (!login()) {
        cout << "Dang nhap that bai. Thoat chuong trinh." << endl;
        return false;
    }

    int choice;
    do {
        showMainMenu();
        choice = ui::readInt("Chon: ");

        switch (choice) {
            case 1: roomMenu(); break;
            case 2: tenantMenu(); break;
            case 3: contractMenu(); break;
            case 4: invoiceMenu(); break;
            case 5: checkinMenu(); break;
            case 6: hoanTac(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::pause();
        }
    } while (choice != 0);

    return true;
}

#endif

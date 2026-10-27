#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <iostream>
#include <string>

#include "../data_structures/BST.h"
#include "../data_structures/DoublyLinkedList.h"
#include "../data_structures/Queue.h"
#include "../data_structures/Stack.h"
#include "../models/Room.h"
#include "../models/Tenant.h"
#include "../models/Contract.h"
#include "../models/Invoice.h"
#include "../models/Service.h"
#include "InputHelper.h"
#include "UndoAction.h"

using namespace std;

class AdminMenu {
private:
    BST<Room>& rooms;
    DoublyLinkedList<Tenant>& tenants;
    DoublyLinkedList<Contract>& contracts;
    DoublyLinkedList<Invoice>& invoices;
    Queue<Tenant>& checkinQueue;
    DoublyLinkedList<Service>& services;
    Stack<ThaoTacHoanTac> nganXepHoanTac;
    int tenantCounter;

    bool dangNhap();
    void hienThiMenuChinh() const;

    template <class T>
    int soTiepTheo(const DoublyLinkedList<T>& ds, const string& prefix) const;
    string taoMa(const string& prefix, int number) const;
    bool capNhatTrangThaiPhong(const string& maPhong, const string& trangThai);
    void xoaLichSuHoanTac();
    void hoanTac();

    void menuPhong();
    void themPhong();
    void suaPhong();
    void xoaPhong();
    void timPhong();
    void xemDanhSachPhong() const;
    int inPhongTrong() const;
    void xemPhongTrong() const;

    void menuCheckin();
    void inKhachCho(const Tenant& t) const;
    template <class Func>
    void duyetHangDoi(Func visit);
    void capNhatBoDemKhach();
    bool cccdDangCho(const string& cccd);
    void xemHangDoi();
    void dangKyCheckin();
    void xuLyCheckin();

    void menuKhach();
    void xemKhach(bool chiDangO) const;
    void timKhach();
    void suaKhach();
    void traPhong();
    void xoaKhach();

    void menuHopDong();
    void xemHopDong(bool chiConHieuLuc) const;
    void timHopDong();
    void giaHanHopDong();
    void xoaHopDong();
    void suaTienCoc();

    void menuHoaDon();
    void lapHoaDon();
    void xemHoaDon(bool chiChuaThu) const;
    void xemHoaDonTheoPhong() const;
    void thanhToanHoaDon();
    void xoaHoaDon();
    void suaChiSoHoaDon();

    void menuDichVu();
    void themDichVu();
    void suaDichVu();
    void xoaDichVu();
    void xemDichVu() const;

public:
    AdminMenu(BST<Room>& r,
              DoublyLinkedList<Tenant>& t,
              DoublyLinkedList<Contract>& c,
              DoublyLinkedList<Invoice>& i,
              Queue<Tenant>& q,
              DoublyLinkedList<Service>& s);

    bool chay();
};

inline AdminMenu::AdminMenu(BST<Room>& r,
                            DoublyLinkedList<Tenant>& t,
                            DoublyLinkedList<Contract>& c,
                            DoublyLinkedList<Invoice>& i,
                            Queue<Tenant>& q,
                            DoublyLinkedList<Service>& s)
    : rooms(r), tenants(t), contracts(c), invoices(i), checkinQueue(q), services(s), tenantCounter(0) {
    tenantCounter = soTiepTheo(tenants, "TN") - 1;
    capNhatBoDemKhach();
}

inline bool AdminMenu::dangNhap() {
    const string ADMIN_USER = "admin";
    const string ADMIN_PASS = "123456";
    const int MAX_ATTEMPTS = 3;

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        ui::xoaManHinh();
        cout << "==============================" << endl;
        cout << "   QUAN LY NHA TRO - ADMIN" << endl;
        cout << "==============================" << endl;
        string user = ui::catKhoangTrang(ui::docDong("Tai khoan: "));
        string pass = ui::nhapMatKhau("Mat khau : ");
        if (user == ADMIN_USER && pass == ADMIN_PASS) return true;
        cout << "Sai tai khoan hoac mat khau (" << attempt << "/" << MAX_ATTEMPTS << ")" << endl;
        ui::tamDung();
    }
    return false;
}

inline void AdminMenu::hienThiMenuChinh() const {
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

    ui::xoaManHinh();
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
    cout << "6. Hoan tac thao tac gan nhat (" << nganXepHoanTac.size() << ")" << endl;
    cout << "7. Quan ly dich vu" << endl;
    cout << "0. Luu va thoat" << endl;
    cout << "-------------------------------------" << endl;
}

template <class T>
int AdminMenu::soTiepTheo(const DoublyLinkedList<T>& ds, const string& prefix) const {
    int maxNo = 0;
    ds.traverseForward([&maxNo, &prefix](const T& x) {
        string id = x.getId();
        if (id.size() > prefix.size() && id.compare(0, prefix.size(), prefix) == 0) {
            string rest = id.substr(prefix.size());
            if (ui::laChuSo(rest) && rest.size() <= 9) {
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
    while (!nganXepHoanTac.isEmpty()) nganXepHoanTac.pop();
}

inline void AdminMenu::hoanTac() {
    ThaoTacHoanTac a;
    if (!nganXepHoanTac.pop(a)) {
        cout << "Khong co thao tac nao de hoan tac." << endl;
        ui::tamDung();
        return;
    }

    if (a.loai == "THEM_PHONG") {
        rooms.remove(a.phongSau.getMaPhong());
        cout << "Da hoan tac: them phong " << a.phongSau.getMaPhong() << " (phong da bi go bo)." << endl;
    } else if (a.loai == "SUA_PHONG") {
        rooms.insert(a.phongTruoc.getMaPhong(), a.phongTruoc);
        cout << "Da hoan tac: sua phong " << a.phongTruoc.getMaPhong() << " (tra ve gia tri cu)." << endl;
    } else if (a.loai == "XOA_PHONG") {
        rooms.insert(a.phongTruoc.getMaPhong(), a.phongTruoc);
        cout << "Da hoan tac: xoa phong " << a.phongTruoc.getMaPhong() << " (phong da duoc khoi phuc)." << endl;
    } else if (a.loai == "XOA_KHACH") {
        tenants.insertBack(a.khach);
        cout << "Da hoan tac: xoa khach " << a.khach.getMaNguoiThue() << " (khach da duoc khoi phuc)." << endl;
    } else if (a.loai == "XOA_HOPDONG") {
        contracts.insertBack(a.hopDong);
        cout << "Da hoan tac: xoa hop dong " << a.hopDong.getMaHopDong() << " (hop dong da duoc khoi phuc)." << endl;
    } else if (a.loai == "XOA_HOADON") {
        invoices.insertBack(a.hoaDon);
        cout << "Da hoan tac: xoa hoa don " << a.hoaDon.getMaHoaDon() << " (hoa don da duoc khoi phuc)." << endl;
    } else if (a.loai == "SUA_HOADON") {
        string maHD = a.hoaDon.getMaHoaDon();
        double dienCu = a.hoaDon.getChiSoDienMoi();
        double nuocCu = a.hoaDon.getChiSoNuocMoi();
        invoices.updateIf(
            [&maHD](const Invoice& x) { return x.getMaHoaDon() == maHD; },
            [dienCu, nuocCu](Invoice& x) { x.capNhatChiSoMoi(dienCu, nuocCu); });
        cout << "Da hoan tac: sua chi so hoa don " << maHD << " (tra ve chi so cu)." << endl;
    } else if (a.loai == "SUA_COC") {
        string maHD = a.hopDong.getMaHopDong();
        string maKhach = a.hopDong.getMaNguoiThue();
        double cocHopDongCu = a.hopDong.getTienCoc();
        double cocKhachCu = a.khach.getTienCoc();
        contracts.updateIf(
            [&maHD](const Contract& x) { return x.getMaHopDong() == maHD; },
            [cocHopDongCu](Contract& x) { x.setTienCoc(cocHopDongCu); });
        tenants.updateIf(
            [&maKhach](const Tenant& x) { return x.getMaNguoiThue() == maKhach; },
            [cocKhachCu](Tenant& x) { x.setTienCoc(cocKhachCu); });
        cout << "Da hoan tac: sua tien coc hop dong " << maHD << " (tra ve tien coc cu)." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::menuPhong() {
    int choice;
    do {
        ui::xoaManHinh();
        cout << "======== QUAN LY PHONG ========" << endl;
        cout << "1. Them phong" << endl;
        cout << "2. Sua phong" << endl;
        cout << "3. Xoa phong" << endl;
        cout << "4. Tim phong theo ma" << endl;
        cout << "5. Xem danh sach (theo ma tang dan)" << endl;
        cout << "6. Xem phong trong" << endl;
        cout << "0. Quay lai" << endl;
        choice = ui::nhapSoNguyen("Chon: ");

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
                ui::tamDung();
        }
    } while (choice != 0);
}

inline void AdminMenu::themPhong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Ma phong (VD: P301): ", false));
    Room tonTai;
    if (rooms.search(ma, tonTai)) {
        cout << "Ma phong " << ma << " da ton tai!" << endl;
        ui::tamDung();
        return;
    }
    double dienTich = ui::nhapSoThucDuong("Dien tich (m2): ");
    double giaCoBan = ui::nhapSoThucDuong("Gia co ban (VND/thang): ");

    Room moi(ma, dienTich, giaCoBan, "Trong");
    rooms.insert(ma, moi);
    ThaoTacHoanTac a("THEM_PHONG");
    a.phongSau = moi;
    nganXepHoanTac.push(a);

    cout << "Da them phong:" << endl;
    moi.display();
    ui::tamDung();
}

inline void AdminMenu::suaPhong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma phong can sua: ", false));
    Room r;
    if (!rooms.search(ma, r)) {
        cout << "Khong tim thay phong " << ma << endl;
        ui::tamDung();
        return;
    }
    r.display();
    cout << "(Nhan Enter de giu nguyen gia tri cu)" << endl;

    Room truoc = r;
    r.setDienTich(ui::nhapTuyChonSoThucDuong("Dien tich moi: ", r.getDienTich()));
    r.setGiaCoBan(ui::nhapTuyChonSoThucDuong("Gia co ban moi: ", r.getGiaCoBan()));

    if (r.getTrangThai() == "Dang thue") {
        cout << "Phong dang co nguoi thue nen khong doi duoc trang thai." << endl;
    } else {
        cout << "Trang thai moi: 1. Trong  2. Dang sua chua" << endl;
        string tt = ui::nhapChuoi("Chon (Enter de giu nguyen): ", true);
        if (tt == "1") r.setTrangThai("Trong");
        else if (tt == "2") r.setTrangThai("Dang sua chua");
        else if (!tt.empty()) cout << "Lua chon khong hop le, giu nguyen trang thai." << endl;
    }

    rooms.insert(ma, r);
    ThaoTacHoanTac a("SUA_PHONG");
    a.phongTruoc = truoc;
    a.phongSau = r;
    nganXepHoanTac.push(a);

    cout << "Da cap nhat phong:" << endl;
    r.display();
    ui::tamDung();
}

inline void AdminMenu::xoaPhong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma phong can xoa: ", false));
    Room r;
    if (!rooms.search(ma, r)) {
        cout << "Khong tim thay phong " << ma << endl;
        ui::tamDung();
        return;
    }
    r.display();
    if (r.getTrangThai() == "Dang thue") {
        cout << "Phong dang co nguoi thue, khong the xoa. Hay lam thu tuc tra phong truoc." << endl;
        ui::tamDung();
        return;
    }
    if (ui::xacNhan("Ban co chac muon xoa phong " + ma + "?")) {
        rooms.remove(ma);
        ThaoTacHoanTac a("XOA_PHONG");
        a.phongTruoc = r;
        nganXepHoanTac.push(a);
        cout << "Da xoa phong " << ma << endl;
    } else {
        cout << "Da huy." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::timPhong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma phong can tim: ", false));
    Room r;
    if (!rooms.search(ma, r)) {
        cout << "Khong tim thay phong " << ma << endl;
        ui::tamDung();
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
    ui::tamDung();
}

inline void AdminMenu::xemDanhSachPhong() const {
    cout << "------ DANH SACH PHONG ------" << endl;
    if (rooms.isEmpty()) {
        cout << "(Chua co phong nao)" << endl;
    } else {
        rooms.inorderTraverse([](const Room& r) { r.display(); });
    }
    ui::tamDung();
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
    ui::tamDung();
}

inline void AdminMenu::inKhachCho(const Tenant& t) const {
    cout << "[Khach cho] " << t.getMaNguoiThue()
         << " | " << t.getHoTen()
         << " | CCCD: " << t.getCccd()
         << " | SDT: " << t.getSoDienThoai() << endl;
}

inline void AdminMenu::menuCheckin() {
    int choice;
    do {
        ui::xoaManHinh();
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
        choice = ui::nhapSoNguyen("Chon: ");

        switch (choice) {
            case 1: dangKyCheckin(); break;
            case 2: xuLyCheckin(); break;
            case 3: xemHangDoi(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::tamDung();
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
            if (ui::laChuSo(rest) && rest.size() <= 9) {
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
    ui::tamDung();
}

inline void AdminMenu::dangKyCheckin() {
    string ten = ui::nhapChuoi("Ho ten: ", false);
    string cccd = ui::nhapChuSo("CCCD (12 so): ", 12, false);

    Tenant daO;
    bool trung = tenants.findIf([&cccd](const Tenant& x) {
        return x.getCccd() == cccd && x.getTrangThai() == "Dang o";
    }, daO);
    if (trung) {
        cout << "CCCD nay dang thue phong " << daO.getMaPhong() << " (khach " << daO.getMaNguoiThue() << ")!" << endl;
        ui::tamDung();
        return;
    }

    if (cccdDangCho(cccd)) {
        cout << "CCCD nay da co trong hang doi!" << endl;
        ui::tamDung();
        return;
    }

    string sdt = ui::nhapChuSo("So dien thoai (10 so): ", 10, false);

    string ma = taoMa("TN", ++tenantCounter);
    Tenant t(ma, ten, cccd, sdt, "", 0, "");
    checkinQueue.enqueue(t);

    cout << "Da them vao hang doi. Ma khach: " << ma << " (vi tri: " << checkinQueue.size() << ")" << endl;
    ui::tamDung();
}

inline void AdminMenu::xuLyCheckin() {
    Tenant t;
    if (!checkinQueue.front(t)) {
        cout << "Khong co khach nao dang cho." << endl;
        ui::tamDung();
        return;
    }
    cout << "Khach dau hang: ";
    inKhachCho(t);

    if (inPhongTrong() == 0) {
        ui::tamDung();
        return;
    }

    string maPhong = ui::vietHoa(ui::nhapChuoi("Chon ma phong: ", false));
    Room r;
    if (!rooms.search(maPhong, r)) {
        cout << "Khong tim thay phong " << maPhong << endl;
        ui::tamDung();
        return;
    }
    if (!r.isTrong()) {
        cout << "Phong " << maPhong << " khong trong (" << r.getTrangThai() << ")!" << endl;
        ui::tamDung();
        return;
    }

    string ngay = ui::nhapNgay("Ngay check-in (YYYY-MM-DD): ");
    int thoiHan = ui::nhapSoNguyenDuong("Thoi han hop dong (thang): ");
    double coc = ui::nhapTuyChonSoThucDuong("Tien coc (Enter = bang gia phong): ", r.getGiaCoBan());

    if (!ui::xacNhan("Xac nhan xep khach " + t.getHoTen() + " vao phong " + maPhong + "?")) {
        cout << "Da huy." << endl;
        ui::tamDung();
        return;
    }

    Tenant chinhThuc(t.getMaNguoiThue(), t.getHoTen(), t.getCccd(), t.getSoDienThoai(),
                     maPhong, coc, ngay);
    string maHD = taoMa("HD", soTiepTheo(contracts, "HD"));
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
    ui::tamDung();
}

inline void AdminMenu::menuKhach() {
    int choice;
    do {
        ui::xoaManHinh();
        cout << "======== QUAN LY KHACH THUE ========" << endl;
        cout << "1. Xem tat ca khach" << endl;
        cout << "2. Xem khach dang o" << endl;
        cout << "3. Tim khach theo ma" << endl;
        cout << "4. Sua thong tin khach" << endl;
        cout << "5. Tra phong (check-out)" << endl;
        cout << "6. Xoa khach da tra phong" << endl;
        cout << "0. Quay lai" << endl;
        choice = ui::nhapSoNguyen("Chon: ");

        switch (choice) {
            case 1: xemKhach(false); break;
            case 2: xemKhach(true); break;
            case 3: timKhach(); break;
            case 4: suaKhach(); break;
            case 5: traPhong(); break;
            case 6: xoaKhach(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::tamDung();
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
    ui::tamDung();
}

inline void AdminMenu::timKhach() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma khach can tim: ", false));
    Tenant t;
    if (tenants.findIf([&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; }, t)) {
        t.display();
    } else {
        cout << "Khong tim thay khach " << ma << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::suaKhach() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma khach can sua: ", false));
    Tenant t;
    if (!tenants.findIf([&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; }, t)) {
        cout << "Khong tim thay khach " << ma << endl;
        ui::tamDung();
        return;
    }
    t.display();
    cout << "(Nhan Enter de giu nguyen gia tri cu)" << endl;

    string ten = ui::nhapChuoi("Ho ten moi: ", true);
    string cccd = ui::nhapChuSo("CCCD moi (12 so): ", 12, true);
    string sdt = ui::nhapChuSo("SDT moi (10 so): ", 10, true);

    tenants.updateIf(
        [&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; },
        [&ten, &cccd, &sdt](Tenant& x) {
            if (!ten.empty()) x.setHoTen(ten);
            if (!cccd.empty()) x.setCccd(cccd);
            if (!sdt.empty()) x.setSoDienThoai(sdt);
        });

    cout << "Da cap nhat." << endl;
    ui::tamDung();
}

inline void AdminMenu::traPhong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma khach tra phong: ", false));
    Tenant t;
    if (!tenants.findIf([&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; }, t)) {
        cout << "Khong tim thay khach " << ma << endl;
        ui::tamDung();
        return;
    }
    if (t.getTrangThai() != "Dang o") {
        cout << "Khach nay da tra phong roi." << endl;
        ui::tamDung();
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

    if (!ui::xacNhan("Xac nhan tra phong " + phong + " cho khach " + t.getHoTen() + "?")) {
        cout << "Da huy." << endl;
        ui::tamDung();
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
    ui::tamDung();
}

inline void AdminMenu::xoaKhach() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma khach can xoa: ", false));
    Tenant t;
    if (!tenants.findIf([&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; }, t)) {
        cout << "Khong tim thay khach " << ma << endl;
        ui::tamDung();
        return;
    }
    t.display();
    if (t.getTrangThai() == "Dang o") {
        cout << "Khach dang o, hay lam thu tuc tra phong truoc." << endl;
        ui::tamDung();
        return;
    }
    Contract hd;
    if (contracts.findIf([&ma](const Contract& c) { return c.getMaNguoiThue() == ma; }, hd)) {
        cout << "Khach con hop dong " << hd.getMaHopDong() << ", hay xoa hop dong truoc." << endl;
        ui::tamDung();
        return;
    }
    if (ui::xacNhan("Ban co chac muon xoa khach " + ma + "?")) {
        ThaoTacHoanTac a("XOA_KHACH");
        a.khach = t;
        nganXepHoanTac.push(a);
        tenants.removeIf([&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; });
        cout << "Da xoa khach " << ma << endl;
    } else {
        cout << "Da huy." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::menuHopDong() {
    int choice;
    do {
        ui::xoaManHinh();
        cout << "======== QUAN LY HOP DONG ========" << endl;
        cout << "1. Xem tat ca hop dong" << endl;
        cout << "2. Xem hop dong con hieu luc" << endl;
        cout << "3. Tim hop dong theo ma" << endl;
        cout << "4. Gia han hop dong" << endl;
        cout << "5. Xoa hop dong da ket thuc" << endl;
        cout << "6. Sua tien coc hop dong" << endl;
        cout << "0. Quay lai" << endl;
        choice = ui::nhapSoNguyen("Chon: ");

        switch (choice) {
            case 1: xemHopDong(false); break;
            case 2: xemHopDong(true); break;
            case 3: timHopDong(); break;
            case 4: giaHanHopDong(); break;
            case 5: xoaHopDong(); break;
            case 6: suaTienCoc(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::tamDung();
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
    ui::tamDung();
}

inline void AdminMenu::timHopDong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hop dong can tim: ", false));
    Contract c;
    if (contracts.findIf([&ma](const Contract& x) { return x.getMaHopDong() == ma; }, c)) {
        c.display();
    } else {
        cout << "Khong tim thay hop dong " << ma << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::giaHanHopDong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hop dong can gia han: ", false));
    Contract c;
    if (!contracts.findIf([&ma](const Contract& x) { return x.getMaHopDong() == ma; }, c)) {
        cout << "Khong tim thay hop dong " << ma << endl;
        ui::tamDung();
        return;
    }
    if (!c.isConHieuLuc()) {
        cout << "Hop dong da ket thuc, khong gia han duoc." << endl;
        ui::tamDung();
        return;
    }
    c.display();
    int them = ui::nhapSoNguyenDuong("Gia han them (thang): ");
    int thoiHanMoi = c.getThoiHan() + them;

    contracts.updateIf(
        [&ma](const Contract& x) { return x.getMaHopDong() == ma; },
        [thoiHanMoi](Contract& x) { x.setThoiHan(thoiHanMoi); });

    cout << "Da gia han. Thoi han moi: " << thoiHanMoi << " thang." << endl;
    ui::tamDung();
}

inline void AdminMenu::xoaHopDong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hop dong can xoa: ", false));
    Contract c;
    if (!contracts.findIf([&ma](const Contract& x) { return x.getMaHopDong() == ma; }, c)) {
        cout << "Khong tim thay hop dong " << ma << endl;
        ui::tamDung();
        return;
    }
    c.display();
    if (c.isConHieuLuc()) {
        cout << "Hop dong con hieu luc, hay lam thu tuc tra phong truoc." << endl;
        ui::tamDung();
        return;
    }
    if (ui::xacNhan("Ban co chac muon xoa hop dong " + ma + "?")) {
        ThaoTacHoanTac a("XOA_HOPDONG");
        a.hopDong = c;
        nganXepHoanTac.push(a);
        contracts.removeIf([&ma](const Contract& x) { return x.getMaHopDong() == ma; });
        cout << "Da xoa hop dong " << ma << endl;
    } else {
        cout << "Da huy." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::suaTienCoc() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hop dong can sua tien coc: ", false));
    Contract c;
    if (!contracts.findIf([&ma](const Contract& x) { return x.getMaHopDong() == ma; }, c)) {
        cout << "Khong tim thay hop dong " << ma << endl;
        ui::tamDung();
        return;
    }
    if (!c.isConHieuLuc()) {
        cout << "Hop dong da ket thuc, khong sua duoc." << endl;
        ui::tamDung();
        return;
    }
    c.display();
    double cocMoi = ui::nhapTuyChonSoThucDuong("Tien coc moi (Enter de giu nguyen): ", c.getTienCoc());
    string maKhach = c.getMaNguoiThue();

    ThaoTacHoanTac a("SUA_COC");
    a.hopDong = c;
    tenants.findIf([&maKhach](const Tenant& x) { return x.getMaNguoiThue() == maKhach; }, a.khach);
    nganXepHoanTac.push(a);

    contracts.updateIf(
        [&ma](const Contract& x) { return x.getMaHopDong() == ma; },
        [cocMoi](Contract& x) { x.setTienCoc(cocMoi); });
    tenants.updateIf(
        [&maKhach](const Tenant& x) { return x.getMaNguoiThue() == maKhach; },
        [cocMoi](Tenant& x) { x.setTienCoc(cocMoi); });

    cout << "Da cap nhat tien coc: " << cocMoi << " VND (hop dong va khach thue)." << endl;
    ui::tamDung();
}

inline void AdminMenu::menuHoaDon() {
    int choice;
    do {
        ui::xoaManHinh();
        cout << "======== QUAN LY HOA DON ========" << endl;
        cout << "1. Lap hoa don thang moi" << endl;
        cout << "2. Xem tat ca hoa don" << endl;
        cout << "3. Xem hoa don chua thanh toan" << endl;
        cout << "4. Xem hoa don theo phong" << endl;
        cout << "5. Thanh toan hoa don" << endl;
        cout << "6. Xoa hoa don chua thanh toan" << endl;
        cout << "7. Sua chi so hoa don chua thanh toan" << endl;
        cout << "0. Quay lai" << endl;
        choice = ui::nhapSoNguyen("Chon: ");

        switch (choice) {
            case 1: lapHoaDon(); break;
            case 2: xemHoaDon(false); break;
            case 3: xemHoaDon(true); break;
            case 4: xemHoaDonTheoPhong(); break;
            case 5: thanhToanHoaDon(); break;
            case 6: xoaHoaDon(); break;
            case 7: suaChiSoHoaDon(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::tamDung();
        }
    } while (choice != 0);
}

inline void AdminMenu::lapHoaDon() {
    string maPhong = ui::vietHoa(ui::nhapChuoi("Ma phong: ", false));
    Room r;
    if (!rooms.search(maPhong, r)) {
        cout << "Khong tim thay phong " << maPhong << endl;
        ui::tamDung();
        return;
    }
    if (r.getTrangThai() != "Dang thue") {
        cout << "Phong " << maPhong << " khong co nguoi thue, khong lap hoa don." << endl;
        ui::tamDung();
        return;
    }

    string thang = ui::nhapThang("Thang (YYYY-MM): ");
    bool trung = false;
    invoices.traverseForward([&maPhong, &thang, &trung](const Invoice& h) {
        if (h.getMaPhong() == maPhong && h.getThangNam() == thang) trung = true;
    });
    if (trung) {
        cout << "Phong " << maPhong << " da co hoa don thang " << thang << "!" << endl;
        ui::tamDung();
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
        dienCu = ui::nhapSoThucKhongAm("Chi so dien cu: ");
        nuocCu = ui::nhapSoThucKhongAm("Chi so nuoc cu: ");
    }

    double dienMoi = ui::nhapSoThuc("Chi so dien moi: ", dienCu, false);
    double nuocMoi = ui::nhapSoThuc("Chi so nuoc moi: ", nuocCu, false);
    double giaDien = ui::nhapTuyChonSoThucDuong("Don gia dien (Enter = 3500): ", 3500);
    double giaNuoc = ui::nhapTuyChonSoThucDuong("Don gia nuoc (Enter = 15000): ", 15000);

    string maHD = taoMa("HDN", soTiepTheo(invoices, "HDN"));
    Invoice hd(maHD, maPhong, thang, dienCu, dienMoi, nuocCu, nuocMoi,
               giaDien, giaNuoc, r.getGiaCoBan());
    invoices.insertBack(hd);
    xoaLichSuHoanTac();

    cout << "Da lap hoa don:" << endl;
    hd.display();
    ui::tamDung();
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
    ui::tamDung();
}

inline void AdminMenu::xemHoaDonTheoPhong() const {
    string maPhong = ui::vietHoa(ui::nhapChuoi("Nhap ma phong: ", false));
    cout << "------ HOA DON PHONG " << maPhong << " ------" << endl;
    int dem = 0;
    invoices.traverseForward([&maPhong, &dem](const Invoice& h) {
        if (h.getMaPhong() == maPhong) {
            h.display();
            dem++;
        }
    });
    if (dem == 0) cout << "(Phong nay chua co hoa don nao)" << endl;
    ui::tamDung();
}

inline void AdminMenu::thanhToanHoaDon() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hoa don: ", false));
    Invoice h;
    if (!invoices.findIf([&ma](const Invoice& x) { return x.getMaHoaDon() == ma; }, h)) {
        cout << "Khong tim thay hoa don " << ma << endl;
        ui::tamDung();
        return;
    }
    h.display();
    if (h.isDaThanhToan()) {
        cout << "Hoa don nay da thanh toan roi." << endl;
        ui::tamDung();
        return;
    }
    if (ui::xacNhan("Xac nhan khach da thanh toan " + to_string((long long)h.tinhTongTien()) + " VND?")) {
        invoices.updateIf(
            [&ma](const Invoice& x) { return x.getMaHoaDon() == ma; },
            [](Invoice& x) { x.danhDauDaThanhToan(); });
        xoaLichSuHoanTac();
        cout << "Da ghi nhan thanh toan." << endl;
    } else {
        cout << "Da huy." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::xoaHoaDon() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hoa don can xoa: ", false));
    Invoice h;
    if (!invoices.findIf([&ma](const Invoice& x) { return x.getMaHoaDon() == ma; }, h)) {
        cout << "Khong tim thay hoa don " << ma << endl;
        ui::tamDung();
        return;
    }
    h.display();
    if (h.isDaThanhToan()) {
        cout << "Hoa don da thanh toan, khong duoc xoa." << endl;
        ui::tamDung();
        return;
    }
    if (ui::xacNhan("Ban co chac muon xoa hoa don " + ma + "?")) {
        ThaoTacHoanTac a("XOA_HOADON");
        a.hoaDon = h;
        nganXepHoanTac.push(a);
        invoices.removeIf([&ma](const Invoice& x) { return x.getMaHoaDon() == ma; });
        cout << "Da xoa hoa don " << ma << endl;
    } else {
        cout << "Da huy." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::suaChiSoHoaDon() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hoa don can sua chi so: ", false));
    Invoice h;
    if (!invoices.findIf([&ma](const Invoice& x) { return x.getMaHoaDon() == ma; }, h)) {
        cout << "Khong tim thay hoa don " << ma << endl;
        ui::tamDung();
        return;
    }
    h.display();
    if (h.isDaThanhToan()) {
        cout << "Hoa don da thanh toan, khong sua duoc." << endl;
        ui::tamDung();
        return;
    }

    string maPhong = h.getMaPhong();
    string thang = h.getThangNam();
    bool coMoiHon = false;
    invoices.traverseForward([&maPhong, &thang, &coMoiHon](const Invoice& x) {
        if (x.getMaPhong() == maPhong && x.getThangNam() > thang) coMoiHon = true;
    });
    if (coMoiHon) {
        cout << "Phong " << maPhong << " da co hoa don thang sau, chi sua duoc hoa don moi nhat." << endl;
        ui::tamDung();
        return;
    }

    cout << "(Nhan Enter de giu nguyen gia tri hien tai)" << endl;
    double dienMoi = ui::nhapTuyChonSoThuc("Chi so dien moi (hien tai " + to_string((long long)h.getChiSoDienMoi()) +
                                            ", cu " + to_string((long long)h.getChiSoDienCu()) + "): ",
                                            h.getChiSoDienMoi(), h.getChiSoDienCu());
    double nuocMoi = ui::nhapTuyChonSoThuc("Chi so nuoc moi (hien tai " + to_string((long long)h.getChiSoNuocMoi()) +
                                            ", cu " + to_string((long long)h.getChiSoNuocCu()) + "): ",
                                            h.getChiSoNuocMoi(), h.getChiSoNuocCu());

    if (dienMoi != h.getChiSoDienMoi() || nuocMoi != h.getChiSoNuocMoi()) {
        ThaoTacHoanTac a("SUA_HOADON");
        a.hoaDon = h;
        nganXepHoanTac.push(a);
    }
    invoices.updateIf(
        [&ma](const Invoice& x) { return x.getMaHoaDon() == ma; },
        [dienMoi, nuocMoi](Invoice& x) { x.capNhatChiSoMoi(dienMoi, nuocMoi); });

    invoices.findIf([&ma](const Invoice& x) { return x.getMaHoaDon() == ma; }, h);
    cout << "Da cap nhat:" << endl;
    h.display();
    ui::tamDung();
}

inline void AdminMenu::menuDichVu() {
    int choice;
    do {
        ui::xoaManHinh();
        cout << "======== QUAN LY DICH VU ========" << endl;
        cout << "1. Them dich vu" << endl;
        cout << "2. Sua dich vu" << endl;
        cout << "3. Xoa dich vu" << endl;
        cout << "4. Xem danh sach dich vu" << endl;
        cout << "0. Quay lai" << endl;
        choice = ui::nhapSoNguyen("Chon: ");

        switch (choice) {
            case 1: themDichVu(); break;
            case 2: suaDichVu(); break;
            case 3: xoaDichVu(); break;
            case 4: xemDichVu(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::tamDung();
        }
    } while (choice != 0);
}

inline void AdminMenu::themDichVu() {
    string ten = ui::nhapChuoi("Ten dich vu: ", false);
    double gia = ui::nhapSoThucDuong("Don gia (VND/thang): ");
    string ma = taoMa("DV", soTiepTheo(services, "DV"));

    Service s(ma, ten, gia);
    services.insertBack(s);

    cout << "Da them dich vu " << ma << ":" << endl;
    s.display();
    ui::tamDung();
}

inline void AdminMenu::suaDichVu() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma dich vu can sua: ", false));
    Service s;
    if (!services.findIf([&ma](const Service& x) { return x.getMaDichVu() == ma; }, s)) {
        cout << "Khong tim thay dich vu " << ma << endl;
        ui::tamDung();
        return;
    }
    s.display();
    cout << "(Nhan Enter de giu nguyen gia tri cu)" << endl;
    string ten = ui::nhapChuoi("Ten moi: ", true);
    double gia = ui::nhapTuyChonSoThucDuong("Don gia moi: ", s.getDonGia());

    services.updateIf(
        [&ma](const Service& x) { return x.getMaDichVu() == ma; },
        [&ten, gia](Service& x) {
            if (!ten.empty()) x.setTenDichVu(ten);
            x.setDonGia(gia);
        });

    cout << "Da cap nhat dich vu " << ma << endl;
    ui::tamDung();
}

inline void AdminMenu::xoaDichVu() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma dich vu can xoa: ", false));
    Service s;
    if (!services.findIf([&ma](const Service& x) { return x.getMaDichVu() == ma; }, s)) {
        cout << "Khong tim thay dich vu " << ma << endl;
        ui::tamDung();
        return;
    }
    s.display();
    if (ui::xacNhan("Ban co chac muon xoa dich vu " + ma + "?")) {
        services.removeIf([&ma](const Service& x) { return x.getMaDichVu() == ma; });
        cout << "Da xoa dich vu " << ma << endl;
    } else {
        cout << "Da huy." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::xemDichVu() const {
    cout << "------ DANH SACH DICH VU ------" << endl;
    if (services.isEmpty()) {
        cout << "(Chua co dich vu nao)" << endl;
    } else {
        services.traverseForward([](const Service& s) {
            cout << s.getMaDichVu() << " | ";
            s.display();
        });
    }
    ui::tamDung();
}

inline bool AdminMenu::chay() {
    if (!dangNhap()) {
        cout << "Dang nhap that bai. Thoat chuong trinh." << endl;
        return false;
    }

    int choice;
    do {
        hienThiMenuChinh();
        choice = ui::nhapSoNguyen("Chon: ");

        switch (choice) {
            case 1: menuPhong(); break;
            case 2: menuKhach(); break;
            case 3: menuHopDong(); break;
            case 4: menuHoaDon(); break;
            case 5: menuCheckin(); break;
            case 6: hoanTac(); break;
            case 7: menuDichVu(); break;
            case 0: break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                ui::tamDung();
        }
    } while (choice != 0);

    return true;
}

#endif

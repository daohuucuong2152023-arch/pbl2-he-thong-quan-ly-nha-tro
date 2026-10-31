#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <iostream>
#include <string>

#include "../models/Room.h"
#include "../models/Tenant.h"
#include "../models/Contract.h"
#include "../models/Invoice.h"
#include "../models/Service.h"
#include "../services/RoomService.h"
#include "../services/TenantService.h"
#include "../services/ContractService.h"
#include "../services/InvoiceService.h"
#include "../services/CheckinService.h"
#include "../services/DichVuService.h"
#include "../services/UndoService.h"
#include "InputHelper.h"

using namespace std;

class AdminMenu {
private:
    RoomService& roomService;
    TenantService& tenantService;
    ContractService& contractService;
    InvoiceService& invoiceService;
    CheckinService& checkinService;
    DichVuService& dichVuService;
    UndoService& undoService;

    bool dangNhap();
    void hienThiMenuChinh() const;
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
    AdminMenu(RoomService& rs,
              TenantService& ts,
              ContractService& cs,
              InvoiceService& is,
              CheckinService& cis,
              DichVuService& dvs,
              UndoService& us)
        : roomService(rs),
          tenantService(ts),
          contractService(cs),
          invoiceService(is),
          checkinService(cis),
          dichVuService(dvs),
          undoService(us) {}

    bool chay();
};

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
    roomService.demPhong(tongPhong, phongTrong);
    int khachDangO = tenantService.demKhachDangO();
    int hoaDonChuaThu = invoiceService.demHoaDonChuaThu();

    ui::xoaManHinh();
    cout << "========== QUAN LY NHA TRO ==========" << endl;
    cout << "Phong: " << tongPhong << " (trong: " << phongTrong << ")"
         << " | Khach dang o: " << khachDangO << endl;
    cout << "Cho check-in: " << checkinService.soLuongDangCho()
         << " | Hoa don chua thu: " << hoaDonChuaThu << endl;
    cout << "-------------------------------------" << endl;
    cout << "1. Quan ly phong" << endl;
    cout << "2. Quan ly khach thue" << endl;
    cout << "3. Quan ly hop dong" << endl;
    cout << "4. Quan ly hoa don" << endl;
    cout << "5. Check-in (hang doi)" << endl;
    cout << "6. Hoan tac thao tac gan nhat (" << undoService.kichThuoc() << ")" << endl;
    cout << "7. Quan ly dich vu" << endl;
    cout << "0. Luu va thoat" << endl;
    cout << "-------------------------------------" << endl;
}

inline void AdminMenu::hoanTac() {
    string thongBao;
    undoService.thucHienHoanTac(thongBao);
    cout << thongBao << endl;
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
    double dienTich = ui::nhapSoThucDuong("Dien tich (m2): ");
    double giaCoBan = ui::nhapSoThucDuong("Gia co ban (VND/thang): ");

    Room moi;
    string thongBaoLoi;
    if (!roomService.themPhong(ma, dienTich, giaCoBan, moi, thongBaoLoi)) {
        cout << thongBaoLoi << endl;
        ui::tamDung();
        return;
    }

    cout << "Da them phong:" << endl;
    moi.display();
    ui::tamDung();
}

inline void AdminMenu::suaPhong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma phong can sua: ", false));
    Room r;
    if (!roomService.timPhong(ma, r)) {
        cout << "Khong tim thay phong " << ma << endl;
        ui::tamDung();
        return;
    }
    r.display();
    cout << "(Nhan Enter de giu nguyen gia tri cu)" << endl;

    double dienTichMoi = ui::nhapTuyChonSoThucDuong("Dien tich moi: ", r.getDienTich());
    double giaCoBanMoi = ui::nhapTuyChonSoThucDuong("Gia co ban moi: ", r.getGiaCoBan());
    string trangThaiMoi = "";

    if (r.getTrangThai() == "Dang thue") {
        cout << "Phong dang co nguoi thue nen khong doi duoc trang thai." << endl;
    } else {
        cout << "Trang thai moi: 1. Trong  2. Dang sua chua" << endl;
        string tt = ui::nhapChuoi("Chon (Enter de giu nguyen): ", true);
        if (tt == "1") trangThaiMoi = "Trong";
        else if (tt == "2") trangThaiMoi = "Dang sua chua";
        else if (!tt.empty()) cout << "Lua chon khong hop le, giu nguyen trang thai." << endl;
    }

    Room phongCapNhat;
    string thongBaoLoi;
    if (!roomService.suaPhong(ma, dienTichMoi, giaCoBanMoi, trangThaiMoi, phongCapNhat, thongBaoLoi)) {
        cout << thongBaoLoi << endl;
    } else {
        cout << "Da cap nhat phong:" << endl;
        phongCapNhat.display();
    }
    ui::tamDung();
}

inline void AdminMenu::xoaPhong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma phong can xoa: ", false));
    Room r;
    if (!roomService.timPhong(ma, r)) {
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
        Room daXoa;
        string thongBaoLoi;
        if (roomService.xoaPhong(ma, daXoa, thongBaoLoi)) {
            cout << "Da xoa phong " << ma << endl;
        } else {
            cout << thongBaoLoi << endl;
        }
    } else {
        cout << "Da huy." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::timPhong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma phong can tim: ", false));
    Room r;
    if (!roomService.timPhong(ma, r)) {
        cout << "Khong tim thay phong " << ma << endl;
        ui::tamDung();
        return;
    }
    r.display();
    if (r.getTrangThai() == "Dang thue") {
        Tenant t;
        if (tenantService.timKhachDangOTheoPhong(ma, t)) {
            cout << "Nguoi dang thue:" << endl;
            t.display();
        }
    }
    ui::tamDung();
}

inline void AdminMenu::xemDanhSachPhong() const {
    cout << "------ DANH SACH PHONG ------" << endl;
    if (roomService.isEmpty()) {
        cout << "(Chua co phong nao)" << endl;
    } else {
        DoublyLinkedList<Room> ds = roomService.layTatCaPhong();
        ds.traverseForward([](const Room& r) { r.display(); });
    }
    ui::tamDung();
}

inline int AdminMenu::inPhongTrong() const {
    int dem = 0;
    cout << "------ PHONG TRONG ------" << endl;
    DoublyLinkedList<Room> ds = roomService.layPhongTrong();
    ds.traverseForward([&dem](const Room& r) {
        r.display();
        dem++;
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
        cout << "So khach dang cho: " << checkinService.soLuongDangCho() << endl;
        Tenant dauHang;
        if (checkinService.layKhachDauHang(dauHang)) {
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

inline void AdminMenu::xemHangDoi() {
    cout << "------ KHACH DANG CHO CHECK-IN ------" << endl;
    if (checkinService.soLuongDangCho() == 0) {
        cout << "(Khong co khach nao dang cho)" << endl;
    } else {
        int stt = 0;
        DoublyLinkedList<Tenant> ds = checkinService.layDanhSachHangDoi();
        ds.traverseForward([this, &stt](const Tenant& t) {
            cout << ++stt << ". ";
            inKhachCho(t);
        });
    }
    ui::tamDung();
}

inline void AdminMenu::dangKyCheckin() {
    string ten = ui::nhapChuoi("Ho ten: ", false);
    string cccd = ui::nhapChuSo("CCCD (12 so): ", 12, false);
    string sdt = ui::nhapChuSo("So dien thoai (10 so): ", 10, false);

    string maKhach;
    int viTri;
    string thongBaoLoi;
    if (!checkinService.dangKyCheckin(ten, cccd, sdt, maKhach, viTri, thongBaoLoi)) {
        cout << thongBaoLoi << endl;
        ui::tamDung();
        return;
    }

    cout << "Da them vao hang doi. Ma khach: " << maKhach << " (vi tri: " << viTri << ")" << endl;
    ui::tamDung();
}

inline void AdminMenu::xuLyCheckin() {
    Tenant t;
    if (!checkinService.layKhachDauHang(t)) {
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
    if (!roomService.timPhong(maPhong, r)) {
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

    Tenant tenantTao;
    Contract contractTao;
    string thongBaoLoi;
    if (!checkinService.xuLyCheckin(maPhong, ngay, thoiHan, coc, tenantTao, contractTao, thongBaoLoi)) {
        cout << thongBaoLoi << endl;
        ui::tamDung();
        return;
    }

    cout << "Check-in thanh cong!" << endl;
    tenantTao.display();
    contractTao.display();
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
    DoublyLinkedList<Tenant> ds = tenantService.layDanhSachKhach(chiDangO);
    if (ds.isEmpty()) {
        cout << "(Khong co khach nao)" << endl;
    } else {
        ds.traverseForward([](const Tenant& t) { t.display(); });
    }
    ui::tamDung();
}

inline void AdminMenu::timKhach() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma khach can tim: ", false));
    Tenant t;
    if (tenantService.timKhachTheoMa(ma, t)) {
        t.display();
    } else {
        cout << "Khong tim thay khach " << ma << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::suaKhach() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma khach can sua: ", false));
    Tenant t;
    if (!tenantService.timKhachTheoMa(ma, t)) {
        cout << "Khong tim thay khach " << ma << endl;
        ui::tamDung();
        return;
    }
    t.display();
    cout << "(Nhan Enter de giu nguyen gia tri cu)" << endl;

    string ten = ui::nhapChuoi("Ho ten moi: ", true);
    string cccd = ui::nhapChuSo("CCCD moi (12 so): ", 12, true);
    string sdt = ui::nhapChuSo("SDT moi (10 so): ", 10, true);

    string thongBaoLoi;
    if (tenantService.suaKhach(ma, ten, cccd, sdt, thongBaoLoi)) {
        cout << "Da cap nhat." << endl;
    } else {
        cout << thongBaoLoi << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::traPhong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma khach tra phong: ", false));
    Tenant t;
    if (!tenantService.timKhachTheoMa(ma, t)) {
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
    int chuaThu = invoiceService.demHoaDonChuaThuCuaPhong(phong);
    t.display();
    if (chuaThu > 0) {
        cout << "CANH BAO: phong " << phong << " con " << chuaThu << " hoa don chua thanh toan!" << endl;
    }

    if (!ui::xacNhan("Xac nhan tra phong " + phong + " cho khach " + t.getHoTen() + "?")) {
        cout << "Da huy." << endl;
        ui::tamDung();
        return;
    }

    string maPhongTra;
    double tienCocHoan = 0;
    int soHoaDon = 0;
    string thongBaoLoi;
    if (checkinService.traPhong(ma, maPhongTra, tienCocHoan, soHoaDon, thongBaoLoi)) {
        cout << "Tra phong thanh cong. Phong " << maPhongTra << " da tro ve trang thai Trong." << endl;
        cout << "Tien coc can hoan lai: " << tienCocHoan << " VND" << endl;
    } else {
        cout << thongBaoLoi << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::xoaKhach() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma khach can xoa: ", false));
    Tenant t;
    if (!tenantService.timKhachTheoMa(ma, t)) {
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
    bool coHopDong = contractService.timHopDongCuaKhach(ma, hd);
    if (coHopDong) {
        cout << "Khach con hop dong " << hd.getMaHopDong() << ", hay xoa hop dong truoc." << endl;
        ui::tamDung();
        return;
    }
    if (ui::xacNhan("Ban co chac muon xoa khach " + ma + "?")) {
        Tenant khachDaXoa;
        string thongBaoLoi;
        if (tenantService.xoaKhach(ma, coHopDong, khachDaXoa, thongBaoLoi)) {
            cout << "Da xoa khach " << ma << endl;
        } else {
            cout << thongBaoLoi << endl;
        }
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
    DoublyLinkedList<Contract> ds = contractService.layDanhSachHopDong(chiConHieuLuc);
    if (ds.isEmpty()) {
        cout << "(Khong co hop dong nao)" << endl;
    } else {
        ds.traverseForward([](const Contract& c) { c.display(); });
    }
    ui::tamDung();
}

inline void AdminMenu::timHopDong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hop dong can tim: ", false));
    Contract c;
    if (contractService.timHopDong(ma, c)) {
        c.display();
    } else {
        cout << "Khong tim thay hop dong " << ma << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::giaHanHopDong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hop dong can gia han: ", false));
    Contract c;
    if (!contractService.timHopDong(ma, c)) {
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
    int thoiHanMoi = 0;
    string thongBaoLoi;
    if (contractService.giaHanHopDong(ma, them, thoiHanMoi, thongBaoLoi)) {
        cout << "Da gia han. Thoi han moi: " << thoiHanMoi << " thang." << endl;
    } else {
        cout << thongBaoLoi << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::xoaHopDong() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hop dong can xoa: ", false));
    Contract c;
    if (!contractService.timHopDong(ma, c)) {
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
        Contract hopDongDaXoa;
        string thongBaoLoi;
        if (contractService.xoaHopDong(ma, hopDongDaXoa, thongBaoLoi)) {
            cout << "Da xoa hop dong " << ma << endl;
        } else {
            cout << thongBaoLoi << endl;
        }
    } else {
        cout << "Da huy." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::suaTienCoc() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hop dong can sua tien coc: ", false));
    Contract c;
    if (!contractService.timHopDong(ma, c)) {
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

    string thongBaoLoi;
    if (contractService.suaTienCoc(ma, cocMoi, thongBaoLoi)) {
        cout << "Da cap nhat tien coc: " << cocMoi << " VND (hop dong va khach thue)." << endl;
    } else {
        cout << thongBaoLoi << endl;
    }
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
    if (!roomService.timPhong(maPhong, r)) {
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
    if (invoiceService.kiemTraDaCoHoaDonThang(maPhong, thang)) {
        cout << "Phong " << maPhong << " da co hoa don thang " << thang << "!" << endl;
        ui::tamDung();
        return;
    }

    double dienCu = 0, nuocCu = 0;
    bool coHoaDonCu = false;
    string thangCu;
    invoiceService.timChiSoCuMoiNhat(maPhong, coHoaDonCu, thangCu, dienCu, nuocCu);

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

    Invoice hd;
    string thongBaoLoi;
    if (!invoiceService.lapHoaDon(maPhong, thang, dienCu, dienMoi, nuocCu, nuocMoi, giaDien, giaNuoc, r.getGiaCoBan(), hd, thongBaoLoi)) {
        cout << thongBaoLoi << endl;
        ui::tamDung();
        return;
    }

    cout << "Da lap hoa don:" << endl;
    hd.display();
    ui::tamDung();
}

inline void AdminMenu::xemHoaDon(bool chiChuaThu) const {
    cout << "------ " << (chiChuaThu ? "HOA DON CHUA THANH TOAN" : "TAT CA HOA DON") << " ------" << endl;
    double tong = 0;
    DoublyLinkedList<Invoice> ds = invoiceService.layDanhSachHoaDon(chiChuaThu, tong);
    if (ds.isEmpty()) {
        cout << "(Khong co hoa don nao)" << endl;
    } else {
        ds.traverseForward([](const Invoice& h) { h.display(); });
        cout << "Tong tien chua thu: " << tong << " VND" << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::xemHoaDonTheoPhong() const {
    string maPhong = ui::vietHoa(ui::nhapChuoi("Nhap ma phong: ", false));
    cout << "------ HOA DON PHONG " << maPhong << " ------" << endl;
    DoublyLinkedList<Invoice> ds = invoiceService.layHoaDonTheoPhong(maPhong);
    if (ds.isEmpty()) {
        cout << "(Phong nay chua co hoa don nao)" << endl;
    } else {
        ds.traverseForward([](const Invoice& h) { h.display(); });
    }
    ui::tamDung();
}

inline void AdminMenu::thanhToanHoaDon() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hoa don: ", false));
    Invoice h;
    if (!invoiceService.timHoaDon(ma, h)) {
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
        Invoice hdDaThanhToan;
        string thongBaoLoi;
        if (invoiceService.thanhToanHoaDon(ma, hdDaThanhToan, thongBaoLoi)) {
            cout << "Da ghi nhan thanh toan." << endl;
        } else {
            cout << thongBaoLoi << endl;
        }
    } else {
        cout << "Da huy." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::xoaHoaDon() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hoa don can xoa: ", false));
    Invoice h;
    if (!invoiceService.timHoaDon(ma, h)) {
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
        Invoice hdDaXoa;
        string thongBaoLoi;
        if (invoiceService.xoaHoaDon(ma, hdDaXoa, thongBaoLoi)) {
            cout << "Da xoa hoa don " << ma << endl;
        } else {
            cout << thongBaoLoi << endl;
        }
    } else {
        cout << "Da huy." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::suaChiSoHoaDon() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma hoa don can sua chi so: ", false));
    Invoice h;
    if (!invoiceService.timHoaDon(ma, h)) {
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

    cout << "(Nhan Enter de giu nguyen gia tri hien tai)" << endl;
    double dienMoi = ui::nhapTuyChonSoThuc("Chi so dien moi (hien tai " + to_string((long long)h.getChiSoDienMoi()) +
                                            ", cu " + to_string((long long)h.getChiSoDienCu()) + "): ",
                                            h.getChiSoDienMoi(), h.getChiSoDienCu());
    double nuocMoi = ui::nhapTuyChonSoThuc("Chi so nuoc moi (hien tai " + to_string((long long)h.getChiSoNuocMoi()) +
                                            ", cu " + to_string((long long)h.getChiSoNuocCu()) + "): ",
                                            h.getChiSoNuocMoi(), h.getChiSoNuocCu());

    Invoice hdCapNhat;
    string thongBaoLoi;
    if (!invoiceService.suaChiSoHoaDon(ma, dienMoi, nuocMoi, hdCapNhat, thongBaoLoi)) {
        cout << thongBaoLoi << endl;
    } else {
        cout << "Da cap nhat:" << endl;
        hdCapNhat.display();
    }
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

    Service s;
    dichVuService.themDichVu(ten, gia, s);

    cout << "Da them dich vu " << s.getMaDichVu() << ":" << endl;
    s.display();
    ui::tamDung();
}

inline void AdminMenu::suaDichVu() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma dich vu can sua: ", false));
    Service s;
    if (!dichVuService.timDichVu(ma, s)) {
        cout << "Khong tim thay dich vu " << ma << endl;
        ui::tamDung();
        return;
    }
    s.display();
    cout << "(Nhan Enter de giu nguyen gia tri cu)" << endl;
    string ten = ui::nhapChuoi("Ten moi: ", true);
    double gia = ui::nhapTuyChonSoThucDuong("Don gia moi: ", s.getDonGia());

    string thongBaoLoi;
    if (dichVuService.suaDichVu(ma, ten, gia, thongBaoLoi)) {
        cout << "Da cap nhat dich vu " << ma << endl;
    } else {
        cout << thongBaoLoi << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::xoaDichVu() {
    string ma = ui::vietHoa(ui::nhapChuoi("Nhap ma dich vu can xoa: ", false));
    Service s;
    if (!dichVuService.timDichVu(ma, s)) {
        cout << "Khong tim thay dich vu " << ma << endl;
        ui::tamDung();
        return;
    }
    s.display();
    if (ui::xacNhan("Ban co chac muon xoa dich vu " + ma + "?")) {
        string thongBaoLoi;
        if (dichVuService.xoaDichVu(ma, thongBaoLoi)) {
            cout << "Da xoa dich vu " << ma << endl;
        } else {
            cout << thongBaoLoi << endl;
        }
    } else {
        cout << "Da huy." << endl;
    }
    ui::tamDung();
}

inline void AdminMenu::xemDichVu() const {
    cout << "------ DANH SACH DICH VU ------" << endl;
    if (dichVuService.isEmpty()) {
        cout << "(Chua co dich vu nao)" << endl;
    } else {
        DoublyLinkedList<Service> ds = dichVuService.layDanhSachDichVu();
        ds.traverseForward([](const Service& s) {
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

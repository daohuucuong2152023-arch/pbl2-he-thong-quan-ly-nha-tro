#ifndef CHECKINSERVICE_H
#define CHECKINSERVICE_H

#include <string>
#include <cctype>
#include "../data_structures/Queue.h"
#include "../data_structures/BST.h"
#include "../data_structures/DoublyLinkedList.h"
#include "../models/Tenant.h"
#include "../models/Room.h"
#include "../models/Contract.h"
#include "../models/Invoice.h"
#include "UndoService.h"

using namespace std;

class CheckinService {
private:
    Queue<Tenant>& checkinQueue;
    BST<Room>& rooms;
    DoublyLinkedList<Tenant>& tenants;
    DoublyLinkedList<Contract>& contracts;
    DoublyLinkedList<Invoice>& invoices;
    UndoService& undoService;
    int tenantCounter;

    template <class Func>
    void duyetHangDoi(Func visit) {
        int n = checkinQueue.size();
        for (int i = 0; i < n; i++) {
            Tenant t;
            checkinQueue.dequeue(t);
            visit(t);
            checkinQueue.enqueue(t);
        }
    }

    void khoiTaoBoDemKhach() {
        tenantCounter = 0;
        tenants.traverseForward([this](const Tenant& x) {
            string id = x.getId();
            if (id.size() > 2 && id.compare(0, 2, "TN") == 0) {
                string rest = id.substr(2);
                bool allDigits = true;
                for (char ch : rest) if (!isdigit(ch)) allDigits = false;
                if (allDigits && rest.size() <= 9 && !rest.empty()) {
                    int n = stoi(rest);
                    if (n > tenantCounter) tenantCounter = n;
                }
            }
        });

        duyetHangDoi([this](const Tenant& t) {
            string id = t.getMaNguoiThue();
            if (id.size() > 2 && id.compare(0, 2, "TN") == 0) {
                string rest = id.substr(2);
                bool allDigits = true;
                for (char ch : rest) if (!isdigit(ch)) allDigits = false;
                if (allDigits && rest.size() <= 9 && !rest.empty()) {
                    int v = stoi(rest);
                    if (v > tenantCounter) tenantCounter = v;
                }
            }
        });
    }

    string taoMaKhachMoi() {
        tenantCounter++;
        string s = to_string(tenantCounter);
        while (s.size() < 3) s = "0" + s;
        return "TN" + s;
    }

    string taoMaHopDongMoi() {
        int maxNo = 0;
        contracts.traverseForward([&maxNo](const Contract& x) {
            string id = x.getId();
            if (id.size() > 2 && id.compare(0, 2, "HD") == 0) {
                string rest = id.substr(2);
                bool allDigits = true;
                for (char ch : rest) if (!isdigit(ch)) allDigits = false;
                if (allDigits && rest.size() <= 9 && !rest.empty()) {
                    int n = stoi(rest);
                    if (n > maxNo) maxNo = n;
                }
            }
        });
        int num = maxNo + 1;
        string s = to_string(num);
        while (s.size() < 3) s = "0" + s;
        return "HD" + s;
    }

public:
    CheckinService(Queue<Tenant>& q, BST<Room>& r, DoublyLinkedList<Tenant>& t,
                   DoublyLinkedList<Contract>& c, DoublyLinkedList<Invoice>& i, UndoService& u)
        : checkinQueue(q), rooms(r), tenants(t), contracts(c), invoices(i), undoService(u), tenantCounter(0) {
        khoiTaoBoDemKhach();
    }

    int soLuongDangCho() const {
        return checkinQueue.size();
    }

    bool layKhachDauHang(Tenant& t) const {
        return checkinQueue.front(t);
    }

    bool cccdDangCho(const string& cccd) {
        bool co = false;
        duyetHangDoi([&cccd, &co](const Tenant& t) {
            if (t.getCccd() == cccd) co = true;
        });
        return co;
    }

    DoublyLinkedList<Tenant> layDanhSachHangDoi() {
        DoublyLinkedList<Tenant> ds;
        duyetHangDoi([&ds](const Tenant& t) {
            ds.insertBack(t);
        });
        return ds;
    }

    bool dangKyCheckin(const string& ten, const string& cccd, const string& sdt,
                      string& maKhachTao, int& viTriHangDoi, string& thongBaoLoi) {
        Tenant daO;
        bool trung = tenants.findIf([&cccd](const Tenant& x) {
            return x.getCccd() == cccd && x.getTrangThai() == "Dang o";
        }, daO);
        if (trung) {
            thongBaoLoi = "CCCD nay dang thue phong " + daO.getMaPhong() + " (khach " + daO.getMaNguoiThue() + ")!";
            return false;
        }

        if (cccdDangCho(cccd)) {
            thongBaoLoi = "CCCD nay da co trong hang doi!";
            return false;
        }

        maKhachTao = taoMaKhachMoi();
        Tenant t(maKhachTao, ten, cccd, sdt, "", 0, "");
        checkinQueue.enqueue(t);
        viTriHangDoi = checkinQueue.size();
        return true;
    }

    bool xuLyCheckin(const string& maPhong, const string& ngay, int thoiHan, double coc,
                     Tenant& tenantTao, Contract& contractTao, string& thongBaoLoi) {
        Tenant t;
        if (!checkinQueue.front(t)) {
            thongBaoLoi = "Khong co khach nao dang cho.";
            return false;
        }

        Room r;
        if (!rooms.search(maPhong, r)) {
            thongBaoLoi = "Khong tim thay phong " + maPhong;
            return false;
        }
        if (!r.isTrong()) {
            thongBaoLoi = "Phong " + maPhong + " khong trong (" + r.getTrangThai() + ")!";
            return false;
        }

        tenantTao = Tenant(t.getMaNguoiThue(), t.getHoTen(), t.getCccd(), t.getSoDienThoai(),
                           maPhong, coc, ngay);
        string maHD = taoMaHopDongMoi();
        contractTao = Contract(maHD, maPhong, tenantTao.getMaNguoiThue(), ngay, thoiHan, coc);

        tenants.insertBack(tenantTao);
        contracts.insertBack(contractTao);
        r.setTrangThai("Dang thue");
        rooms.insert(maPhong, r);

        Tenant boQua;
        checkinQueue.dequeue(boQua);
        undoService.xoaLichSu();

        return true;
    }

    bool traPhong(const string& maKhach, string& maPhongTra, double& tienCocHoan, int& soHoaDonChuaThu, string& thongBaoLoi) {
        Tenant t;
        if (!tenants.findIf([&maKhach](const Tenant& x) { return x.getMaNguoiThue() == maKhach; }, t)) {
            thongBaoLoi = "Khong tim thay khach " + maKhach;
            return false;
        }
        if (t.getTrangThai() != "Dang o") {
            thongBaoLoi = "Khach nay da tra phong roi.";
            return false;
        }

        maPhongTra = t.getMaPhong();
        tienCocHoan = t.getTienCoc();

        soHoaDonChuaThu = 0;
        invoices.traverseForward([&maPhongTra, &soHoaDonChuaThu](const Invoice& h) {
            if (h.getMaPhong() == maPhongTra && !h.isDaThanhToan()) soHoaDonChuaThu++;
        });

        tenants.updateIf(
            [&maKhach](const Tenant& x) { return x.getMaNguoiThue() == maKhach; },
            [](Tenant& x) { x.checkOut(); });

        contracts.updateIf(
            [&maKhach](const Contract& c) { return c.getMaNguoiThue() == maKhach && c.isConHieuLuc(); },
            [](Contract& c) { c.ketThuc(); });

        Room r;
        if (rooms.search(maPhongTra, r)) {
            r.setTrangThai("Trong");
            rooms.insert(maPhongTra, r);
        }

        undoService.xoaLichSu();
        return true;
    }
};

#endif

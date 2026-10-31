#ifndef TENANTSERVICE_H
#define TENANTSERVICE_H

#include <string>
#include "../data_structures/DoublyLinkedList.h"
#include "../models/Tenant.h"
#include "UndoService.h"

using namespace std;

class TenantService {
private:
    DoublyLinkedList<Tenant>& tenants;
    UndoService& undoService;

public:
    TenantService(DoublyLinkedList<Tenant>& t, UndoService& u) : tenants(t), undoService(u) {}

    DoublyLinkedList<Tenant> layDanhSachKhach(bool chiDangO) const {
        DoublyLinkedList<Tenant> ds;
        tenants.traverseForward([chiDangO, &ds](const Tenant& t) {
            if (!chiDangO || t.getTrangThai() == "Dang o") {
                ds.insertBack(t);
            }
        });
        return ds;
    }

    int demKhachDangO() const {
        int count = 0;
        tenants.traverseForward([&count](const Tenant& t) {
            if (t.getTrangThai() == "Dang o") count++;
        });
        return count;
    }

    bool timKhachTheoMa(const string& ma, Tenant& result) const {
        return tenants.findIf([&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; }, result);
    }

    bool timKhachDangOTheoPhong(const string& maPhong, Tenant& result) const {
        return tenants.findIf([&maPhong](const Tenant& x) {
            return x.getMaPhong() == maPhong && x.getTrangThai() == "Dang o";
        }, result);
    }

    bool kiemTraCccdDangO(const string& cccd, Tenant& tenantTrung) const {
        return tenants.findIf([&cccd](const Tenant& x) {
            return x.getCccd() == cccd && x.getTrangThai() == "Dang o";
        }, tenantTrung);
    }

    bool suaKhach(const string& ma, const string& tenMoi, const string& cccdMoi, const string& sdtMoi, string& thongBaoLoi) {
        Tenant t;
        if (!timKhachTheoMa(ma, t)) {
            thongBaoLoi = "Khong tim thay khach " + ma;
            return false;
        }

        tenants.updateIf(
            [&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; },
            [&tenMoi, &cccdMoi, &sdtMoi](Tenant& x) {
                if (!tenMoi.empty()) x.setHoTen(tenMoi);
                if (!cccdMoi.empty()) x.setCccd(cccdMoi);
                if (!sdtMoi.empty()) x.setSoDienThoai(sdtMoi);
            });
        return true;
    }

    bool xoaKhach(const string& ma, bool coHopDong, Tenant& khachDaXoa, string& thongBaoLoi) {
        Tenant t;
        if (!timKhachTheoMa(ma, t)) {
            thongBaoLoi = "Khong tim thay khach " + ma;
            return false;
        }
        if (t.getTrangThai() == "Dang o") {
            thongBaoLoi = "Khach dang o, hay lam thu tuc tra phong truoc.";
            return false;
        }
        if (coHopDong) {
            thongBaoLoi = "Khach con hop dong, hay xoa hop dong truoc.";
            return false;
        }

        ThaoTacHoanTac a("XOA_KHACH");
        a.khach = t;
        undoService.ghiNhan(a);

        tenants.removeIf([&ma](const Tenant& x) { return x.getMaNguoiThue() == ma; });
        khachDaXoa = t;
        return true;
    }
};

#endif

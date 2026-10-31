#ifndef UNDOSERVICE_H
#define UNDOSERVICE_H

#include <string>
#include "../data_structures/Stack.h"
#include "../data_structures/BST.h"
#include "../data_structures/DoublyLinkedList.h"
#include "../models/Room.h"
#include "../models/Tenant.h"
#include "../models/Contract.h"
#include "../models/Invoice.h"
#include "../ui/UndoAction.h"

using namespace std;

class UndoService {
private:
    Stack<ThaoTacHoanTac> nganXepHoanTac;
    BST<Room>& rooms;
    DoublyLinkedList<Tenant>& tenants;
    DoublyLinkedList<Contract>& contracts;
    DoublyLinkedList<Invoice>& invoices;

public:
    UndoService(BST<Room>& r, DoublyLinkedList<Tenant>& t, DoublyLinkedList<Contract>& c, DoublyLinkedList<Invoice>& i)
        : rooms(r), tenants(t), contracts(c), invoices(i) {}

    void ghiNhan(const ThaoTacHoanTac& action) {
        nganXepHoanTac.push(action);
    }

    void xoaLichSu() {
        while (!nganXepHoanTac.isEmpty()) nganXepHoanTac.pop();
    }

    int kichThuoc() const {
        return nganXepHoanTac.size();
    }

    bool coTheHoanTac() const {
        return !nganXepHoanTac.isEmpty();
    }

    bool thucHienHoanTac(string& thongBao) {
        ThaoTacHoanTac a;
        if (!nganXepHoanTac.pop(a)) {
            thongBao = "Khong co thao tac nao de hoan tac.";
            return false;
        }

        if (a.loai == "THEM_PHONG") {
            rooms.remove(a.phongSau.getMaPhong());
            thongBao = "Da hoan tac: them phong " + a.phongSau.getMaPhong() + " (phong da bi go bo).";
        } else if (a.loai == "SUA_PHONG") {
            rooms.insert(a.phongTruoc.getMaPhong(), a.phongTruoc);
            thongBao = "Da hoan tac: sua phong " + a.phongTruoc.getMaPhong() + " (tra ve gia tri cu).";
        } else if (a.loai == "XOA_PHONG") {
            rooms.insert(a.phongTruoc.getMaPhong(), a.phongTruoc);
            thongBao = "Da hoan tac: xoa phong " + a.phongTruoc.getMaPhong() + " (phong da duoc khoi phuc).";
        } else if (a.loai == "XOA_KHACH") {
            tenants.insertBack(a.khach);
            thongBao = "Da hoan tac: xoa khach " + a.khach.getMaNguoiThue() + " (khach da duoc khoi phuc).";
        } else if (a.loai == "XOA_HOPDONG") {
            contracts.insertBack(a.hopDong);
            thongBao = "Da hoan tac: xoa hop dong " + a.hopDong.getMaHopDong() + " (hop dong da duoc khoi phuc).";
        } else if (a.loai == "XOA_HOADON") {
            invoices.insertBack(a.hoaDon);
            thongBao = "Da hoan tac: xoa hoa don " + a.hoaDon.getMaHoaDon() + " (hoa don da duoc khoi phuc).";
        } else if (a.loai == "SUA_HOADON") {
            string maHD = a.hoaDon.getMaHoaDon();
            double dienCu = a.hoaDon.getChiSoDienMoi();
            double nuocCu = a.hoaDon.getChiSoNuocMoi();
            invoices.updateIf(
                [&maHD](const Invoice& x) { return x.getMaHoaDon() == maHD; },
                [dienCu, nuocCu](Invoice& x) { x.capNhatChiSoMoi(dienCu, nuocCu); });
            thongBao = "Da hoan tac: sua chi so hoa don " + maHD + " (tra ve chi so cu).";
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
            thongBao = "Da hoan tac: sua tien coc hop dong " + maHD + " (tra ve tien coc cu).";
        }
        return true;
    }
};

#endif

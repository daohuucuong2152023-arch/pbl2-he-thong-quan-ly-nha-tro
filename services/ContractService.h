#ifndef CONTRACTSERVICE_H
#define CONTRACTSERVICE_H

#include <string>
#include <cctype>
#include "../data_structures/DoublyLinkedList.h"
#include "../models/Contract.h"
#include "../models/Tenant.h"
#include "UndoService.h"

using namespace std;

class ContractService {
private:
    DoublyLinkedList<Contract>& contracts;
    DoublyLinkedList<Tenant>& tenants;
    UndoService& undoService;

public:
    ContractService(DoublyLinkedList<Contract>& c, DoublyLinkedList<Tenant>& t, UndoService& u)
        : contracts(c), tenants(t), undoService(u) {}

    DoublyLinkedList<Contract> layDanhSachHopDong(bool chiConHieuLuc) const {
        DoublyLinkedList<Contract> ds;
        contracts.traverseForward([chiConHieuLuc, &ds](const Contract& c) {
            if (!chiConHieuLuc || c.isConHieuLuc()) {
                ds.insertBack(c);
            }
        });
        return ds;
    }

    bool timHopDong(const string& ma, Contract& result) const {
        return contracts.findIf([&ma](const Contract& x) { return x.getMaHopDong() == ma; }, result);
    }

    bool timHopDongCuaKhach(const string& maKhach, Contract& result) const {
        return contracts.findIf([&maKhach](const Contract& c) { return c.getMaNguoiThue() == maKhach; }, result);
    }

    bool giaHanHopDong(const string& ma, int soThangThem, int& thoiHanMoi, string& thongBaoLoi) {
        Contract c;
        if (!timHopDong(ma, c)) {
            thongBaoLoi = "Khong tim thay hop dong " + ma;
            return false;
        }
        if (!c.isConHieuLuc()) {
            thongBaoLoi = "Hop dong da ket thuc, khong gia han duoc.";
            return false;
        }

        thoiHanMoi = c.getThoiHan() + soThangThem;
        contracts.updateIf(
            [&ma](const Contract& x) { return x.getMaHopDong() == ma; },
            [thoiHanMoi](Contract& x) { x.setThoiHan(thoiHanMoi); });
        return true;
    }

    bool xoaHopDong(const string& ma, Contract& hopDongDaXoa, string& thongBaoLoi) {
        Contract c;
        if (!timHopDong(ma, c)) {
            thongBaoLoi = "Khong tim thay hop dong " + ma;
            return false;
        }
        if (c.isConHieuLuc()) {
            thongBaoLoi = "Hop dong con hieu luc, hay lam thu tuc tra phong truoc.";
            return false;
        }

        ThaoTacHoanTac a("XOA_HOPDONG");
        a.hopDong = c;
        undoService.ghiNhan(a);

        contracts.removeIf([&ma](const Contract& x) { return x.getMaHopDong() == ma; });
        hopDongDaXoa = c;
        return true;
    }

    bool suaTienCoc(const string& ma, double cocMoi, string& thongBaoLoi) {
        Contract c;
        if (!timHopDong(ma, c)) {
            thongBaoLoi = "Khong tim thay hop dong " + ma;
            return false;
        }
        if (!c.isConHieuLuc()) {
            thongBaoLoi = "Hop dong da ket thuc, khong sua duoc.";
            return false;
        }

        string maKhach = c.getMaNguoiThue();

        ThaoTacHoanTac a("SUA_COC");
        a.hopDong = c;
        tenants.findIf([&maKhach](const Tenant& x) { return x.getMaNguoiThue() == maKhach; }, a.khach);
        undoService.ghiNhan(a);

        contracts.updateIf(
            [&ma](const Contract& x) { return x.getMaHopDong() == ma; },
            [cocMoi](Contract& x) { x.setTienCoc(cocMoi); });
        tenants.updateIf(
            [&maKhach](const Tenant& x) { return x.getMaNguoiThue() == maKhach; },
            [cocMoi](Tenant& x) { x.setTienCoc(cocMoi); });
        return true;
    }

    string taoMaHopDongMoi() const {
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
};

#endif

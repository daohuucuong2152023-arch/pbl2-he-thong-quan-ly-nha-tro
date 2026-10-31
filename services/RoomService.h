#ifndef ROOMSERVICE_H
#define ROOMSERVICE_H

#include <string>
#include "../data_structures/BST.h"
#include "../data_structures/DoublyLinkedList.h"
#include "../models/Room.h"
#include "UndoService.h"

using namespace std;

class RoomService {
private:
    BST<Room>& rooms;
    UndoService& undoService;

public:
    RoomService(BST<Room>& r, UndoService& u) : rooms(r), undoService(u) {}

    bool themPhong(const string& ma, double dienTich, double giaCoBan, Room& phongMoi, string& thongBaoLoi) {
        Room tonTai;
        if (rooms.search(ma, tonTai)) {
            thongBaoLoi = "Ma phong " + ma + " da ton tai!";
            return false;
        }
        phongMoi = Room(ma, dienTich, giaCoBan, "Trong");
        rooms.insert(ma, phongMoi);

        ThaoTacHoanTac a("THEM_PHONG");
        a.phongSau = phongMoi;
        undoService.ghiNhan(a);
        return true;
    }

    bool suaPhong(const string& ma, double dienTichMoi, double giaCoBanMoi, const string& trangThaiMoi, Room& phongCapNhat, string& thongBaoLoi) {
        Room r;
        if (!rooms.search(ma, r)) {
            thongBaoLoi = "Khong tim thay phong " + ma;
            return false;
        }
        Room truoc = r;
        r.setDienTich(dienTichMoi);
        r.setGiaCoBan(giaCoBanMoi);
        if (!trangThaiMoi.empty()) {
            r.setTrangThai(trangThaiMoi);
        }
        rooms.insert(ma, r);

        ThaoTacHoanTac a("SUA_PHONG");
        a.phongTruoc = truoc;
        a.phongSau = r;
        undoService.ghiNhan(a);

        phongCapNhat = r;
        return true;
    }

    bool xoaPhong(const string& ma, Room& phongDaXoa, string& thongBaoLoi) {
        Room r;
        if (!rooms.search(ma, r)) {
            thongBaoLoi = "Khong tim thay phong " + ma;
            return false;
        }
        if (r.getTrangThai() == "Dang thue") {
            thongBaoLoi = "Phong dang co nguoi thue, khong the xoa. Hay lam thu tuc tra phong truoc.";
            return false;
        }
        rooms.remove(ma);
        ThaoTacHoanTac a("XOA_PHONG");
        a.phongTruoc = r;
        undoService.ghiNhan(a);

        phongDaXoa = r;
        return true;
    }

    bool timPhong(const string& ma, Room& result) const {
        return rooms.search(ma, result);
    }

    bool capNhatTrangThai(const string& maPhong, const string& trangThai) {
        Room r;
        if (!rooms.search(maPhong, r)) return false;
        r.setTrangThai(trangThai);
        rooms.insert(maPhong, r);
        return true;
    }

    void demPhong(int& tongPhong, int& phongTrong) const {
        tongPhong = 0;
        phongTrong = 0;
        rooms.inorderTraverse([&tongPhong, &phongTrong](const Room& r) {
            tongPhong++;
            if (r.isTrong()) phongTrong++;
        });
    }

    DoublyLinkedList<Room> layTatCaPhong() const {
        DoublyLinkedList<Room> ds;
        rooms.inorderTraverse([&ds](const Room& r) {
            ds.insertBack(r);
        });
        return ds;
    }

    DoublyLinkedList<Room> layPhongTrong() const {
        DoublyLinkedList<Room> ds;
        rooms.inorderTraverse([&ds](const Room& r) {
            if (r.isTrong()) {
                ds.insertBack(r);
            }
        });
        return ds;
    }

    bool isEmpty() const {
        return rooms.isEmpty();
    }
};

#endif

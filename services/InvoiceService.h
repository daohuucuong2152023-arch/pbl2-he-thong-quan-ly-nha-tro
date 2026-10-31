#ifndef INVOICESERVICE_H
#define INVOICESERVICE_H

#include <string>
#include <cctype>
#include "../data_structures/DoublyLinkedList.h"
#include "../models/Invoice.h"
#include "UndoService.h"

using namespace std;

class InvoiceService {
private:
    DoublyLinkedList<Invoice>& invoices;
    UndoService& undoService;

public:
    InvoiceService(DoublyLinkedList<Invoice>& i, UndoService& u) : invoices(i), undoService(u) {}

    int demHoaDonChuaThu() const {
        int count = 0;
        invoices.traverseForward([&count](const Invoice& h) {
            if (!h.isDaThanhToan()) count++;
        });
        return count;
    }

    int demHoaDonChuaThuCuaPhong(const string& maPhong) const {
        int count = 0;
        invoices.traverseForward([&maPhong, &count](const Invoice& h) {
            if (h.getMaPhong() == maPhong && !h.isDaThanhToan()) count++;
        });
        return count;
    }

    bool kiemTraDaCoHoaDonThang(const string& maPhong, const string& thang) const {
        bool trung = false;
        invoices.traverseForward([&maPhong, &thang, &trung](const Invoice& h) {
            if (h.getMaPhong() == maPhong && h.getThangNam() == thang) trung = true;
        });
        return trung;
    }

    void timChiSoCuMoiNhat(const string& maPhong, bool& coHoaDonCu, string& thangCu, double& dienCu, double& nuocCu) const {
        coHoaDonCu = false;
        invoices.traverseForward([&](const Invoice& h) {
            if (h.getMaPhong() == maPhong && (!coHoaDonCu || h.getThangNam() > thangCu)) {
                coHoaDonCu = true;
                thangCu = h.getThangNam();
                dienCu = h.getChiSoDienMoi();
                nuocCu = h.getChiSoNuocMoi();
            }
        });
    }

    string taoMaHoaDonMoi() const {
        int maxNo = 0;
        invoices.traverseForward([&maxNo](const Invoice& x) {
            string id = x.getId();
            if (id.size() > 3 && id.compare(0, 3, "HDN") == 0) {
                string rest = id.substr(3);
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
        return "HDN" + s;
    }

    bool lapHoaDon(const string& maPhong, const string& thang,
                   double dienCu, double dienMoi,
                   double nuocCu, double nuocMoi,
                   double giaDien, double giaNuoc, double giaPhong,
                   Invoice& hoaDonMoi, string& thongBaoLoi) {
        if (kiemTraDaCoHoaDonThang(maPhong, thang)) {
            thongBaoLoi = "Phong " + maPhong + " da co hoa don thang " + thang + "!";
            return false;
        }

        string maHD = taoMaHoaDonMoi();
        hoaDonMoi = Invoice(maHD, maPhong, thang, dienCu, dienMoi, nuocCu, nuocMoi, giaDien, giaNuoc, giaPhong);
        invoices.insertBack(hoaDonMoi);
        undoService.xoaLichSu();
        return true;
    }

    DoublyLinkedList<Invoice> layDanhSachHoaDon(bool chiChuaThu, double& tongTienChuaThu) const {
        DoublyLinkedList<Invoice> ds;
        tongTienChuaThu = 0;
        invoices.traverseForward([chiChuaThu, &ds, &tongTienChuaThu](const Invoice& h) {
            if (!chiChuaThu || !h.isDaThanhToan()) {
                ds.insertBack(h);
                if (!h.isDaThanhToan()) tongTienChuaThu += h.tinhTongTien();
            }
        });
        return ds;
    }

    DoublyLinkedList<Invoice> layHoaDonTheoPhong(const string& maPhong) const {
        DoublyLinkedList<Invoice> ds;
        invoices.traverseForward([&maPhong, &ds](const Invoice& h) {
            if (h.getMaPhong() == maPhong) {
                ds.insertBack(h);
            }
        });
        return ds;
    }

    bool timHoaDon(const string& maHD, Invoice& result) const {
        return invoices.findIf([&maHD](const Invoice& x) { return x.getMaHoaDon() == maHD; }, result);
    }

    bool thanhToanHoaDon(const string& maHD, Invoice& hoaDonDaThanhToan, string& thongBaoLoi) {
        Invoice h;
        if (!timHoaDon(maHD, h)) {
            thongBaoLoi = "Khong tim thay hoa don " + maHD;
            return false;
        }
        if (h.isDaThanhToan()) {
            thongBaoLoi = "Hoa don nay da thanh toan roi.";
            return false;
        }

        invoices.updateIf(
            [&maHD](const Invoice& x) { return x.getMaHoaDon() == maHD; },
            [](Invoice& x) { x.danhDauDaThanhToan(); });
        undoService.xoaLichSu();

        timHoaDon(maHD, hoaDonDaThanhToan);
        return true;
    }

    bool xoaHoaDon(const string& maHD, Invoice& hoaDonDaXoa, string& thongBaoLoi) {
        Invoice h;
        if (!timHoaDon(maHD, h)) {
            thongBaoLoi = "Khong tim thay hoa don " + maHD;
            return false;
        }
        if (h.isDaThanhToan()) {
            thongBaoLoi = "Hoa don da thanh toan, khong duoc xoa.";
            return false;
        }

        ThaoTacHoanTac a("XOA_HOADON");
        a.hoaDon = h;
        undoService.ghiNhan(a);

        invoices.removeIf([&maHD](const Invoice& x) { return x.getMaHoaDon() == maHD; });
        hoaDonDaXoa = h;
        return true;
    }

    bool suaChiSoHoaDon(const string& maHD, double dienMoi, double nuocMoi, Invoice& hoaDonCapNhat, string& thongBaoLoi) {
        Invoice h;
        if (!timHoaDon(maHD, h)) {
            thongBaoLoi = "Khong tim thay hoa don " + maHD;
            return false;
        }
        if (h.isDaThanhToan()) {
            thongBaoLoi = "Hoa don da thanh toan, khong sua duoc.";
            return false;
        }

        string maPhong = h.getMaPhong();
        string thang = h.getThangNam();
        bool coMoiHon = false;
        invoices.traverseForward([&maPhong, &thang, &coMoiHon](const Invoice& x) {
            if (x.getMaPhong() == maPhong && x.getThangNam() > thang) coMoiHon = true;
        });
        if (coMoiHon) {
            thongBaoLoi = "Phong " + maPhong + " da co hoa don thang sau, chi sua duoc hoa don moi nhat.";
            return false;
        }

        if (dienMoi != h.getChiSoDienMoi() || nuocMoi != h.getChiSoNuocMoi()) {
            ThaoTacHoanTac a("SUA_HOADON");
            a.hoaDon = h;
            undoService.ghiNhan(a);
        }

        invoices.updateIf(
            [&maHD](const Invoice& x) { return x.getMaHoaDon() == maHD; },
            [dienMoi, nuocMoi](Invoice& x) { x.capNhatChiSoMoi(dienMoi, nuocMoi); });

        timHoaDon(maHD, hoaDonCapNhat);
        return true;
    }
};

#endif

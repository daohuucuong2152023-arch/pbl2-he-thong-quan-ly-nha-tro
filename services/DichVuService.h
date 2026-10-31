#ifndef DICHVUSERVICE_H
#define DICHVUSERVICE_H

#include <string>
#include <cctype>
#include "../data_structures/DoublyLinkedList.h"
#include "../models/Service.h"

using namespace std;

class DichVuService {
private:
    DoublyLinkedList<Service>& services;

    string taoMaDichVuMoi() const {
        int maxNo = 0;
        services.traverseForward([&maxNo](const Service& x) {
            string id = x.getId();
            if (id.size() > 2 && id.compare(0, 2, "DV") == 0) {
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
        return "DV" + s;
    }

public:
    DichVuService(DoublyLinkedList<Service>& s) : services(s) {}

    bool themDichVu(const string& ten, double gia, Service& dichVuMoi) {
        string ma = taoMaDichVuMoi();
        dichVuMoi = Service(ma, ten, gia);
        services.insertBack(dichVuMoi);
        return true;
    }

    bool timDichVu(const string& ma, Service& result) const {
        return services.findIf([&ma](const Service& x) { return x.getMaDichVu() == ma; }, result);
    }

    bool suaDichVu(const string& ma, const string& tenMoi, double giaMoi, string& thongBaoLoi) {
        Service s;
        if (!timDichVu(ma, s)) {
            thongBaoLoi = "Khong tim thay dich vu " + ma;
            return false;
        }

        services.updateIf(
            [&ma](const Service& x) { return x.getMaDichVu() == ma; },
            [&tenMoi, giaMoi](Service& x) {
                if (!tenMoi.empty()) x.setTenDichVu(tenMoi);
                x.setDonGia(giaMoi);
            });
        return true;
    }

    bool xoaDichVu(const string& ma, string& thongBaoLoi) {
        Service s;
        if (!timDichVu(ma, s)) {
            thongBaoLoi = "Khong tim thay dich vu " + ma;
            return false;
        }
        services.removeIf([&ma](const Service& x) { return x.getMaDichVu() == ma; });
        return true;
    }

    DoublyLinkedList<Service> layDanhSachDichVu() const {
        return services;
    }

    bool isEmpty() const {
        return services.isEmpty();
    }
};

#endif

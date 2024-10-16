#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <sstream>
using namespace std;

struct Aset {
    string nama;
    string jenis;
    int jumlah;
    double harga;

    Aset(string nama, string jenis, int jumlah, double harga)
        : nama(nama), jenis(jenis), jumlah(jumlah), harga(harga) {}

    virtual void nextYear() = 0;
    virtual string toString() const = 0;
};

struct Saham : public Aset {
    double dividen;
    double pertumbuhan;

    Saham(string nama, int jumlah, double harga, double pertumbuhan, double dividen)
        : Aset(nama, "SAHAM", jumlah, harga), pertumbuhan(pertumbuhan), dividen(dividen) {}

    void nextYear() override {
        grow();
        harga *= (1 + pertumbuhan);
    }

    void grow() {
        int a = 0x4b;
        int c = 0x4a;
        int m = 2;
        cout << int(a * pertumbuhan + c) % m << endl;
        pertumbuhan = (int(a * pertumbuhan + c) % m) - 1;
        pertumbuhan = pertumbuhan < 0 ? (int)pertumbuhan % -m : pertumbuhan;
    }

    string toString() const override {
        ostringstream oss;
        oss << fixed << setprecision(2);
        oss << nama << "\nTipe: " << jenis
            << "\nHarga: " << harga
            << "\nJumlah: " << jumlah
            << "\nDividen: " << dividen
            << "\nPertumbuhan: " << pertumbuhan;
        return oss.str();
    }
};

struct Obligasi : public Aset {
    double bunga;
    const double fixBunga;
    int jatuhTempo;
    double hargaMulai;

    Obligasi(string nama, int jumlah, double harga, double bunga, int maturitas)
        : Aset(nama, "OBLIGASI", jumlah, harga), hargaMulai(harga), fixBunga(harga * bunga), bunga(bunga), jatuhTempo(maturitas + 1) {}

    void nextYear() override {
        if (jatuhTempo == 0) return;
        jatuhTempo--;
        harga += fixBunga;
    }

    string toString() const override {
        ostringstream oss;
        oss << fixed << setprecision(2);
        oss << nama << "\nTipe: " << jenis
            << "\nHarga: " << hargaMulai
            << "\nJumlah: " << jumlah
            << "\nBunga: " << bunga
            << "\nJatuh Tempo: " << ((jatuhTempo == 0) ? "true" : "false");
        return oss.str();
    }
};

struct Node {
    Aset* data;
    Node* next;

    Node(Aset* data) : data(data), next(nullptr) {}
};

class Pacilnomo {
private:
    Node* head;
    double earnings;
    int asetCount;

    void printSeparator() const {
        cout << string(64, '=') << endl;
    }

    void daftarAset() const {
        printSeparator();
        cout << "Kamu memiliki " << asetCount << " total aset:" << endl;
        Node* current = head;
        while (current != nullptr) {
            cout << "- " << current->data->toString() << endl;
            current = current->next;
        }
        printSeparator();
    } 

    void infoPortofolio() const {
        int jumlahSaham = 0, jumlahObligasi = 0;
        double netWorth = 0;
        Node* current = head;

        while (current != nullptr) {
            if (current->data->jenis == "SAHAM") jumlahSaham++;
            else if (current->data->jenis == "OBLIGASI") jumlahObligasi++;
            netWorth += current->data->harga * current->data->jumlah;
            current = current->next;
        }

        printSeparator();
        cout << "Info Portofolio\n"
                  << "Jumlah Jenis Saham: " << jumlahSaham << "\n"
                  << "Jumlah Jenis Obligasi: " << jumlahObligasi << "\n"
                  << fixed << setprecision(2)
                  << "Total Nilai Portofolio: " << netWorth << endl;
        printSeparator();
    }

    void nextYear() {
        Node* current = head;
        while (current != nullptr) {
            current->data->nextYear();
            current = current->next;
        }
        cout << "Setahun telah berlalu...\n";
        printSeparator();
    }

    void insertFirst(Aset* aset) {
        Node* newNode = new Node(aset);
        newNode->next = head;
        head = newNode;
    }

    void insertLast(Aset* aset) {
        if (!head) {
            insertFirst(aset);
            return;
        }
        Node* newNode = new Node(aset);
        Node* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }

public:
    Pacilnomo() : head(nullptr), earnings(0), asetCount(0) {}

    void addAset(Aset* aset) {
        insertLast(aset);
    }

    void addToEarnings(double jumlah) {
        earnings += jumlah;
    }

    void run() {
        int banyakAset;
        cout << "Silakan masukkan banyak aset yang tersedia: ";
        cin >> banyakAset;
        asetCount = banyakAset;
        cin.ignore();

        for (int i = 0; i < banyakAset; i++) {
            cout << "Aset " << i + 1 << ": ";
            string namaAset, jenisAset;
            int jumlah;
            double harga, dividen_or_bunga, pertumbuhan;
            cin >> namaAset >> jenisAset >> jumlah >> harga >> pertumbuhan >> dividen_or_bunga;

            if (jenisAset == "SAHAM") {
                addAset(new Saham(namaAset, jumlah, harga, pertumbuhan, dividen_or_bunga));
            } else if (jenisAset == "OBLIGASI") {
                addAset(new Obligasi(namaAset, jumlah, harga, pertumbuhan, dividen_or_bunga));
            }
        }

        cout << "Selamat datang di...\n"
                  << " /$$$$$$$                     /$$ /$$\n"
                  << "| $$__  $$                   |__/| $$\n"
                  << "| $$  \\ $$ /$$$$$$   /$$$$$$$ /$$| $$ /$$$$$$$   /$$$$$$  /$$$$$$/$$$$   /$$$$$$\n"
                  << "| $$$$$$$/|____  $$ /$$_____/| $$| $$| $$__  $$ /$$__  $$| $$_  $$_  $$ /$$__  $$\n"
                  << "| $$____/  /$$$$$$$| $$      | $$| $$| $$  \\ $$| $$  \\ $$| $$ \\ $$ \\ $$| $$  \\ $$\n"
                  << "| $$      /$$__  $$| $$      | $$| $$| $$  | $$| $$  | $$| $$ | $$ | $$| $$  | $$\n"
                  << "| $$     |  $$$$$$$|  $$$$$$$| $$| $$| $$  | $$|  $$$$$$/| $$ | $$ | $$|  $$$$$$/\n"
                  << "|__/      \\_______/ \\_______/|__/|__/|__/  |__/ \\______/ |__/ |__/ |__/ \\______/\n\n";

        while (true) {
            cout << "Silakan pilih salah satu opsi berikut:\n"
                      << "[1] Daftar aset\n"
                      << "[2] Info portofolio\n"
                      << "[3] Lanjut ke tahun berikutnya\n"
                      << "[*] Keluar\n";
            printSeparator();
            cout << "Input: ";
            string pilihan;
            cin >> pilihan;
            if (pilihan == "1") {
                daftarAset();
            } else if (pilihan == "2") {
                infoPortofolio();
            } else if (pilihan == "3") {
                nextYear();
            } else {
                cout << "Terima kasih telah menggunakan layanan Pacilnomo ~ !\n";
                break;
            }
        }
    }
};

int main() {
    Pacilnomo pacilnomo;
    pacilnomo.run();
    return 0;
}

#include <iostream>
#include <iomanip>
using namespace std;

#define MAX_ANTRIAN 100   
#define MAX_STACK 100  

// =========================
// PRESS ANY KEY
// =========================
void pressAnyKey() {
    cout << "Press any key to continue . . .";
    cin.ignore(1000, '\n');
    cin.get();
    cout << "\n";
}

// =========================
// STRUCT BAGASI
// =========================
struct Bagasi {
    int flightTime; 
    float berat;    
};

// Cetak bagasi dengan format [Flight:0830|Berat:10.7kg]
void cetakBagasi(Bagasi b) {
    cout << "[Flight:" << setfill('0') << setw(4) << b.flightTime 
         << "|Berat:" << fixed << setprecision(1) << b.berat << "kg]";
}

// =========================
// CIRCULAR QUEUE
// =========================
struct Queue {
    Bagasi items[MAX_ANTRIAN];
    int front;
    int rear;
} q = {{}, -1, -1};

bool queueEmpty() {
    return q.front == -1;
}

bool queueFull() {
    return (q.rear + 1) % MAX_ANTRIAN == q.front;
}

void enqueue(Bagasi b) {
    if (queueFull()) {
        return;
    }

    if (queueEmpty()) {
        q.front = 0;
        q.rear = 0;
    } else {
        q.rear = (q.rear + 1) % MAX_ANTRIAN;
    }

    q.items[q.rear] = b;
}

Bagasi dequeue() {
    Bagasi temp = q.items[q.front];

    if (q.front == q.rear) {
        q.front = -1;
        q.rear = -1;
    } else {
        q.front = (q.front + 1) % MAX_ANTRIAN;
    }

    return temp;
}

void cetakQueue() {
    if (queueEmpty()) {
        cout << "(Queue Kosong)\n";
        return;
    }

    int i = q.front;

    while (true) {
        cout << "-> ";
        cetakBagasi(q.items[i]);
        cout << "\n";

        if (i == q.rear) {
            break;
        }

        i = (i + 1) % MAX_ANTRIAN;
    }
}

// =========================
// STACK
// =========================
struct Stack {
    Bagasi items[MAX_STACK];
    int top;
} s = {{}, -1};

bool stackEmpty() {
    return s.top == -1;
}

bool stackFull() {
    return s.top == MAX_STACK - 1;
}

void push(Bagasi b) {
    if (stackFull()) {
        return;
    }

    s.top++;
    s.items[s.top] = b;
}

Bagasi pop() {
    Bagasi temp = s.items[s.top];
    s.top--;
    return temp;
}

void cetakStack() {
    if (stackEmpty()) {
        cout << "(Stack Kosong)\n";
        return;
    }

    for (int i = s.top; i >= 0; i--) {
        cout << "[TOP] ";
        cetakBagasi(s.items[i]);
        cout << "\n";
    }
}

// =========================
// MAIN MENU
// =========================
int main() {
    int pilihan;

    do {
        cout << "\n=== Sistem Keamanan Bandara ===\n";
        cout << "1. Check-In Bagasi (langsung ke Queue)\n";
        cout << "2. Lihat Antrian Konveyor (Queue)\n";
        cout << "3. Proses Scanning X-Ray (Queue -> Stack)\n";
        cout << "4. Lihat Tumpukan Overweight (Stack)\n";
        cout << "5. Pemeriksaan Manual (Pop Stack)\n";
        cout << "0. Keluar\n";
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        // buang newline sisa input menu sebelum pressAnyKey pertama
        cin.ignore(1000, '\n');

        // ========================
        // MENU 1 - Input ke Queue
        // ========================
        if (pilihan == 1) {
            Bagasi b;

            cout << "\n--- Input Data Bagasi Baru ---\n";
            cout << "Flight Time (Format HHMM, cth: 1430): ";
            cin >> b.flightTime;

            int jam  = b.flightTime / 100;
            int menit = b.flightTime % 100;

            if (b.flightTime < 0 || b.flightTime > 2359 || menit > 59) {
                cout << "ERROR: Format jam tidak valid (0000 - 2359, menit 00-59).\n";
                pressAnyKey();
                continue;
            }

            cout << "Berat Bagasi (kg, harus > 0): ";
            cin >> b.berat;

            if (b.berat <= 0) {
                cout << "ERROR: Berat harus positif.\n";
                pressAnyKey();
                continue;
            }

            enqueue(b);
            cout << "-> Data berhasil masuk ke Antrian Konveyor (Queue).\n";
            pressAnyKey();
        }

        // ========================
        // MENU 2 - Lihat Queue
        // ========================
        else if (pilihan == 2) {
            cout << "\n--- Isi Antrian Konveyor (Queue) ---\n";
            cetakQueue();
            pressAnyKey();
        }

        // ========================
        // MENU 3 - Queue -> Stack (X-Ray)
        // ========================
        else if (pilihan == 3) {
            cout << "\n--- Memulai Proses Scanning X-Ray ---\n";

            if (queueEmpty()) {
                cout << "GAGAL: Konveyor kosong! Input data dulu di Menu 1.\n";
                pressAnyKey();
            } else {
                cout << "Aturan Filter: Berat > 9.3 kg = OVERWEIGHT (masuk Stack)\n\n";

                while (!queueEmpty()) {
                    Bagasi b = dequeue();

                    cout << "Scan: ";
                    cetakBagasi(b);

                    if (b.berat > 9.3f) {
                        cout << " -> STATUS: OVERWEIGHT! (Push ke Stack)\n";
                        push(b);
                    } else {
                        cout << " -> STATUS: AMAN (Lolos ke Pesawat)\n";
                    }
                }

                cout << "\n-> Konveyor telah kosong. Proses scanning selesai.\n";
                pressAnyKey();
            }
        }

        // ========================
        // MENU 4 - Lihat Stack
        // ========================
        else if (pilihan == 4) {
            cout << "\n--- Isi Area Isolasi (Tumpukan Stack) ---\n";
            cetakStack();
            pressAnyKey();
        }

        // ========================
        // MENU 5 - Pemeriksaan Manual (Pop Stack)
        // ========================
        else if (pilihan == 5) {
            cout << "\n--- Pemeriksaan Manual (Pop Stack sampai habis) ---\n";

            if (stackEmpty()) {
                cout << "INFO: Stack kosong. Tidak ada bagasi overweight.\n";
                pressAnyKey();
            } else {
                while (!stackEmpty()) {
                    Bagasi b = pop();
                    cout << "Petugas memeriksa: ";
                    cetakBagasi(b);
                    cout << " -> SELESAI DIPERIKSA.\n";
                }
                cout << "\n-> Semua bagasi overweight telah diperiksa. Tumpukan kosong.\n";
                pressAnyKey();
            }
        }

        // ========================
        // MENU 0 - Keluar
        // ========================
        else if (pilihan == 0) {
            cout << "\nKeluar dari program...\n";
            cout << "Terima kasih!\n";
        }

    } while (pilihan != 0);

    return 0;
}

// PENJELASAN

// 2. PENJELASAN PER MENU & GARIS BESAR KODE
// A. Bagian Atas: #include, define, helper
// #include <iostream>
// #include <iomanip>
// using namespace std;

// #define MAX_ANTRIAN 100   
// #define MAX_STACK 100  


// iostream untuk cin / cout

// iomanip untuk setw, setfill, setprecision

// MAX_ANTRIAN → kapasitas array Queue

// MAX_STACK → kapasitas array Stack

// void pressAnyKey() {
//     cout << "Press any key to continue . . .";
//     cin.ignore(1000, '\n');
//     cin.get();
//     cout << "\n";
// }


// Fungsi pause:

// Tampilkan pesan

// cin.ignore buang karakter tersisa di buffer input

// cin.get() menunggu user tekan satu tombol

// Cetak newline setelahnya

// B. Struct Bagasi + cetak
// struct Bagasi {
//     int flightTime; 
//     float berat;    
// };


// Satu bagasi punya:

// flightTime dalam format HHMM (misal 0830)

// berat dalam kg

// void cetakBagasi(Bagasi b) {
//     cout << "[Flight:" << setfill('0') << setw(4) << b.flightTime 
//          << "|Berat:" << fixed << setprecision(1) << b.berat << "kg]";
// }


// setfill('0') + setw(4) → 830 jadi 0830

// fixed << setprecision(1) → 10.73 dicetak 10.7

// Output akhir: [Flight:0534|Berat:10.7kg]

// C. Circular Queue
// struct Queue {
//     Bagasi items[MAX_ANTRIAN];
//     int front;
//     int rear;
// } q = {{}, -1, -1};


// items array untuk menyimpan bagasi

// front = indeks depan

// rear = indeks belakang

// awalnya -1 artinya kosong

// bool queueEmpty() { return q.front == -1; }
// bool queueFull()  { return (q.rear + 1) % MAX_ANTRIAN == q.front; }


// queueEmpty → true kalau belum ada data

// queueFull → true kalau next posisi rear ketemu front (circular)

// void enqueue(Bagasi b) { ... }


// Jika queue kosong → front = rear = 0

// Jika tidak kosong → rear maju 1 (modulo MAX_ANTRIAN)

// Simpan bagasi di items[rear]

// Bagasi dequeue() { ... }


// Ambil items[front]

// Kalau setelah diambil tinggal 1, set front & rear = -1 (jadi kosong)

// Kalau masih lebih dari 1, front digeser ke depan (modulo)

// void cetakQueue() { ... }


// Kalau kosong → tulis (Queue Kosong)

// Kalau ada isi:

// Mulai dari front

// Loop sampai rear

// Cetak dengan format -> [Flight:...|Berat:...kg]

// D. Stack
// struct Stack {
//     Bagasi items[MAX_STACK];
//     int top;
// } s = {{}, -1};


// items = array bagasi

// top = indeks elemen paling atas, -1 artinya kosong

// bool stackEmpty() { return s.top == -1; }
// bool stackFull()  { return s.top == MAX_STACK - 1; }

// void push(Bagasi b) {
//     if (stackFull()) return;
//     s.top++;
//     s.items[s.top] = b;
// }


// Tambah data di posisi top+1

// Bagasi pop() {
//     Bagasi temp = s.items[s.top];
//     s.top--;
//     return temp;
// }


// Ambil data paling atas lalu turunkan top

// void cetakStack() { ... }


// Kalau kosong → (Stack Kosong)

// Kalau ada isi → loop dari top ke bawah, setiap item dicetak "[TOP] [Flight:...|Berat:...kg]"

// 3. PENJELASAN MENU SATU PER SATU
// MENU 1 — Check-In Bagasi (langsung ke Queue)
// if (pilihan == 1) {
//     Bagasi b;

//     cout << "\n--- Input Data Bagasi Baru ---\n";
//     cout << "Flight Time (Format HHMM, cth: 1430): ";
//     cin >> b.flightTime;

//     int jam  = b.flightTime / 100;
//     int menit = b.flightTime % 100;


// Input jam penerbangan

// Pisahkan jam & menit untuk validasi

//     if (b.flightTime < 0 || b.flightTime > 2359 || menit > 59) {
//         cout << "ERROR: Format jam tidak valid (0000 - 2359, menit 00-59).\n";
//         pressAnyKey();
//         continue;
//     }


// Validasi:

// Tidak boleh < 0000 atau > 2359

// Menit tidak boleh lebih dari 59

//     cout << "Berat Bagasi (kg, harus > 0): ";
//     cin >> b.berat;

//     if (b.berat <= 0) {
//         cout << "ERROR: Berat harus positif.\n";
//         pressAnyKey();
//         continue;
//     }


// Validasi berat > 0

//     enqueue(b);
//     cout << "-> Data berhasil masuk ke Antrian Konveyor (Queue).\n";
//     pressAnyKey();
// }


// Data dimasukkan ke Queue (konveyor)

// Tampilkan pesan berhasil

// Pause dengan pressAnyKey()

// MENU 2 — Lihat Antrian Konveyor (Queue)
// else if (pilihan == 2) {
//     cout << "\n--- Isi Antrian Konveyor (Queue) ---\n";
//     cetakQueue();
//     pressAnyKey();
// }


// Menampilkan isi queue

// Kalau kosong, cetakQueue() menulis (Queue Kosong)

// MENU 3 — Proses Scanning X-Ray (Queue → Stack)
// else if (pilihan == 3) {
//     cout << "\n--- Memulai Proses Scanning X-Ray ---\n";

//     if (queueEmpty()) {
//         cout << "GAGAL: Konveyor kosong! Input data dulu di Menu 1.\n";
//         pressAnyKey();
//     } else {
//         cout << "Aturan Filter: Berat > 9.3 kg = OVERWEIGHT (masuk Stack)\n\n";


// Kalau queue kosong → error

// Kalau ada, tampilkan aturan filter

//         while (!queueEmpty()) {
//             Bagasi b = dequeue();

//             cout << "Scan: ";
//             cetakBagasi(b);

//             if (b.berat > 9.3f) {
//                 cout << " -> STATUS: OVERWEIGHT! (Push ke Stack)\n";
//                 push(b);
//             } else {
//                 cout << " -> STATUS: AMAN (Lolos ke Pesawat)\n";
//             }
//         }


// Dequeue satu per satu dari antrian

// Cetak Scan: [Flight:...|Berat:...kg]

// Jika berat > 9.3 → push ke stack (overweight)

// Jika ≤ 9.3 → aman

//         cout << "\n-> Konveyor telah kosong. Proses scanning selesai.\n";
//         pressAnyKey();
//     }
// }


// Setelah queue habis → tampilkan bahwa proses selesai

// MENU 4 — Lihat Tumpukan Overweight (Stack)
// else if (pilihan == 4) {
//     cout << "\n--- Isi Area Isolasi (Tumpukan Stack) ---\n";
//     cetakStack();
//     pressAnyKey();
// }


// Menampilkan isi stack:

// Dari paling atas ke bawah (Last In First Out)

// Setiap baris diawali [TOP] ...

// MENU 5 — Pemeriksaan Manual (Pop Stack sampai habis)
// else if (pilihan == 5) {
//     cout << "\n--- Pemeriksaan Manual (Pop Stack sampai habis) ---\n";

//     if (stackEmpty()) {
//         cout << "INFO: Stack kosong. Tidak ada bagasi overweight.\n";
//         pressAnyKey();
//     } else {
//         while (!stackEmpty()) {
//             Bagasi b = pop();
//             cout << "Petugas memeriksa: ";
//             cetakBagasi(b);
//             cout << " -> SELESAI DIPERIKSA.\n";
//         }
//         cout << "\n-> Semua bagasi overweight telah diperiksa. Tumpukan kosong.\n";
//         pressAnyKey();
//     }
// }


// Jika stack kosong → info tidak ada bagasi overweight

// Kalau ada:

// pop() satu per satu

// Cetak bahwa petugas memeriksa bagasi itu

// Setelah habis → tampilkan info bahwa stack kosong

// MENU 0 — Keluar
// else if (pilihan == 0) {
//     cout << "\nKeluar dari program...\n";
//     cout << "Terima kasih!\n";
// }


// Keluar dari loop do-while

// Program selesai
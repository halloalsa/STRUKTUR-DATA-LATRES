#include <iostream>
#include <iomanip>
using namespace std;

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
// QUEUE (Linked List)
// =========================
struct QNode {
    Bagasi data;
    QNode* next;
};

struct Queue {
    QNode* front;
    QNode* rear;
} q = {NULL, NULL};

bool queueEmpty() {
    return q.front == NULL;
}

void enqueue(Bagasi b) {
    QNode* baru = new QNode;
    baru->data = b;
    baru->next = NULL;

    if (queueEmpty()) {
        q.front = q.rear = baru;
    } else {
        q.rear->next = baru;
        q.rear = baru;
    }
}

Bagasi dequeue() {
    QNode* hapus = q.front;
    Bagasi temp = hapus->data;

    q.front = q.front->next;
    if (q.front == NULL) {
        q.rear = NULL;
    }

    delete hapus;
    return temp;
}

void cetakQueue() {
    if (queueEmpty()) {
        cout << "(Queue Kosong)\n";
        return;
    }

    QNode* bantu = q.front;
    while (bantu != NULL) {
        cout << "-> ";
        cetakBagasi(bantu->data);
        cout << "\n";
        bantu = bantu->next;
    }
}

// =========================
// STACK (Linked List)
// =========================
struct SNode {
    Bagasi data;
    SNode* next;
};

struct Stack {
    SNode* top;
} s = {NULL};

bool stackEmpty() {
    return s.top == NULL;
}

void push(Bagasi b) {
    SNode* baru = new SNode;
    baru->data = b;
    baru->next = s.top;
    s.top = baru;
}

Bagasi pop() {
    SNode* hapus = s.top;
    Bagasi temp = hapus->data;

    s.top = s.top->next;
    delete hapus;

    return temp;
}

void cetakStack() {
    if (stackEmpty()) {
        cout << "(Stack Kosong)\n";
        return;
    }

    SNode* bantu = s.top;
    while (bantu != NULL) {
        cout << "[TOP] ";
        cetakBagasi(bantu->data);
        cout << "\n";
        bantu = bantu->next;
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

// PENJELASAN.

// Library yang digunakan
// #include <iostream>
// #include <iomanip>
// using namespace std;


// iostream untuk input-output (cin, cout)

// iomanip untuk format tampilan (setw, setfill, setprecision)

// Karena kita ingin output [Flight:0830|Berat:10.7kg] seperti di PDF, iomanip wajib dipakai.

// Fungsi pressAnyKey()
// void pressAnyKey() {
//     cout << "Press any key to continue . . .";
//     cin.ignore(1000, '\n');
//     cin.get();
//     cout << "\n";
// }


// Fungsinya untuk:

// Menampilkan pesan

// Membersihkan buffer input (mengabaikan newline tersisa)

// Menunggu user menekan tombol apa saja

// Menambahkan baris baru setelahnya

// Fungsi ini dipanggil tiap akhir menu supaya tampilan rapi.

// =====================================================
// 2. STRUCT BAGASI & CETAK BAGASI
// =====================================================
// Struct Bagasi
// struct Bagasi {
//     int flightTime;
//     float berat;
// };


// flightTime = jam penerbangan (format HHMM).

// berat = berat bagasi (kg)

// Fungsi cetakBagasi()
// void cetakBagasi(Bagasi b) {
//     cout << "[Flight:" << setfill('0') << setw(4) << b.flightTime 
//          << "|Berat:" << fixed << setprecision(1) << b.berat << "kg]";
// }


// Tujuan:

// Menampilkan bagasi dengan format persis seperti PDF

// setw(4) + setfill('0') → memastikan 830 dicetak sebagai 0830

// setprecision(1) → hanya 1 angka di belakang koma

// Output contoh:

// [Flight:0534|Berat:10.7kg]

// =====================================================
// 3. QUEUE (ANTRIAN) – LINKED LIST
// =====================================================
// Node Queue
// struct QNode {
//     Bagasi data;
//     QNode* next;
// };


// Setiap node menyimpan satu Bagasi

// next menunjuk ke node berikutnya

// Struct Queue
// struct Queue {
//     QNode* front;
//     QNode* rear;
// } q = {NULL, NULL};


// front → ujung depan antrian

// rear → ujung belakang antrian

// Awal: front = rear = NULL artinya Queue kosong

// queueEmpty()
// bool queueEmpty() {
//     return q.front == NULL;
// }


// Queue kosong jika tidak ada node.

// Enqueue
// void enqueue(Bagasi b) {
//     QNode* baru = new QNode;
//     baru->data = b;
//     baru->next = NULL;

//     if (queueEmpty()) {
//         q.front = q.rear = baru;
//     } else {
//         q.rear->next = baru;
//         q.rear = baru;
//     }
// }


// Penjelasan:

// Buat node baru

// Jika queue kosong → node baru jadi front dan rear

// Jika tidak kosong → node baru ditambah di belakang (rear->next)

// Dequeue
// Bagasi dequeue() {
//     QNode* hapus = q.front;
//     Bagasi temp = hapus->data;

//     q.front = q.front->next;
//     if (q.front == NULL) {
//         q.rear = NULL;
//     }

//     delete hapus;
//     return temp;
// }


// Penjelasan:

// Simpan node terdepan

// Geser front ke node berikutnya

// Jika setelah geser menjadi NULL → queue kosong → rear = NULL

// Kembalikan data bagasi

// Cetak Queue
// void cetakQueue() {
//     if (queueEmpty()) {
//         cout << "(Queue Kosong)\n";
//         return;
//     }

//     QNode* bantu = q.front;
//     while (bantu != NULL) {
//         cout << "-> ";
//         cetakBagasi(bantu->data);
//         cout << "\n";
//         bantu = bantu->next;
//     }
// }


// Menampilkan tiap node dari depan ke belakang

// Format sesuai PDF

// =====================================================
// 4. STACK – LINKED LIST
// =====================================================
// Node Stack
// struct SNode {
//     Bagasi data;
//     SNode* next;
// };


// Setiap node menyimpan bagasi, dan pointer ke node di bawahnya.

// Struct Stack
// struct Stack {
//     SNode* top;
// } s = {NULL};


// Awalnya kosong (top=NULL).

// Cek Kosong
// bool stackEmpty() {
//     return s.top == NULL;
// }

// Push
// void push(Bagasi b) {
//     SNode* baru = new SNode;
//     baru->data = b;
//     baru->next = s.top;
//     s.top = baru;
// }


// Node baru ditumpuk di atas stack (LIFO)

// Pop
// Bagasi pop() {
//     SNode* hapus = s.top;
//     Bagasi temp = hapus->data;

//     s.top = s.top->next;
//     delete hapus;

//     return temp;
// }


// Ambil node teratas

// Geser top ke node bawahnya

// Cetak Stack
// void cetakStack() {
//     if (stackEmpty()) {
//         cout << "(Stack Kosong)\n";
//         return;
//     }

//     SNode* bantu = s.top;
//     while (bantu != NULL) {
//         cout << "[TOP] ";
//         cetakBagasi(bantu->data);
//         cout << "\n";
//         bantu = bantu->next;
//     }
// }


// Format sesuai screenshot PDF.

// =====================================================
// 5. MAIN MENU
// =====================================================
// Header Menu
// cout << "\n=== Sistem Keamanan Bandara ===\n";
// cout << "1. Check-In Bagasi (langsung ke Queue)\n";
// cout << "2. Lihat Antrian Konveyor (Queue)\n";
// cout << "3. Proses Scanning X-Ray (Queue -> Stack)\n";
// cout << "4. Lihat Tumpukan Overweight (Stack)\n";
// cout << "5. Pemeriksaan Manual (Pop Stack)\n";
// cout << "0. Keluar\n";
// cout << "Pilihan Anda: ";


// Ini sama dengan versi screenshot PDF.

// =====================================================
// MENU 1 — Check-In Bagasi (Input ke Queue)
// =====================================================
// if (pilihan == 1) {
//     Bagasi b;

//     cout << "\n--- Input Data Bagasi Baru ---\n";
//     cout << "Flight Time (Format HHMM, cth: 1430): ";
//     cin >> b.flightTime;


// User memasukkan jam penerbangan.

//     int jam = b.flightTime / 100;
//     int menit = b.flightTime % 100;


// Pisahkan jam & menit untuk validasi.

// Validasi Waktu
// if (b.flightTime < 0 || b.flightTime > 2359 || menit > 59)


// Sesuai aturan jam 0000–2359

// Menit tidak lebih dari 59

// Input Berat
// cout << "Berat Bagasi (kg, harus > 0): ";
// cin >> b.berat;


// Validasi >0

// Masukkan ke Queue
// enqueue(b);
// cout << "-> Data berhasil masuk ke Antrian Konveyor (Queue).\n";


// Setelah sukses → pause program dengan:

// pressAnyKey();

// =====================================================
// MENU 2 — Lihat Antrian Konveyor (Queue)
// =====================================================
// cout << "\n--- Isi Antrian Konveyor (Queue) ---\n";
// cetakQueue();
// pressAnyKey();


// Jika kosong → (Queue Kosong)
// Jika ada → tampilkan satu per satu:

// -> [Flight:0534|Berat:10.7kg]
// -> [Flight:0735|Berat:9.1kg]
// ...

// =====================================================
// MENU 3 — Scanning X-Ray (Queue → Stack)
// =====================================================
// cout << "\n--- Memulai Proses Scanning X-Ray ---\n";

// if (queueEmpty())
//     cout << "GAGAL: Konveyor kosong! Load data dulu di Menu 1.\n";


// Jika Queue kosong, proses dihentikan.

// Scan satu per satu
// while (!queueEmpty()) {
//     Bagasi b = dequeue();
//     cout << "Scan: ";
//     cetakBagasi(b);

// Filter overweight
// if (b.berat > 9.3f) {
//     cout << " -> STATUS: OVERWEIGHT! (Push ke Stack)\n";
//     push(b);
// } else {
//     cout << " -> STATUS: AMAN (Lolos ke Pesawat)\n";
// }


// Jika overweight → push ke stack

// Jika aman → lolos

// Setelah Queue habis:

// cout << "\n-> Konveyor telah kosong. Proses scanning selesai.\n";

// =====================================================
// MENU 4 — Lihat Tumpukan Overweight (Stack)
// =====================================================
// cout << "\n--- Isi Area Isolasi (Tumpukan Stack) ---\n";
// cetakStack();
// pressAnyKey();


// Kalau kosong → (Stack Kosong)
// Kalau ada:

// [TOP] [Flight:0534|Berat:10.7kg]
// [TOP] [Flight:0100|Berat:12.2kg]

// =====================================================
// MENU 5 — Pemeriksaan Manual (Pop Stack)
// =====================================================
// cout << "\n--- Pemeriksaan Manual (Pop Stack sampai habis) ---\n";


// Jika kosong:

// INFO: Stack kosong. Tidak ada bagasi overweight.


// Jika ada:

// while (!stackEmpty()) {
//     Bagasi b = pop();
//     cout << "Petugas memeriksa: ";
//     cetakBagasi(b);
//     cout << " -> SELESAI DIPERIKSA.\n";
// }


// Output:

// Petugas memeriksa: [Flight:0534|Berat:10.7kg] -> SELESAI DIPERIKSA.


// Setelah habis:

// -> Semua bagasi overweight telah diperiksa. Tumpukan kosong.

// =====================================================
// MENU 0 — Keluar Program
// =====================================================
// cout << "\nKeluar dari program...\n";
// cout << "Terima kasih!\n";
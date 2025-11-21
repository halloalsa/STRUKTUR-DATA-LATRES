#include <iostream>
#include <iomanip>  // untuk waktu, flight time.
using namespace std;

// struct jadiin satu
struct Bagasi {
    int flightTime;
    float berat;
};

// Cetak Bagasi, mencetak waktu dengan iomanip.
void cetakBagasi(Bagasi b) {
    cout << "[Flight:" 
         << setfill('0') << setw(4) << b.flightTime   // 0830, 1430, dst.
         << "|Berat:" 
         << fixed << setprecision(1) << b.berat       // 1 angka di belakang koma
         << "kg]";
}

// POHON TELUSUR BINER (PTB)
struct Node {
    Bagasi data;
    Node* left;
    Node* right;
};

Node* root = NULL;

// Cek duplikat flightTime
bool cekDuplikat(Node* r, int key) {
    if (!r) 
        return false;
    if (r->data.flightTime == key) 
        return true;
    if (key < r->data.flightTime) 
        return cekDuplikat(r->left, key);
    else 
        return cekDuplikat(r->right, key);
}

Node* insertNode(Node* r, Bagasi b) {
    if (!r) {
        return new Node{b, NULL, NULL};
    }
    if (b.flightTime < r->data.flightTime)
        r->left = insertNode(r->left, b);
    else
        r->right = insertNode(r->right, b);
    return r;
}

void inorder(Node* r) {
    if (!r) 
        return;
    inorder(r->left);
    cetakBagasi(r->data);
    cout << "\n";
    inorder(r->right);
}

void hapusPTB(Node*& r) {
    if (!r) 
        return;
    hapusPTB(r->left);
    hapusPTB(r->right);
    delete r;
    r = NULL;
}

//CIRCULAR QUEUE
#define MAXQ 50
struct Queue {
    Bagasi items[MAXQ];
    int front = -1;
    int rear = -1;
} q;

bool queueEmpty() { 
    return q.front == -1; }
bool queueFull() { 
    return (q.rear + 1) % MAXQ == q.front; }

void enqueue(Bagasi b) {
    if (queueFull()) 
        return;
    if (queueEmpty()) {
        q.front = q.rear = 0;
    } else {
        q.rear = (q.rear + 1) % MAXQ;
    }
    q.items[q.rear] = b;
}

Bagasi dequeue() {
    Bagasi temp = q.items[q.front];
        if (q.front == q.rear) q.front = q.rear = -1;
        else q.front = (q.front + 1) % MAXQ;
            return temp;
}

void cetakQueue() {
    if (queueEmpty()) {
        cout << "   (Queue Kosong)\n";
        return;
    }
    int i = q.front;
    while (true) {
        cout << "   -> ";
        cetakBagasi(q.items[i]);
        cout << "\n";
        if (i == q.rear) 
            break;
        i = (i + 1) % MAXQ;
    }
}

// STACK
#define MAXS 50
struct Stack {
    Bagasi items[MAXS];
    int top = -1;
} s;

bool stackEmpty() { 
    return s.top == -1; }
bool stackFull() { 
    return s.top == MAXS - 1; }

void push(Bagasi b) {
    if (!stackFull()) {
        s.items[++s.top] = b;
    }
}

Bagasi pop() {
    return s.items[s.top--];
}

void cetakStack() {
    if (stackEmpty()) {
        cout << "   (Stack Kosong)\n";
        return;
    }
    for (int i = s.top; i >= 0; i--) {
        cout << "   -> ";
        cetakBagasi(s.items[i]);
        cout << "\n";
    }
}

// Main Menu
int main() {
    int pilihan;

    do {
        cout << "\n=== SISTEM KEAMANAN BANDARA ===\n";
        cout << "1. [PTB] Check-In Bagasi\n";
        cout << "2. [PTB] Lihat Database Bagasi\n";
        cout << "3. [PTB -> Queue] Pindahkan Semua ke Konveyor\n";
        cout << "4. [Queue] Lihat Antrian Konveyor\n";
        cout << "5. [Queue -> Stack] Proses Scanning X-Ray\n";
        cout << "6. [Stack] Lihat Tumpukan Overweight\n";
        cout << "7. [Stack Process] Pemeriksaan Manual\n";
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;

        // MENU 1 - Input PTB
        if (pilihan == 1) {
            Bagasi b;

            cout << "\nFlight Time (Format HHMM, cth: 1430): ";
            cin >> b.flightTime;

            int jam = b.flightTime / 100;
            int menit = b.flightTime % 100;

            if (b.flightTime < 0 || b.flightTime > 2359 || menit > 59) {
                cout << "ERROR: Format jam tidak valid (0000 - 2359, menit 00-59).\n";
                continue;
            }

            if (cekDuplikat(root, b.flightTime)) {
                cout << "ERROR: Flight Time tidak boleh duplikat.\n";
                continue;
            }

            cout << "Berat Bagasi (kg, harus > 0): ";
            cin >> b.berat;

            if (b.berat <= 0) {
                cout << "ERROR: Berat harus positif.\n";
                continue;
            }

            root = insertNode(root, b);
            cout << ">> Data berhasil masuk ke Database PTB.\n";
        }

        // MENU 2 - Lihat PTB
        else if (pilihan == 2) {
            if (!root) 
                cout << "(Database kosong).\n";
            else {
                cout << "\n=== Isi Database Bagasi (PTB Inorder) ===\n";
                inorder(root);
            }
        }

        // MENU 3 - PTB -> Queue
        else if (pilihan == 3) {
            if (!root) {
                cout << "GAGAL: PTB Kosong! Input data dulu di Menu 1.\n";
            } else {
                cout << "\n=== Memindahkan Data ke Konveyor ===\n";

                // traversal + enqueue
                struct Helper {
                    static void transfer(Node* r) {
                        if (!r) 
                            return;
                        transfer(r->left);
                        enqueue(r->data);
                        transfer(r->right);
                    }
                };
                Helper::transfer(root);

                hapusPTB(root);

                cout << "-> SUKSES: Semua data dipindahkan dari PTB ke Queue Konveyor.\n";
                cout << "-> INFO: Database PTB sekarang telah dikosongkan.\n";
            }
        }

        // MENU 4 - lihat Queue
        else if (pilihan == 4) {
            cout << "\n=== Isi Antrian Konveyor (Queue) ===\n";
            cetakQueue();
        }

        // MENU 5 - Queue -> Stack
        else if (pilihan == 5) {
            if (queueEmpty()) {
                cout << "GAGAL: Konveyor kosong! Load data dulu di Menu 3.\n";
            } else {
                cout << "\n=== Memulai Proses Scanning X-Ray ===\n";
                while (!queueEmpty()) {
                    Bagasi b = dequeue();

                    cetakBagasi(b);

                    if (b.berat <= 9.3f) {
                        cout << "Aturan Filter: Berat > 9.3 kg = OVERWEIGHT (masuk Stack)\n";
                    } else {
                        cout << " -> Konveyor telah kosong. Proses scanning selesai.\n";
                        push(b);
                    }
                }
            }
        }

        // MENU 6 - lihat Stack
        else if (pilihan == 6) {
            cout << "\n=== Isi Area Isolasi (Tumpukan Stack) ===\n";
            cetakStack();
        }

        // MENU 7 - pemeriksaan manual
        else if (pilihan == 7) {
            if (stackEmpty()) {
                cout << "INFO: Stack kosong. Tidak ada bagasi overweight.\n";
            } else {
                cout << "\n=== Pemeriksaan Manual (Pop Stack sampai habis) ===\n";
                while (!stackEmpty()) {
                    Bagasi b = pop();
                    cout << "Petugas Memeriksa -> ";
                    cetakBagasi(b);
                    cout << "-> Semua bagasi overweight telah diperiksa. Tumpukan kosong.\n";
                }
            }
        }

       // MENU 0 - Keluar
        else if (pilihan == 0) {
            hapusPTB(root);
            cout << "\n>> Keluar dari program, terima kasih !\n";
        }

    } while (pilihan != 0);

    return 0;
}

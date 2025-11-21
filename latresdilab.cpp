#include <iostream>
#include <iomanip>
using namespace std;

#define MAX_ANTRIAN 100   
#define MAX_STACK 100  

void pressAnyKey() {
    cout << "Press any key to continue . . .";
    cin.ignore(1000, '\n');
    cin.get();
    cout << "\n";
}

struct Bagasi {
    int flightTime; 
    float berat;    
};

void cetakBagasi(Bagasi b) {
    cout << "[Flight:" << setfill('0') << setw(4) << b.flightTime 
         << "|Berat:" << fixed << setprecision(1) << b.berat << "kg]";
}

// PTB.
struct Node {
    Bagasi data;
    Node* left;
    Node* right;
};

Node* root = NULL;

// Cek apakah flightTime sudah ada di PTB (tidak boleh duplikat)
bool cekDuplikat(Node* r, int key) {
    if (r == NULL) {
        return false;
    }

    if (r->data.flightTime == key) {
        return true;
    } else if (key < r->data.flightTime) {
        return cekDuplikat(r->left, key);
    } else {
        return cekDuplikat(r->right, key);
    }
}

// Insert node baru ke PTB, key = flightTime
Node* insertNode(Node* r, Bagasi b) {
    if (r == NULL) {
        Node* baru = new Node;
        baru->data = b;
        baru->left = NULL;
        baru->right = NULL;
        return baru;
    }

    if (b.flightTime < r->data.flightTime) {
        r->left = insertNode(r->left, b);
    } else {
        r->right = insertNode(r->right, b);
    }

    return r;
}

// Traversal inorder: dari flight paling pagi ke paling malam
void inorder(Node* r) {
    if (r == NULL) {
        return;
    }

    inorder(r->left);
    cout << "-> ";
    cetakBagasi(r->data);
    cout << "\n";
    inorder(r->right);
}

// Hapus PTB.
void hapusPTB(Node*& r) {
    if (r == NULL) {
        return;
    }

    hapusPTB(r->left);
    hapusPTB(r->right);

    delete r;
    r = NULL;
}

// CIRCULAR QUEUE.
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

// STACK.
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

// MAIN MENU.
int main() {
    int pilihan;

    do {
        cout << "\n=== Sistem Keamanan Bandara ===\n";
        cout << "1. [PTB] Check-In Bagasi (Input Data)\n";
        cout << "2. [PTB] Lihat Database Bagasi (Cetak PTB)\n";
        cout << "3. [PTB -> Queue] Pindahkan Semua ke Konveyor (Hapus PTB)\n";
        cout << "4. [Queue] Lihat Antrian Konveyor (Cetak Queue)\n";
        cout << "5. [Queue -> Stack] Proses Scanning X-Ray\n";
        cout << "6. [Stack] Lihat Tumpukan Overweight (Cetak Stack)\n";
        cout << "7. [Stack Process] Pemeriksaan Manual (Pop Semua)\n";
        cout << "0. Keluar\n";
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        // buang newline sisa input menu sebelum pressAnyKey pertama
        cin.ignore(1000, '\n');

        // MENU 1 - Input PTB
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

            if (cekDuplikat(root, b.flightTime)) {
                cout << "ERROR: Flight Time tidak boleh duplikat.\n";
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

            root = insertNode(root, b);
            cout << "-> Data berhasil masuk ke Database PTB.\n";
            pressAnyKey();
        }

        // MENU 2 - Lihat PTB
        else if (pilihan == 2) {
            if (root == NULL) {
                cout << "\n--- Isi Database Bagasi (PTB) ---\n";
                cout << "(Database Kosong)\n";
                pressAnyKey();
            } else {
                cout << "\n--- Isi Database Bagasi (PTB Inorder) ---\n";
                inorder(root);
                cout << "\n";
                pressAnyKey();
            }
        }

        // MENU 3 - PTB -> Queue
        else if (pilihan == 3) {
            cout << "\n--- Memindahkan Data ke Konveyor ---\n";

            if (root == NULL) {
                cout << "GAGAL: PTB Kosong! Input data dulu di Menu 1.\n";
                pressAnyKey();
            } else {
                struct Traversal {
                    static void transfer(Node* r) {
                        if (r == NULL) return;
                        transfer(r->left);
                        enqueue(r->data);
                        transfer(r->right);
                    }
                };

                Traversal::transfer(root);
                hapusPTB(root);

                cout << "-> SUKSES: Semua data dipindahkan dari PTB ke Queue Konveyor.\n";
                cout << "-> INFO: Database PTB sekarang telah dikosongkan.\n";
                pressAnyKey();
            }
        }

        // MENU 4 - lihat Queue
        else if (pilihan == 4) {
            cout << "\n--- Isi Antrian Konveyor (Queue) ---\n";
            cetakQueue();
            pressAnyKey();
        }

        // MENU 5 - Queue -> Stack (X-Ray)
        else if (pilihan == 5) {
            cout << "\n--- Memulai Proses Scanning X-Ray ---\n";

            if (queueEmpty()) {
                cout << "GAGAL: Konveyor kosong! Load data dulu di Menu 3.\n";
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

        // MENU 6 - lihat Stack
        else if (pilihan == 6) {
            cout << "\n--- Isi Area Isolasi (Tumpukan Stack) ---\n";
            cetakStack();
            pressAnyKey();
        }

        // MENU 7 - pemeriksaan manual (Stack)
        else if (pilihan == 7) {
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

        // KELUAR.
        else if (pilihan == 0) {
            hapusPTB(root);
            cout << "\nKeluar dari program...\n";
            cout << "Terima kasih!\n";
        }

    } while (pilihan != 0);

    return 0;
}

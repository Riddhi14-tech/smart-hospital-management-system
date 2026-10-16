#include <iostream>
using namespace std;

// ======================================================
// SECTION 1: RIDDHI JAGWAN
// Common definitions used by all team members
// ======================================================

const int MAX_PATIENTS = 100;
const int MAX_HISTORY = 5;
const int MAX_APPOINTMENTS = 100;
const int MAX_EMERGENCIES = 100;

struct Patient {
    int id;
    int age;
    char name[50];
    char vitals[50];
    char history[MAX_HISTORY][100];
    int historyCount;
};

struct Doctor {
    int id;
    char name[30];
    char department[30];
};

struct Appointment {
    int patientId;
    int doctorId;
};

struct Emergency {
    int patientId;
    int severity;
};

// ======================================================
// SECTION 2: SANCHITA KUKRETI
// Patient Registration and Clinical Records
// ======================================================

class PatientManager {
private:
    Patient patients[MAX_PATIENTS];
    int count;
    int nextId;

public:
    PatientManager() {
        count = 0;
        nextId = 1001;
    }

    int findPatient(int id) {
        for (int i = 0; i < count; i++) {
            if (patients[i].id == id)
                return i;
        }
        return -1;
    }

    bool exists(int id) {
        return findPatient(id) != -1;
    }

    void registerPatient() {
        if (count >= MAX_PATIENTS) {
            cout << "Patient storage is full!\n";
            return;
        }

        Patient &p = patients[count];

        p.id = nextId++;

        cout << "Enter patient name (one word): ";
        cin >> p.name;

        cout << "Enter age: ";
        cin >> p.age;

        p.vitals[0] = '\0';
        p.historyCount = 0;

        count++;

        cout << "Patient registered successfully!\n";
        cout << "Patient ID: " << p.id << "\n";
    }

    void showPatient() {
        int id;

        cout << "Enter patient ID: ";
        cin >> id;

        int index = findPatient(id);

        if (index == -1) {
            cout << "Patient not found!\n";
            return;
        }

        Patient &p = patients[index];

        cout << "\nPatient ID: " << p.id;
        cout << "\nName: " << p.name;
        cout << "\nAge: " << p.age;
        cout << "\nVitals: ";

        if (p.vitals[0] == '\0')
            cout << "Not recorded";
        else
            cout << p.vitals;

        cout << "\nMedical History:\n";

        if (p.historyCount == 0)
            cout << "No history recorded.\n";

        for (int i = 0; i < p.historyCount; i++)
            cout << "- " << p.history[i] << "\n";
    }

    void addVitals() {
        int id;

        cout << "Enter patient ID: ";
        cin >> id;

        int index = findPatient(id);

        if (index == -1) {
            cout << "Patient not found!\n";
            return;
        }

        cout << "Enter vitals (one word, e.g. BP120_80): ";
        cin >> patients[index].vitals;

        cout << "Vitals saved successfully.\n";
    }

    void addHistory() {
        int id;

        cout << "Enter patient ID: ";
        cin >> id;

        int index = findPatient(id);

        if (index == -1) {
            cout << "Patient not found!\n";
            return;
        }

        Patient &p = patients[index];

        if (p.historyCount >= MAX_HISTORY) {
            cout << "Medical history storage is full!\n";
            return;
        }

        cout << "Enter medical note (one word): ";
        cin >> p.history[p.historyCount];

        p.historyCount++;

        cout << "Medical history added.\n";
    }

    void listPatients() {
        if (count == 0) {
            cout << "No patients registered.\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            cout << "ID: " << patients[i].id
                 << " | Name: " << patients[i].name
                 << " | Age: " << patients[i].age << "\n";
        }
    }
};

// ======================================================
// SECTION 3: PRERNA RAWAT
// Doctor Management and FIFO Appointment Queue
// ======================================================

class HospitalStaff {
public:
    virtual void displayRole() {
        cout << "Hospital Staff";
    }

    virtual ~HospitalStaff() {}
};

class DoctorStaff : public HospitalStaff {
private:
    Doctor info;

public:
    // Default constructor
    DoctorStaff() : info{0, "", ""} {}

    // Parameterized constructor
    DoctorStaff(Doctor d) : info(d) {}

    // Get doctor ID
    int getId() {
        return info.id;
    }

    // Get doctor name
    char* getName() {
        return info.name;
    }

    // Get department
    char* getDepartment() {
        return info.department;
    }

    // Function overriding
    void displayRole() override {
        cout << "Doctor: Dr. " << info.name
             << " | Department: " << info.department;
    }
};

class ResourceManager {
private:
    DoctorStaff doctors[3];

    Appointment appointments[MAX_APPOINTMENTS];
    int front;
    int rear;
    int appointmentCount;

public:
    // Constructor
    ResourceManager() {
        doctors[0] = DoctorStaff(
            Doctor{201, "Aarav", "General"}
        );

        doctors[1] = DoctorStaff(
            Doctor{202, "Meera", "Cardiology"}
        );

        doctors[2] = DoctorStaff(
            Doctor{203, "Kabir", "Pediatrics"}
        );

        front = 0;
        rear = 0;
        appointmentCount = 0;
    }

    // Display doctors using runtime polymorphism
    void showDoctors() {
        cout << "\nAvailable Doctors:\n";

        for (int i = 0; i < 3; i++) {
            HospitalStaff* staff = &doctors[i];

            cout << "ID: " << doctors[i].getId()
                 << " | ";

            staff->displayRole();

            cout << "\n";
        }
    }

    // Linear search for doctor ID
    bool validDoctor(int id) {
        for (int i = 0; i < 3; i++) {
            if (doctors[i].getId() == id)
                return true;
        }

        return false;
    }

    // Find doctor position
    int findDoctor(int id) {
        for (int i = 0; i < 3; i++) {
            if (doctors[i].getId() == id)
                return i;
        }

        return -1;
    }

    // Book appointment in circular queue
    void bookAppointment(PatientManager &pm) {
        if (appointmentCount >= MAX_APPOINTMENTS) {
            cout << "Appointment queue is full!\n";
            return;
        }

        int patientId, doctorId;

        cout << "Enter patient ID: ";
        cin >> patientId;

        if (!pm.exists(patientId)) {
            cout << "Patient not found!\n";
            return;
        }

        showDoctors();

        cout << "Enter doctor ID: ";
        cin >> doctorId;

        if (!validDoctor(doctorId)) {
            cout << "Invalid doctor ID!\n";
            return;
        }

        // Prevent duplicate pending appointments
        for (int i = 0; i < appointmentCount; i++) {
            int index = (front + i) % MAX_APPOINTMENTS;

            if (appointments[index].patientId == patientId &&
                appointments[index].doctorId == doctorId) {
                cout << "Duplicate pending appointment!\n";
                return;
            }
        }

        appointments[rear].patientId = patientId;
        appointments[rear].doctorId = doctorId;

        rear = (rear + 1) % MAX_APPOINTMENTS;
        appointmentCount++;

        cout << "Appointment booked successfully.\n";
    }

    // Display appointments in FIFO order
    void showAppointments() {
        if (appointmentCount == 0) {
            cout << "No appointments available.\n";
            return;
        }

        cout << "\nAppointments in FIFO order:\n";

        for (int i = 0; i < appointmentCount; i++) {
            int index = (front + i) % MAX_APPOINTMENTS;

            cout << "Patient ID: "
                 << appointments[index].patientId
                 << " -> Doctor ID: "
                 << appointments[index].doctorId << "\n";
        }
    }

    // Process the oldest appointment
    void processAppointment() {
        if (appointmentCount == 0) {
            cout << "No appointments to process.\n";
            return;
        }

        cout << "Processing appointment for Patient "
             << appointments[front].patientId
             << " with Doctor "
             << appointments[front].doctorId << "\n";

        front = (front + 1) % MAX_APPOINTMENTS;
        appointmentCount--;
    }

    // Doctor-wise schedule
    void showDoctorSchedule() {
        int doctorId;

        showDoctors();

        cout << "Enter doctor ID: ";
        cin >> doctorId;

        if (!validDoctor(doctorId)) {
            cout << "Invalid doctor ID!\n";
            return;
        }

        int found = 0;

        cout << "\nPending appointments for Doctor "
             << doctorId << ":\n";

        for (int i = 0; i < appointmentCount; i++) {
            int index = (front + i) % MAX_APPOINTMENTS;

            if (appointments[index].doctorId == doctorId) {
                cout << "Patient ID: "
                     << appointments[index].patientId << "\n";
                found++;
            }
        }

        if (found == 0)
            cout << "No pending appointments for this doctor.\n";
    }

    // Doctor workload report
    void showDoctorWorkload() {
        cout << "\nDoctor Workload Report:\n";

        for (int i = 0; i < 3; i++) {
            int total = 0;

            for (int j = 0; j < appointmentCount; j++) {
                int index = (front + j) % MAX_APPOINTMENTS;

                if (appointments[index].doctorId ==
                    doctors[i].getId()) {
                    total++;
                }
            }

            cout << "Dr. " << doctors[i].getName()
                 << " | Department: "
                 << doctors[i].getDepartment()
                 << " | Pending appointments: "
                 << total << "\n";
        }
    }
};

// ======================================================
// SECTION 4: ASHNA HASSAN
// Emergency Case Priority Management
// ======================================================

class EmergencyManager {
private:
    Emergency emergencies[MAX_EMERGENCIES];
    int count;

public:
    EmergencyManager() {
        count = 0;
    }

    void addEmergency(PatientManager &pm) {
        if (count >= MAX_EMERGENCIES) {
            cout << "Emergency storage is full!\n";
            return;
        }

        int id, severity;

        cout << "Enter patient ID: ";
        cin >> id;

        if (!pm.exists(id)) {
            cout << "Patient not found!\n";
            return;
        }

        cout << "Clinician-assigned severity:\n";
        cout << "1 = Low\n";
        cout << "2 = Medium\n";
        cout << "3 = High\n";
        cout << "4 = Critical\n";
        cout << "Enter severity: ";
        cin >> severity;

        if (severity < 1 || severity > 4) {
            cout << "Invalid severity!\n";
            return;
        }

        Emergency e = {id, severity};

        // Manual priority insertion using an array.
        // Higher severity is displayed first.
        int i = count;

        while (i > 0 &&
               emergencies[i - 1].severity < e.severity) {
            emergencies[i] = emergencies[i - 1];
            i--;
        }

        emergencies[i] = e;
        count++;

        cout << "Emergency case added successfully.\n";
    }

    void showEmergencies() {
        if (count == 0) {
            cout << "No emergency cases available.\n";
            return;
        }

        cout << "\nEmergency Priority List:\n";

        for (int i = 0; i < count; i++) {
            cout << "Patient ID: "
                 << emergencies[i].patientId
                 << " | Severity: ";

            switch (emergencies[i].severity) {
                case 1: cout << "Low"; break;
                case 2: cout << "Medium"; break;
                case 3: cout << "High"; break;
                case 4: cout << "Critical"; break;
            }

            cout << "\n";
        }
    }

    void processEmergency() {
        if (count == 0) {
            cout << "No emergency cases to process.\n";
            return;
        }

        cout << "Next priority case: Patient "
             << emergencies[0].patientId
             << " | Severity: "
             << emergencies[0].severity << "\n";

        cout << "Forward this case for clinician assessment.\n";

        for (int i = 1; i < count; i++)
            emergencies[i - 1] = emergencies[i];

        count--;
    }
};

// ======================================================
// SECTION 5: RIDDHI JAGWAN
// Bed Management + Main Menu Integration
// ======================================================

class BedManager {
private:
    // Each Bed node is part of a singly linked list.
    struct Bed {
        int number;
        bool occupied;
        int patientId;
        Bed* next;
    };

    Bed* head;

public:
    // Constructor: dynamically allocate three bed nodes.
    BedManager() {
        head = NULL;
        Bed* tail = NULL;

        for (int i = 1; i <= 3; i++) {
            Bed* newBed = new Bed;

            newBed->number = i;
            newBed->occupied = false;
            newBed->patientId = -1;
            newBed->next = NULL;

            if (head == NULL) {
                head = newBed;
                tail = newBed;
            } else {
                tail->next = newBed;
                tail = newBed;
            }
        }
    }

    // Traverse the linked list and display bed status.
    void showBeds() {
        cout << "\nBed Availability:\n";

        Bed* current = head;

        while (current != NULL) {
            cout << "Bed " << current->number << ": ";

            if (current->occupied)
                cout << "Occupied by Patient " << current->patientId;
            else
                cout << "Available";

            cout << "\n";
            current = current->next;
        }
    }

    void allocateBed(PatientManager &pm) {
        int id;

        cout << "Enter patient ID: ";
        cin >> id;

        if (!pm.exists(id)) {
            cout << "Patient not found!\n";
            return;
        }

        // Prevent the same patient from receiving multiple beds.
        Bed* current = head;
        while (current != NULL) {
            if (current->occupied && current->patientId == id) {
                cout << "Patient already has a bed!\n";
                return;
            }
            current = current->next;
        }

        // Find the first available bed.
        current = head;
        while (current != NULL) {
            if (!current->occupied) {
                current->occupied = true;
                current->patientId = id;

                cout << "Bed " << current->number
                     << " allocated to Patient " << id << "\n";
                return;
            }
            current = current->next;
        }

        cout << "No beds available!\n";
    }

    // Destructor: release all memory allocated with new.
    ~BedManager() {
        Bed* current = head;

        while (current != NULL) {
            Bed* temp = current;
            current = current->next;
            delete temp;
        }

        head = NULL;
    }
};

int main() {
    PatientManager patients;
    ResourceManager resources;
    EmergencyManager emergencies;
    BedManager beds;

    int choice;

    do {
        cout << "\n===== SMART HOSPITAL MANAGEMENT SYSTEM =====\n";
        cout << "1. Register patient\n";
        cout << "2. Show patient profile\n";
        cout << "3. Record patient vitals\n";
        cout << "4. Add medical history\n";
        cout << "5. List all patients\n";
        cout << "6. Show doctors\n";
        cout << "7. Book appointment\n";
        cout << "8. Show appointments\n";
        cout << "9. Process next appointment\n";
        cout << "10. Show bed availability\n";
        cout << "11. Allocate bed\n";
        cout << "12. Add emergency case\n";
        cout << "13. Show emergency priority list\n";
        cout << "14. Process highest-priority case\n";
        cout << "15. Show doctor-wise schedule\n";
        cout << "16. Show doctor workload report\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice) {
            case 1:
                patients.registerPatient();
                break;

            case 2:
                patients.showPatient();
                break;

            case 3:
                patients.addVitals();
                break;

            case 4:
                patients.addHistory();
                break;

            case 5:
                patients.listPatients();
                break;

            case 6:
                resources.showDoctors();
                break;

            case 7:
                resources.bookAppointment(patients);
                break;

            case 8:
                resources.showAppointments();
                break;

            case 9:
                resources.processAppointment();
                break;

            case 10:
                beds.showBeds();
                break;

            case 11:
                beds.allocateBed(patients);
                break;

            case 12:
                emergencies.addEmergency(patients);
                break;

            case 13:
                emergencies.showEmergencies();
                break;

            case 14:
                emergencies.processEmergency();
                break;
            case 15:
                resources.showDoctorSchedule();
                break;

            case 16:
                resources.showDoctorWorkload();
                break;
            case 0:
                cout << "Hospital system closed.\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
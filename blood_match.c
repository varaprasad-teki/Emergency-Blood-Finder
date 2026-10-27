#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DONORS 200
#define DATA_FILE "donors.dat"

struct Donor {
    int id;
    char name[50];
    int age;
    char gender[12];
    char bloodGroup[6];
    char phone[20];
    char location[60];
    double latitude;
    double longitude;
    int lastDonationDays;
    int donationsCount;
    int isAvailable;
    int emergencyPriority;
};

static void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        ;
    }
}

static int loginAdmin(void) {
    char username[30];
    char password[30];

    printf("\n=== Admin Login ===\n");
    printf("Username: ");
    scanf("%29s", username);
    clearInputBuffer();

    printf("Password: ");
    scanf("%29s", password);
    clearInputBuffer();

    if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
        printf("Login successful.\n");
        return 1;
    }

    printf("Invalid login credentials.\n");
    return 0;
}

static int saveDonorsToFile(const struct Donor *donors, int count) {
    FILE *file = fopen(DATA_FILE, "wb");
    if (!file) {
        printf("Unable to open donor data file for writing.\n");
        return 0;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(donors, sizeof(struct Donor), count, file);
    fclose(file);
    return 1;
}

static int loadDonorsFromFile(struct Donor *donors, int *count) {
    FILE *file = fopen(DATA_FILE, "rb");
    if (!file) {
        *count = 0;
        return 1;
    }

    fread(count, sizeof(int), 1, file);
    if (*count < 0 || *count > MAX_DONORS) {
        *count = 0;
        fclose(file);
        return 1;
    }

    fread(donors, sizeof(struct Donor), *count, file);
    fclose(file);
    return 1;
}

static void printDonor(const struct Donor *donor) {
    printf("\nID: %d\n", donor->id);
    printf("Name: %s\n", donor->name);
    printf("Age: %d\n", donor->age);
    printf("Gender: %s\n", donor->gender);
    printf("Blood Group: %s\n", donor->bloodGroup);
    printf("Phone: %s\n", donor->phone);
    printf("Location: %s\n", donor->location);
    printf("Coordinates: %.5f, %.5f\n", donor->latitude, donor->longitude);
    printf("Last Donation: %d days\n", donor->lastDonationDays);
    printf("Donations: %d\n", donor->donationsCount);
    printf("Availability: %s\n", donor->isAvailable ? "Available" : "Unavailable");
}

static int isEligibleDonor(const struct Donor *donor) {
    return donor->lastDonationDays >= 90 && donor->isAvailable;
}

static int detectEmergencyPriority(const struct Donor *donor) {
    int score = 0;

    if (donor->isAvailable) score += 30;
    if (donor->lastDonationDays >= 90) score += 20;
    if (donor->donationsCount >= 3) score += 10;
    if (strcmp(donor->bloodGroup, "O-") == 0 || strcmp(donor->bloodGroup, "O+") == 0) score += 25;
    if (strcmp(donor->bloodGroup, "AB+") == 0) score += 20;

    return score;
}

static double haversineDistanceKm(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371.0;
    double dLat = (lat2 - lat1) * 3.141592653589793 / 180.0;
    double dLon = (lon2 - lon1) * 3.141592653589793 / 180.0;
    double a = sin(dLat / 2.0) * sin(dLat / 2.0) +
               cos(lat1 * 3.141592653589793 / 180.0) *
               cos(lat2 * 3.141592653589793 / 180.0) *
               sin(dLon / 2.0) * sin(dLon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return R * c;
}

static int canDonate(const char *donorBlood, const char *recipientBlood) {
    const char *compatibleTable[][2] = {
        {"O-", "O-"},
        {"O+", "O-"},
        {"O+", "O+"},
        {"A-", "A-"},
        {"A-", "O-"},
        {"A+", "A-"},
        {"A+", "O-"},
        {"A+", "A+"},
        {"A+", "O+"},
        {"B-", "B-"},
        {"B-", "O-"},
        {"B+", "B-"},
        {"B+", "O-"},
        {"B+", "B+"},
        {"B+", "O+"},
        {"AB-", "AB-"},
        {"AB-", "A-"},
        {"AB-", "B-"},
        {"AB-", "O-"},
        {"AB+", "AB+"},
        {"AB+", "AB-"},
        {"AB+", "A+"},
        {"AB+", "B+"},
        {"AB+", "A-"},
        {"AB+", "B-"},
        {"AB+", "O-"},
        {"AB+", "O+"}
    };

    int count = sizeof(compatibleTable) / sizeof(compatibleTable[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(donorBlood, compatibleTable[i][0]) == 0 && strcmp(recipientBlood, compatibleTable[i][1]) == 0) {
            return 1;
        }
    }
    return 0;
}

static void addDonor(struct Donor *donors, int *count) {
    if (*count >= MAX_DONORS) {
        printf("Donor list is full.\n");
        return;
    }

    struct Donor donor;
    printf("\n=== Add Donor ===\n");
    printf("ID: ");
    scanf("%d", &donor.id);
    clearInputBuffer();

    printf("Name: ");
    fgets(donor.name, sizeof(donor.name), stdin);
    donor.name[strcspn(donor.name, "\n")] = '\0';

    printf("Age: ");
    scanf("%d", &donor.age);
    clearInputBuffer();

    printf("Gender: ");
    fgets(donor.gender, sizeof(donor.gender), stdin);
    donor.gender[strcspn(donor.gender, "\n")] = '\0';

    printf("Blood Group: ");
    fgets(donor.bloodGroup, sizeof(donor.bloodGroup), stdin);
    donor.bloodGroup[strcspn(donor.bloodGroup, "\n")] = '\0';

    printf("Phone: ");
    fgets(donor.phone, sizeof(donor.phone), stdin);
    donor.phone[strcspn(donor.phone, "\n")] = '\0';

    printf("Location: ");
    fgets(donor.location, sizeof(donor.location), stdin);
    donor.location[strcspn(donor.location, "\n")] = '\0';

    printf("Latitude: ");
    scanf("%lf", &donor.latitude);
    clearInputBuffer();

    printf("Longitude: ");
    scanf("%lf", &donor.longitude);
    clearInputBuffer();

    printf("Days since last donation: ");
    scanf("%d", &donor.lastDonationDays);
    clearInputBuffer();

    printf("Number of donations: ");
    scanf("%d", &donor.donationsCount);
    clearInputBuffer();

    printf("Availability (1=Available, 0=Not Available): ");
    scanf("%d", &donor.isAvailable);
    clearInputBuffer();

    donor.emergencyPriority = detectEmergencyPriority(&donor);
    donors[(*count)++] = donor;
    printf("Donor added successfully.\n");
}

static void viewDonors(const struct Donor *donors, int count) {
    if (count == 0) {
        printf("No donors available.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printDonor(&donors[i]);
    }
}

static void updateDonor(struct Donor *donors, int count) {
    int id;
    printf("Enter donor ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (int i = 0; i < count; i++) {
        if (donors[i].id == id) {
            printf("New phone: ");
            fgets(donors[i].phone, sizeof(donors[i].phone), stdin);
            donors[i].phone[strcspn(donors[i].phone, "\n")] = '\0';

            printf("New location: ");
            fgets(donors[i].location, sizeof(donors[i].location), stdin);
            donors[i].location[strcspn(donors[i].location, "\n")] = '\0';

            printf("New availability (1=Available, 0=Unavailable): ");
            scanf("%d", &donors[i].isAvailable);
            clearInputBuffer();

            printf("Donor updated successfully.\n");
            return;
        }
    }

    printf("Donor with ID %d not found.\n", id);
}

static void deleteDonor(struct Donor *donors, int *count) {
    int id;
    printf("Enter donor ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (int i = 0; i < *count; i++) {
        if (donors[i].id == id) {
            for (int j = i; j < *count - 1; j++) {
                donors[j] = donors[j + 1];
            }
            (*count)--;
            printf("Donor deleted successfully.\n");
            return;
        }
    }

    printf("Donor with ID %d not found.\n", id);
}

static void bloodCompatibilityChecker(void) {
    char donorBlood[6];
    char recipientBlood[6];

    printf("Donor blood group: ");
    scanf("%5s", donorBlood);
    printf("Recipient blood group: ");
    scanf("%5s", recipientBlood);

    if (canDonate(donorBlood, recipientBlood)) {
        printf("Compatible donation.\n");
    } else {
        printf("Not compatible for this blood type.\n");
    }
}

static void searchDonorsByBloodLocation(const struct Donor *donors, int count) {
    char bloodGroup[6];
    char location[60];

    printf("Blood group: ");
    scanf("%5s", bloodGroup);
    printf("Location: ");
    scanf("%59s", location);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(donors[i].bloodGroup, bloodGroup) == 0 && strcasestr(donors[i].location, location) != NULL) {
            printDonor(&donors[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No donors matched the requested blood group and location.\n");
    }
}

static void rankNearbyDonors(const struct Donor *donors, int count) {
    double userLat, userLon;
    printf("User latitude: ");
    scanf("%lf", &userLat);
    printf("User longitude: ");
    scanf("%lf", &userLon);

    struct Donor ranked[MAX_DONORS];
    int idx = 0;

    for (int i = 0; i < count; i++) {
        if (donors[i].latitude != 0.0 && donors[i].longitude != 0.0) {
            ranked[idx++] = donors[i];
            ranked[idx - 1].emergencyPriority = (int)haversineDistanceKm(userLat, userLon, donors[i].latitude, donors[i].longitude);
        }
    }

    for (int i = 0; i < idx; i++) {
        for (int j = i + 1; j < idx; j++) {
            if (ranked[i].emergencyPriority > ranked[j].emergencyPriority) {
                struct Donor temp = ranked[i];
                ranked[i] = ranked[j];
                ranked[j] = temp;
            }
        }
    }

    for (int i = 0; i < idx; i++) {
        printf("Rank %d: %s | Distance: %d km\n", i + 1, ranked[i].name, ranked[i].emergencyPriority);
    }
}

static void bestDonorRecommendation(const struct Donor *donors, int count) {
    char bloodGroup[6];
    printf("Requested blood group: ");
    scanf("%5s", bloodGroup);

    int bestIndex = -1;
    int bestScore = -1;

    for (int i = 0; i < count; i++) {
        if (strcmp(donors[i].bloodGroup, bloodGroup) == 0 && isEligibleDonor(&donors[i])) {
            int score = detectEmergencyPriority(&donors[i]) + donors[i].donationsCount * 5;
            if (score > bestScore) {
                bestScore = score;
                bestIndex = i;
            }
        }
    }

    if (bestIndex == -1) {
        printf("No eligible donor found for %s.\n", bloodGroup);
        return;
    }

    printf("Best donor recommendation:\n");
    printDonor(&donors[bestIndex]);
}

static void printMenu(void) {
    printf("\n=== LifeLink AI Admin Panel ===\n");
    printf("1. Admin Login\n");
    printf("2. Add Donor\n");
    printf("3. View Donors\n");
    printf("4. Update Donor\n");
    printf("5. Delete Donor\n");
    printf("6. Blood Compatibility Checker\n");
    printf("7. Eligibility Checker\n");
    printf("8. Emergency Priority Detection\n");
    printf("9. Best Donor Recommendation\n");
    printf("10. Save and Load Records\n");
    printf("11. Search Donor by Blood Group and Location\n");
    printf("12. Distance-based Donor Ranking\n");
    printf("13. Exit\n");
    printf("Choice: ");
}

int main(void) {
    struct Donor donors[MAX_DONORS];
    int count = 0;
    int loggedIn = 0;
    int choice;

    loadDonorsFromFile(donors, &count);

    while (1) {
        printMenu();
        scanf("%d", &choice);
        clearInputBuffer();

        if (choice == 13) {
            printf("Exiting application.\n");
            break;
        }

        if (choice == 1) {
            loggedIn = loginAdmin();
            continue;
        }

        if (!loggedIn) {
            printf("Please login first.\n");
            continue;
        }

        switch (choice) {
            case 2:
                addDonor(donors, &count);
                saveDonorsToFile(donors, count);
                break;
            case 3:
                viewDonors(donors, count);
                break;
            case 4:
                updateDonor(donors, count);
                saveDonorsToFile(donors, count);
                break;
            case 5:
                deleteDonor(donors, &count);
                saveDonorsToFile(donors, count);
                break;
            case 6:
                bloodCompatibilityChecker();
                break;
            case 7: {
                int donorId;
                printf("Donor ID to check: ");
                scanf("%d", &donorId);
                clearInputBuffer();
                int found = 0;
                for (int i = 0; i < count; i++) {
                    if (donors[i].id == donorId) {
                        printf("Eligibility: %s\n", isEligibleDonor(&donors[i]) ? "Eligible" : "Not eligible");
                        found = 1;
                        break;
                    }
                }
                if (!found) printf("Donor not found.\n");
                break;
            }
            case 8: {
                int donorId;
                printf("Donor ID to evaluate: ");
                scanf("%d", &donorId);
                clearInputBuffer();
                int found = 0;
                for (int i = 0; i < count; i++) {
                    if (donors[i].id == donorId) {
                        printf("Emergency priority score: %d\n", detectEmergencyPriority(&donors[i]));
                        found = 1;
                        break;
                    }
                }
                if (!found) printf("Donor not found.\n");
                break;
            }
            case 9:
                bestDonorRecommendation(donors, count);
                break;
            case 10:
                saveDonorsToFile(donors, count);
                loadDonorsFromFile(donors, &count);
                printf("Records saved and loaded successfully.\n");
                break;
            case 11:
                searchDonorsByBloodLocation(donors, count);
                break;
            case 12:
                rankNearbyDonors(donors, count);
                break;
            default:
                printf("Invalid option.\n");
                break;
        }
    }

    return 0;
}

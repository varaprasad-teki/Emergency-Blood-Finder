#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 100

struct Donor
{
    int id;
    char name[50];
    int age;
    char gender[10];
    char bloodGroup[5];
    char phone[20];
    char location[50];
    int lastDonationDays;
    int donationsCount;
    int distance;
};

struct Donor donors[MAX];
int count = 0;

/* =========================
   ADMIN LOGIN
========================= */

int login()
{
    char username[20];
    char password[20];

    printf("\nUsername : ");
    scanf("%s",username);

    printf("Password : ");
    scanf("%s",password);

    if(
        strcmp(username,"admin")==0 &&
        strcmp(password,"1234")==0
    )
    {
        printf("\nLogin Success\n");
        return 1;
    }

    printf("\nInvalid Login\n");
    return 0;
}

/* =========================
   ADD DONOR
========================= */

void addDonor()
{
    printf("\nID : ");
    scanf("%d",&donors[count].id);

    printf("Name : ");
    scanf("%s",donors[count].name);

    printf("Age : ");
    scanf("%d",&donors[count].age);

    printf("Gender : ");
    scanf("%s",donors[count].gender);

    printf("Blood Group : ");
    

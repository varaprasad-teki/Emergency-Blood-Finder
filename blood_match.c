/*
======================================
 LifeLink AI
 Smart Blood Donation System
======================================
Features
1. Add Donors
2. View Donors
3. Search Blood Group
4. Eligibility Check
5. Compatibility Check
6. Emergency Priority
7. Best Donor Finder
======================================
*/

#include<stdio.h>
#include<string.h>

struct Donor
{
    int id;
    char name[50];
    int age;
    char bloodGroup[5];
    char phone[20];
    char location[50];
    int lastDonationDays;
    int distance;
};

struct Donor donors[100];

int count = 0;

/* Add Donor */

void addDonor()
{
    printf("\nEnter ID: ");
    scanf("%d",&donors[count].id);

    printf("Enter Name: ");
    scanf("%s",donors[count].name);

    printf("Enter Age: ");
    scanf("%d",&donors[count].age);

    printf("Enter Blood Group: ");
    scanf("%s",donors[count].bloodGroup);

    printf("Enter Phone: ");
    scanf("%s",donors[count].phone);

    printf("Enter Location: ");
    scanf("%s",donors[count].location);

    printf("Last Donation Days: ");
    scanf("%d",&donors[count].lastDonationDays);

    printf("Distance From Hospital (KM): ");
    scanf("%d",&donors[count].distance);

    count++;

    printf("\nDonor Added Successfully\n");
}

/* View Donors */

void viewDonors()
{
    int i;

    printf("\n===== DONOR LIST =====\n");

    for(i=0;i<count;i++)
    {
        printf("\nID : %d",donors[i].id);
        printf("\nName : %s",donors[i].name);
        printf("\nAge : %d",donors[i].age);
        printf("\nBlood Group : %s",donors[i].bloodGroup);
        printf("\nPhone : %s",donors[i].phone);
        printf("\nLocation : %s",donors[i].location);
        printf("\n------------------------\n");
    }
}

/* Search Blood Group */

void searchBlood()
{
    char blood[5];
    int i;
    int found=0;

    printf("\nEnter Blood Group: ");
    scanf("%s",blood);

    printf("\nAvailable Donors\n");

    for(i=0;i<count;i++)
    {
        if(strcmp(donors[i].bloodGroup,blood)==0)
        {
            found=1;

            printf("\n%s",donors[i].name);
            printf("\nPhone : %s",donors[i].phone);
            printf("\nLocation : %s\n",donors[i].location);
        }
    }

    if(found==0)
    {
        printf("\nNo Donors Found\n");
    }
}

/* Eligibility Checker */

void checkEligibility()
{
    int days;

    printf("\nLast Donation Days: ");
    scanf("%d",&days);

    if(days>=90)
    {
        printf("\nEligible To Donate\n");
    }
    else
    {
        printf("\nNot Eligible\n");
        printf("Need %d More Days\n",90-days);
    }
}

/* Compatibility Check */

void compatibilityCheck()
{
    char donor[5];

    printf("\nEnter Blood Group: ");
    scanf("%s",donor);

    printf("\nCompatibility Result\n");

    if(strcmp(donor,"O-")==0)
    {
        printf("Universal Donor\n");
        printf("Can Donate To All Groups\n");
    }

    else if(strcmp(donor,"AB+")==0)
    {
        printf("Universal Receiver\n");
    }

    else if(strcmp(donor,"O+")==0)
    {
        printf("Can Donate To O+, A+, B+, AB+\n");
    }

    else
    {
        printf("Standard Compatibility\n");
    }
}

/* Emergency Priority */

void emergencyPriority()
{
    int units;

    printf("\nUnits Required : ");
    scanf("%d",&units);

    if(units>=5)
    {
        printf("\nHIGH EMERGENCY\n");
    }
    else if(units>=3)
    {
        printf("\nMEDIUM EMERGENCY\n");
    }
    else
    {
        printf("\nLOW EMERGENCY\n");
    }
}

/* Best Donor Finder */

void bestDonor()
{
    int i;
    int bestScore=0;
    int bestIndex=-1;
    char patientBlood[5];

    printf("\nPatient Blood Group: ");
    scanf("%s",patientBlood);

    for(i=0;i<count;i++)
    {
        int score=0;

        if(strcmp(
            donors[i].bloodGroup,
            patientBlood)==0)
        {
            score+=100;
        }

        if(donors[i].lastDonationDays>=90)
        {
            score+=30;
        }

        if(donors[i].distance<=5)
        {
            score+=20;
        }

        if(score>bestScore)
        {
            bestScore=score;
            bestIndex=i;
        }
    }

    if(bestIndex!=-1)
    {
        printf("\n===== BEST DONOR =====\n");

        printf("Name : %s\n",
               donors[bestIndex].name);

        printf("Blood Group : %s\n",
               donors[bestIndex].bloodGroup);

        printf("Phone : %s\n",
               donors[bestIndex].phone);

        printf("Score : %d\n",
               bestScore);
    }
    else
    {
        printf("\nNo Matching Donor Found\n");
    }
}

/* Main */

int main()
{
    int choice;

    while(1)
    {
        printf("\n");
        printf("\n===== LIFELINK AI =====");
        printf("\n1.Add Donor");
        printf("\n2.View Donors");
        printf("\n3.Search Blood");
        printf("\n4.Eligibility Check");
        printf("\n5.Compatibility Check");
        printf("\n6.Emergency Priority");
        printf("\n7.Best Donor Finder");
        printf("\n8.Exit");

        printf("\n\nEnter Choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                addDonor();
                break;

            case 2:
                viewDonors();
                break;

            case 3:
                searchBlood();
                break;

            case 4:
                checkEligibility();
                break;

            case 5:
                compatibilityCheck();
                break;

            case 6:
                emergencyPriority();
                break;

            case 7:
                bestDonor();
                break;

            case 8:
                return 0;

            default:
                printf("\nInvalid Choice");
        }
    }

    return 0;
}

/* Name: Gary Hii Ngo Cheong  Student ID: 20502593 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX 40

struct visitData
{
    char *SrNo;
    char *ticketNo;
    char *Name;
    char *ID;
};

// self-referential structure
struct ticket
{
    struct visitData visitor;
    struct ticket *nextPtr;
};

// prototype of functions
void loaddb(struct ticket **head_ref);
void addVisitor(struct ticket **head_ref);
void searchVisitor(struct ticket **head);
void searchByName(struct ticket **head);
void searchByTicketNumber(struct ticket **head);
void searchByID(struct ticket **head);
void dltVisitor1(struct ticket **head_ref);
void dltVisitor2(struct ticket **head_ref);
void dltVisitor3(struct ticket **head);
void append(struct ticket **head_ref, char *passSrNo, char *passTicketNo, char *passName, char *passID);
void delete(struct ticket **head_ref, struct ticket **deleteNode);
void sort(struct ticket *start);
void swap(struct ticket *a, struct ticket *b);
int bigger(char *str1, char *str2);
void printList(struct ticket *node);
void updateTickets(struct ticket **head_ref);
void deleteList(struct ticket **head_ref);
int isElementPresent(int num);
void extractNumber(char inputStr[]);
void printOutputInstruction(char instruction[]);
void printAdd(char name[]);
void printDelete(char instruction[], char item[]);
void printTicket(char srno[], char ticketno[], char name[], char id[]);
void printOverall(struct ticket *node);

bool run = true;                    // determine running of the program
int totalV = 0, ascending = 0, sel; // totalV-- total visitor; sel-- selection of user
int sr_no_list[MAX + 1];            // store useed seriel number

int main(void)
{
    /* Start with the empty list */
    struct ticket *head = NULL;

    while (run == true)
    {
        deleteList(&head); // clear previous database
        loaddb(&head); // load ticket database
        // instruction
        printf("\n### Theme Park Booking System ###\n\n");
        printf("Hi, welcome to Theme Park\nSelect a number for the action that you would like to do:\n\n");

        puts("[1] Add a new visitor information");
        puts("[2] Search visitor information based on Ticket No, Name, or ID");
        puts("[3] Delete the visitor information based on ticket number");
        puts("[4] Delete the visitor information based on full name");
        puts("[5] Delete the visitor information based on ID");
        puts("[6] Display Total No of tickets sold");
        puts("[7] Quit Theme Park Booking System\n");

        printf("Make your selection: ");
        scanf("%d", &sel); // Ask the user to make a selection

        // Determine which action to perform based on the user's selection
        switch (sel)
        {
        case 1:
            // add visitor if visitor less than 40
            if (totalV < 40)
            {
                addVisitor(&head);
            }
            else
            {
                puts("The theme park has reached maximum capacity (40 persons)");
            }
            break;

        case 2:
            // search visitor based on name/ticket/ID
            if (totalV > 0)
            {
                searchVisitor(&head);
            }
            else
            {
                puts("Nobody inside the theme park");
            }
            break;

        case 3:
            // delete visitor if there are more than 0 visitor based on ticket
            if (totalV > 0)
            {
                dltVisitor1(&head);
            }
            else
            {
                puts("Nobody inside the theme park");
            }
            break;

        case 4:
            // delete visitor if there are more than 0 visitor based on name
            if (totalV > 0)
            {
                dltVisitor2(&head);
            }
            else
            {
                puts("Nobody inside the theme park");
            }
            break;

        case 5:
            // delete visitor if there are more than 0 visitor based on ID
            if (totalV > 0)
            {
                dltVisitor3(&head);
            }
            else
            {
                puts("Nobody inside the theme park");
            }
            break;

        case 6:
            ascending = 0;
            puts("Enter 1 if you want to sort the information of visitor in ascending order based on name of visitor;\nEnter any number if you don't want to sort");
            scanf("%d", &ascending); // ask user if the user want a sorted list of visitors
            if (ascending == 1)
            {
                printOutputInstruction("Name in ascending order: true\n");
                sort(head); // sort if user ask to
            }
            else
            {
                printOutputInstruction("Name in ascending order: false\n");
            }

            printf("\nTotal visitors: %d", totalV);
            // print to output
            printList(head);
            printOverall(head);
            deleteList(&head); // restore linked list
            break;

        case 7:
            printf("Bye~~");
            printOutputInstruction("Exit system\n"); // print to output.txt
            run = false;                             // exit program
            break;

        default:
            printf("\nYou did not make a valid selection.\n");
            break;
        }
    }

    return 0;
}

// load database
void loaddb(struct ticket **head_ref)
{
    FILE *filePtr = NULL; // filePtr = Tickets.txt file pointer
    struct visitData data;
    // fopen open file; exit program if file cannot be openned
    if ((filePtr = fopen("Tickets.txt", "r")) == NULL)
    {
        puts("File could not be opened");
    }
    else
    {

        fscanf(filePtr, "\nTotal visitors: %d", &totalV); // skip first line

        for (int i = 0; i < totalV; i++)
        {
            char SrNo[3];
            char ticketNo[31];
            char Name[31];
            char IDNo[31];
            memset(SrNo, 0, 3);
            memset(ticketNo, 0, 31);
            memset(Name, 0, 31);
            memset(IDNo, 0, 31);

            fscanf(filePtr, "%s%s%s%s", SrNo, ticketNo, Name, IDNo); // scan data in line

            append(head_ref, SrNo, ticketNo, Name, IDNo); // add to ticket data list
            sr_no_list[i] = atoi(SrNo);                   // add seriel number (in integer) to a list
        }

        fclose(filePtr); // fclose closes the file
    }
}

// add new visitor
void addVisitor(struct ticket **head_ref)
{
    char SrNo[3];
    char ticketNo[31];
    char Name[31];
    char IDNo[31];
    char ID[31];
    memset(SrNo, 0, 3);
    memset(ticketNo, 0, 31);
    memset(Name, 0, 31);
    memset(IDNo, 0, 31);
    memset(ID, 0, 31);

    int local = 0;

    puts("\n### New Visitor ###\n");
    printf("Name: "); // instruction
    fflush(stdin);
    fgets(Name, 30, stdin);
    Name[strcspn(Name, "\n")] = '\0'; // clear the "\n" character in the end of string after fgets()

    printf("\nWelcome, enter\n");
    puts("[1] if you are local visitor");
    puts("[2] if you are international visitor"); // instruction
    printf("1/2: ");                              // instruction
    scanf("%d", &local);                          // get visitor type
    while ((local != 1) && (local != 2))          // repeat until user has entered a valid input
    {
        printf("Sorry, you have enter an invalid option, please enter a valid input\n");
        puts("[1] if you are local visitor");
        puts("[2] if you are international visitor"); // instruction
        scanf("%d", &local);                          // get visitor type
    }
    if (local == 1)
    {
        printf("Please enter your MyKad number\nMyKad No.: ");
    }
    else
    {
        printf("Please enter your Passport number\nPassport No.: ");
    }
    fflush(stdin);
    fgets(IDNo, 18, stdin);           // get visitor's id
    IDNo[strcspn(IDNo, "\n")] = '\0'; // clear the "\n" character in the end of string after fgets()

    // generate seriel number
    int sr_no = 1;
    if (totalV != 0)
    {
        for (int i = 0; i < totalV; i++)
        {
            while (isElementPresent(sr_no))
            {
                sr_no++;
            }
        }
    }

    // convert integer to string
    if (sr_no < 10)
    {
        char temp[3];
        sprintf(temp, "0%d", sr_no);
        strcpy(SrNo, temp);
    }
    else
    {
        char temp[3];
        sprintf(temp, "%d", sr_no);
        strcpy(SrNo, temp);
    }
    // seriel number is generated and stored

    // generate ticket number
    char ticketName[7]; // store first 6 character of name
    char character[4];  // store each converted character

    int length;
    // record string length for name
    for (length = 0; Name[length] != '\0'; length++)
    {
        ;
    }

    // if visitor's name less than 6 alphabets, append space until there are 6 character in the string
    if (length < 6) //**the last element in the name is not a character
    {
        int i;
        for (i = 0; i < 6 - length; i++)
        {
            ticketName[i] = ' '; // append space
        }
        for (int j = 0; j < length; i++, j++)
        {
            ticketName[i] = Name[j];
        }
    }
    else
    {
        strncat(ticketName, Name, 6); // copy first 6 character
    }

    // convert first 6 character of name to decimal
    for (int i = 0; i < 6; i++)
    {
        int N = (int)ticketName[i];
        sprintf(character, "%d", N);
        strcat(ticketNo, character);
    }

    strcat(ticketNo, SrNo);
    // ticket number is generated and stored

    // generate formatted name
    int i = 0;

    // check every character in the name and replace ' ' with '_'
    for (; Name[i] != '\0'; i++)
    {
        if (isspace(Name[i]))
        {
            if (Name[i + 1] != '\0')
            {
                Name[i] = '_';
            }
            else
            {
                Name[i] = '\0'; // remove the space character in the last element of the string
            }
        }
    }
    // Name is modified

    // generate formatted id
    if (local == 1)
    {
        strcpy(ID, "IC"); // for local visitor
    }
    else
    {
        strcpy(ID, "PASS"); // for international visitor
    }

    // generate a formatted id
    extractNumber(IDNo);

    strcat(ID, IDNo);
    // formatted id is generated

    totalV++; // total visitor increase
    printf("\nThe information of the visitor %s is recorded.\n", Name);
    // print to output.txt
    printAdd(Name);
    printOverall(*head_ref);
    printTicket(SrNo, ticketNo, Name, ID);
    // update ticket.txt
    append(head_ref, SrNo, ticketNo, Name, ID);
    updateTickets(head_ref);
}

// search visitor
void searchVisitor(struct ticket **head)
{
    int search; // record type of search user want
    puts("\n### Search Visitor ###\n");
    printf("Select a number for the action that you would like to do:\n\n");

    puts("[1] Search visitor based on name");
    puts("[2] Search visitor based on ticket number");
    puts("[3] Search visitor based on ID\n");

    printf("Make your selection: ");
    scanf("%d", &search); // Ask the user to make a selection

    // Determine which action to perform based on the user's selection
    switch (search)
    {
    case 1:
        // search based on name
        searchByName(head);
        break;

    case 2:
        // search based on ticket no
        searchByTicketNumber(head);
        break;
    case 3:
        // search based on id
        searchByID(head);
        break;
    default:
        printf("\nYou did not make a valid selection.\n");
        break;
    }
}

// search visitor based on full name
void searchByName(struct ticket **head)
{
    int repeat = 0; // record how many matched name
    char Name[31];
    memset(Name, 0, 31);

    printf("Name: "); // instruction
    fflush(stdin);
    fgets(Name, 30, stdin);
    Name[strcspn(Name, "\n")] = '\0'; // clear the "\n" character in the end of string after fgets()

    puts("The following record of the visitor is found: ");
    printOutputInstruction("The following record of the visitor is found: \n");

    int lenght = strlen(Name); // store length of name
    int i = 0;

    // check every character in the name and replace ' ' with '_'
    for (; Name[i] != '\0'; i++)
    {
        if (isspace(Name[i]))
        {
            if (Name[i + 1] != '\0')
            {
                Name[i] = '_';
            }
            else
            {
                Name[i] = '\0'; // remove the space character in the last element of the string
            }
        }
        else
        {
            Name[i] = tolower(Name[i]);
        }
    }

    struct ticket *current = *head; // Initialize current
    while (current != NULL)
    {
        char ticketName[30];
        memset(ticketName, 0, 30);
        strcpy(ticketName, current->visitor.Name);
        i = 0;
        // check every character in the name and replace ' ' with '_'
        for (; ticketName[i] != '\0'; i++)
        {
            ticketName[i] = tolower(ticketName[i]);
        }

        // if name is match with user's input
        if (!strcmp(ticketName, Name))
        {
            printf("%s %s %s %s\n", current->visitor.SrNo, current->visitor.ticketNo, current->visitor.Name, current->visitor.ID);
            // print to output.txt
            printTicket(current->visitor.SrNo, current->visitor.ticketNo, current->visitor.Name, current->visitor.ID);
            printOverall(*head);
            repeat++;
        }

        current = current->nextPtr;
    }
    // no visitor matched
    if (repeat == 0)
    {
        printf("\nVisitor does not exist\n");
        // print to output.txt
        printOutputInstruction("Visitor does not exist\n");
        printOverall(*head);
    }

    return;
}

// search visitor based on ticket number
void searchByTicketNumber(struct ticket **head)
{
    int found = 0; // record if there is any visitor matched
    char ticketNo[31];
    memset(ticketNo, 0, 31);
    printf("Ticket Number: "); // instruction
    fflush(stdin);
    fgets(ticketNo, 30, stdin);
    ticketNo[strcspn(ticketNo, "\n")] = '\0'; // clear the "\n" character in the end of string after fgets()

    puts("The following record of the visitor is found: ");
    printOutputInstruction("The following record of the visitor is found: \n");

    struct ticket *current = *head; // Initialize current
    while (current != NULL)
    {
        // if there is any visitor's ticket matched
        if (!strcmp(ticketNo, current->visitor.ticketNo))
        {
            printf("%s %s %s %s\n", current->visitor.SrNo, current->visitor.ticketNo, current->visitor.Name, current->visitor.ID);
            // print to output.txt
            printTicket(current->visitor.SrNo, current->visitor.ticketNo, current->visitor.Name, current->visitor.ID);
            printOverall(*head);
            found = 1;
            break;
        }

        current = current->nextPtr;
    }
    // no visitor's ticket matched
    if (found == 0)
    {
        printf("\nVisitor does not exist\n");
        // print to output.txt
        printOutputInstruction("Visitor does not exist\n");
        printOverall(*head);
    }

    return;
}

// search visitor based on ID
void searchByID(struct ticket **head)
{
    int local;
    int found = 0; // record is visitor's id is found
    char IDNo[31];
    char ID[31];
    memset(IDNo, 0, 31);
    puts("Enter");
    puts("[1] if you are local visitor");
    puts("[2] if you are international visitor"); // instruction
    printf("1/2: ");                              // instruction
    scanf("%d", &local);                          // get visitor type
    while ((local != 1) && (local != 2))          // repeat until user has entered a valid input
    {
        printf("Sorry, you have enter an invalid option, please enter a valid input\n");
        puts("[1] if you are local visitor");
        puts("[2] if you are international visitor"); // instruction
        scanf("%d", &local);                          // get visitor type
    }
    if (local == 1)
    {
        printf("Please enter your MyKad number\nMyKad No.: ");
    }
    else
    {
        printf("Please enter your Passport number\nPassport No.: ");
    }
    fflush(stdin);
    fgets(IDNo, 18, stdin);           // get visitor's id
    IDNo[strcspn(IDNo, "\n")] = '\0'; // clear the "\n" character in the end of string after fgets()

    // generate formatted id
    if (local == 1)
    {
        strcpy(ID, "IC"); // for local visitor
    }
    else
    {
        strcpy(ID, "PASS"); // for international visitor
    }

    // generate a formatted id
    extractNumber(IDNo);

    strcat(ID, IDNo);
    // formatted id is generated

    puts("The following record of the visitor is found: ");
    printOutputInstruction("The following record of the visitor is found: \n");

    struct ticket *current = *head; // Initialize current
    while (current != NULL)
    {
        // if visitor's id matched
        if (!strcmp(ID, current->visitor.ID))
        {
            printf("%s %s %s %s\n", current->visitor.SrNo, current->visitor.ticketNo, current->visitor.Name, current->visitor.ID);
            // print to output.txt
            printTicket(current->visitor.SrNo, current->visitor.ticketNo, current->visitor.Name, current->visitor.ID);
            printOverall(*head);
            found = 1;
            break;
        }

        current = current->nextPtr;
    }
    // no visitor id matched
    if (found == 0)
    {
        printf("\nVisitor does not exist\n");
        // print to output.txt
        printOutputInstruction("Visitor does not exist\n");
        printOverall(*head);
    }
    return;
}

// delete visitor based on ticket number
void dltVisitor1(struct ticket **head)
{
    int found = 0; // record if there is any visitor's ticket matched
    char ticketNo[31];
    memset(ticketNo, 0, 31);
    printf("Ticket Number: "); // instruction
    fflush(stdin);
    fgets(ticketNo, 30, stdin);
    ticketNo[strcspn(ticketNo, "\n")] = '\0'; // clear the "\n" character in the end of string after fgets()

    struct ticket *current = *head; // Initialize current
    while (current != NULL)
    {
        // if there is any visitor's ticket matched
        if (!strcmp(ticketNo, current->visitor.ticketNo))
        {
            printf("\nThe visitor with Ticket No.: %s, information is deleted.\n", ticketNo);
            delete (head, &current); // delete from linked list
            totalV--;                // total visitor reduced by 1
            // print to output.txt
            printDelete("The visitor with Ticket No.", ticketNo);
            printOverall(*head);
            updateTickets(head); // update ticket.txt

            found = 1;
            break;
        }

        current = current->nextPtr;
    }
    // no ticket matched
    if (found == 0)
    {
        printf("\nVisitor does not exist\n");
        // print to output.txt
        printOutputInstruction("Visitor does not exist\n");
        printOverall(*head);
    }
}

// delete visitor based on full name
void dltVisitor2(struct ticket **head)
{
    int repeat = 0; // record if there is repeated name
    char Name[31];
    memset(Name, 0, 31);

    printf("Name: "); // instruction
    fflush(stdin);
    fgets(Name, 30, stdin);
    Name[strcspn(Name, "\n")] = '\0'; // clear the "\n" character in the end of string after fgets()

    int lenght = strlen(Name); // store length of name
    int i = 0;

    // check every character in the name and replace ' ' with '_'; changed all the character to small letter
    for (; Name[i] != '\0'; i++)
    {
        if (isspace(Name[i]))
        {
            if (Name[i + 1] != '\0')
            {
                Name[i] = '_';
            }
            else
            {
                Name[i] = '\0'; // remove the space character in the last element of the string
            }
        }
        else
        {
            Name[i] = tolower(Name[i]);
        }
    }

    struct ticket *current = *head; // Initialize current
    struct ticket *dlt;             // To store memory location of the node to be deleted
    while (current != NULL)
    {
        char ticketName[30];
        memset(ticketName, 0, 30);
        strcpy(ticketName, current->visitor.Name);
        i = 0;
        // convert all character to small letter
        for (; ticketName[i] != '\0'; i++)
        {
            if (isspace(ticketName[i]))
            {
                if (ticketName[i + 1] != '\0')
                {
                    ticketName[i] = '_';
                }
                else
                {
                    ticketName[i] = '\0'; // remove the space character in the last element of the string
                }
            }
            else
            {
                ticketName[i] = tolower(ticketName[i]);
            }
        }

        if (!strcmp(ticketName, Name))
        {
            dlt = current;
            repeat++;
        }

        current = current->nextPtr;
    }
    // if no name matched
    if (repeat == 0)
    {
        printf("\nVisitor does not exist\n");
        // print to output.txt
        printOutputInstruction("Visitor does not exist\n");
        printOverall(*head);
    }
    // one person's name matched
    else if (repeat == 1)
    {
        printf("\nThe visitor with Name %s, information is deleted.\n", dlt->visitor.Name);
        totalV--; // total visitor reduced by 1
        // print to output.txt
        printDelete("The visitor with Name", dlt->visitor.Name);
        delete (head, &dlt); // delete visitor's information from linked list
        printOverall(*head);
        updateTickets(head); // update ticket.txt
    }
    // more than 1 name matched
    else
    {
        puts("The following visitors have same names, Please enter the ID to deleted:");
        // print to output.txt
        printOutputInstruction("The following visitors have same names, Please enter the ID to deleted: \n");
        struct ticket *repeatingName = *head; // Initialize current

        // print out all repeated name
        while (repeatingName != NULL)
        {
            char repeatName[30];
            memset(repeatName, 0, 30);
            strcpy(repeatName, repeatingName->visitor.Name);
            i = 0;
            // convert to samll letter
            for (; repeatName[i] != '\0'; i++)
            {
                if (isspace(repeatName[i]))
                {
                    if (repeatName[i + 1] != '\0')
                    {
                        repeatName[i] = '_';
                    }
                    else
                    {
                        repeatName[i] = '\0'; // remove the space character in the last element of the string
                    }
                }
                else
                {
                    repeatName[i] = tolower(repeatName[i]);
                }
            }
            if (!strcmp(repeatName, Name))
            {
                printf("%s %s %s %s\n", repeatingName->visitor.SrNo, repeatingName->visitor.ticketNo, repeatingName->visitor.Name, repeatingName->visitor.ID);
                printTicket(repeatingName->visitor.SrNo, repeatingName->visitor.ticketNo, repeatingName->visitor.Name, repeatingName->visitor.ID);
            }

            repeatingName = repeatingName->nextPtr;
        }
        dltVisitor3(head); // delete visitor based on id
    }

    return;
}

// delete visitor based on ID
void dltVisitor3(struct ticket **head)
{
    int local;
    int found = 0; // record whether any id matched
    char IDNo[31];
    char ID[31];
    memset(IDNo, 0, 31);
    puts("Enter");
    puts("[1] if you are local visitor");
    puts("[2] if you are international visitor"); // instruction
    printf("1/2: ");                              // instruction
    scanf("%d", &local);                          // get visitor type
    while ((local != 1) && (local != 2))          // repeat until user has entered a valid input
    {
        printf("Sorry, you have enter an invalid option, please enter a valid input\n");
        puts("[1] if you are local visitor");
        puts("[2] if you are international visitor"); // instruction
        scanf("%d", &local);                          // get visitor type
    }
    if (local == 1)
    {
        printf("Please enter your MyKad number\nMyKad No.: ");
    }
    else
    {
        printf("Please enter your Passport number\nPassport No.: ");
    }
    fflush(stdin);
    fgets(IDNo, 18, stdin);           // get visitor's id
    IDNo[strcspn(IDNo, "\n")] = '\0'; // clear the "\n" character in the end of string after fgets()

    // generate formatted id
    if (local == 1)
    {
        strcpy(ID, "IC"); // for local visitor
    }
    else
    {
        strcpy(ID, "PASS"); // for international visitor
    }

    // generate a formatted id
    extractNumber(IDNo);

    strcat(ID, IDNo);
    // formatted id is generated

    struct ticket *current = *head; // Initialize current
    while (current != NULL)
    {
        // if id matched
        if (!strcmp(ID, current->visitor.ID))
        {
            printf("\nThe visitor with ID.: %s, information is deleted.\n", ID);
            delete (head, &current); // deleted visitor's information from linked list
            totalV--;                // total visitor reduced by 1
            // print to output.txt
            printDelete("The visitor with ID.", ID);
            printOverall(*head);
            updateTickets(head); // update ticket.txt2
            found = 1;
            break;
        }

        current = current->nextPtr;
    }
    // if no id matched
    if (found == 0)
    {
        printf("\nVisitor does not exist\n");
        // print to output.txt
        printOutputInstruction("Visitor does not exist\n");
        printOverall(*head);
    }
}

// insert a new visitor into ticket data list
void append(struct ticket **head_ref, char *passSrNo, char *passTicketNo, char *passName, char *passID)
{
    // allocate node
    struct ticket *new_node = (struct ticket *)malloc(sizeof(struct ticket));

    struct ticket *last = *head_ref;

    // put in the data
    new_node->visitor.SrNo = malloc(strlen(passSrNo) + 1);
    new_node->visitor.ticketNo = malloc(strlen(passTicketNo) + 1);
    new_node->visitor.Name = malloc(strlen(passName) + 1);
    new_node->visitor.ID = malloc(strlen(passID) + 1);

    // copy the current word
    strcpy(new_node->visitor.SrNo, passSrNo);
    strcpy(new_node->visitor.ticketNo, passTicketNo);
    strcpy(new_node->visitor.Name, passName);
    strcpy(new_node->visitor.ID, passID);

    // This new node is the last node
    new_node->nextPtr = NULL;

    // If the Linked List is empty, then make the new node as head
    if (*head_ref == NULL)
    {
        *head_ref = new_node;
        return;
    }

    // Else traverse till the last node */
    while (last->nextPtr != NULL)
    {
        last = last->nextPtr;
    }

    // Change the next of last node
    last->nextPtr = new_node;

    return;
}

// delete visitor from ticket data list
void delete(struct ticket **head_ref, struct ticket **deleteNode)
{
    struct ticket *current = *head_ref;

    if (current == *deleteNode) // when delete the first node
    {
        struct ticket *temp = current->nextPtr;
        current->nextPtr = NULL;
        free(current->visitor.SrNo);
        free(current->visitor.ticketNo);
        free(current->visitor.Name);
        free(current->visitor.ID);
        free(current); // Node is deleted
        *head_ref = temp;
    }
    else
    {
        while (current->nextPtr != *deleteNode) // move pointer to the node before the deleting node
        {
            current = current->nextPtr;
        }
        // Now temp pointer points to the previous node of the node to be deleted
        struct ticket *temp = current->nextPtr;
        current->nextPtr = current->nextPtr->nextPtr;
        free(temp->visitor.SrNo);
        free(temp->visitor.ticketNo);
        free(temp->visitor.Name);
        free(temp->visitor.ID);
        free(temp); // Node is deleted
    }
}

// sort the string in ascending order
void sort(struct ticket *start)
{
    int swapped;
    struct ticket *ptr1;
    struct ticket *ptr2;
    struct ticket *lptr = NULL;

    /* Checking for empty list */
    if (start == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = start;

        while (ptr1->nextPtr != lptr)
        {
            ptr2 = ptr1->nextPtr;
            char name1[31];
            char name2[31];
            strcpy(name1, ptr1->visitor.Name);
            strcpy(name2, ptr2->visitor.Name);

            // if name1 is bigger than name 2, then swap
            if (bigger(name1, name2))
            {
                swap(ptr1, ptr1->nextPtr);
                swapped = 1;
            }
            ptr1 = ptr1->nextPtr;
        }
        lptr = ptr1;
    } while (swapped);
}

// swap data of two nodes a and b
void swap(struct ticket *a, struct ticket *b)
{
    struct visitData temp = a->visitor;
    a->visitor = b->visitor;
    b->visitor = temp;
}

// return true when the first string is bigger than second
int bigger(char *str1, char *str2)
{
    int bigger = 0;
    int i = 0;
    while (tolower(str1[i]) == tolower(str2[i]))
    {
        i++;
    }
    if (tolower(str1[i]) > tolower(str2[i]))
    {
        bigger = 1;
    }

    return bigger;
}

// This function prints contents of linked list starting from head
void printList(struct ticket *node)
{
    while (node != NULL)
    {
        printf("\n%s ", node->visitor.SrNo);
        printf(" %s ", node->visitor.ticketNo);
        printf(" %s ", node->visitor.Name);
        printf(" %s", node->visitor.ID);

        node = node->nextPtr;
    }
}

// update tickets.txt
void updateTickets(struct ticket **head_ref)
{
    FILE *fPtr = NULL; // fPtr = Tickets.txt file pointer
    struct ticket *last = *head_ref;
    if ((fPtr = fopen("Tickets.txt", "w")) == NULL)
    {
        puts("File could not be opened");
    }
    else
    {
        fprintf(fPtr, "Total visitors: %2d\n", totalV); // print the first line of Tickets.txt; rewrite Tickets.txt according to visitor data list
        while (last != NULL)
        {
            fprintf(fPtr, "%s %s %s %s\n", last->visitor.SrNo, last->visitor.ticketNo, last->visitor.Name, last->visitor.ID); // print data in each node
            last = last->nextPtr;
        }
        fclose(fPtr); // close the file
        deleteList(head_ref);
    }
}

// free all the memory of the list
void deleteList(struct ticket **head_ref)
{
    struct ticket *current = *head_ref;
    struct ticket *next;

    while (current != NULL)
    {
        next = current->nextPtr;
        free(current->visitor.SrNo);
        free(current->visitor.ticketNo);
        free(current->visitor.Name);
        free(current->visitor.ID);
        free(current);
        current = next;
    }

    *head_ref = NULL;
}

// check seriel number exist or not
int isElementPresent(int x)
{
    int arrLen = sizeof sr_no_list / sizeof sr_no_list[0];
    int isElementPresent = 0;

    for (int i = 0; i < arrLen; i++)
    {
        if (sr_no_list[i] == x)
        {
            isElementPresent = 1;
            break;
        }
    }

    if (isElementPresent)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// extract number from ID that may include other character
void extractNumber(char inputStr[])
{
    int i = 0;

    int j = 0;

    for (i = 0; inputStr[i]; i++)
    {
        if (inputStr[i] >= '0' && inputStr[i] <= '9')
        {
            inputStr[j] = inputStr[i];
            j++;
        }
    }

    inputStr[j] = '\0';
}

// print instruction to output.txt
void printOutputInstruction(char instruction[])
{
    FILE *outPtr = NULL; // outPtr = output.txt file pointer
    if ((outPtr = fopen("output.txt", "a")) == NULL)
    {
        puts("File could not be opened");
    }
    else
    {
        fprintf(outPtr, "\n%s", instruction); // print the line to output.txt;
        fclose(outPtr);                       // close the file
    }
    fclose(outPtr); // close the file
}

// print added visitor to output.txt
void printAdd(char name[])
{
    FILE *outPtr = NULL; // outPtr = output.txt file pointer
    if ((outPtr = fopen("output.txt", "a+")) == NULL)
    {
        puts("File could not be opened");
    }
    else
    {
        fprintf(outPtr, "\nThe information of the visitor %s is recorded.", name); // print the line to output.txt;
    }
    fclose(outPtr); // close the file
}

// print deleted ticket information to output.txt
void printDelete(char instruction[], char item[])
{
    FILE *outPtr = NULL; // outPtr = output.txt file pointer
    if ((outPtr = fopen("output.txt", "a")) == NULL)
    {
        puts("File could not be opened");
    }
    else
    {
        fprintf(outPtr, "\n%s: %s, information is deleted.\n", instruction, item); // print the line to output.txt;
    }
    fclose(outPtr); // close the file
}

// print ticket information to output.txt
void printTicket(char srno[], char ticketno[], char name[], char id[])
{
    FILE *outPtr = NULL; // outPtr = output.txt file pointer
    if ((outPtr = fopen("output.txt", "a")) == NULL)
    {
        puts("File could not be opened");
    }
    else
    {
        fprintf(outPtr, "%s %s %s %s\n", srno, ticketno, name, id); // print the line to output.txt;
    }
    fclose(outPtr); // close the file
}

// print ticket.txt to output.txt
void printOverall(struct ticket *node)
{
    FILE *outPtr = NULL; // outPtr = output.txt file pointer
    if ((outPtr = fopen("output.txt", "a")) == NULL)
    {
        puts("File could not be opened");
    }
    else
    {
        fprintf(outPtr, "\nTotal visitors: %d\n", totalV); // print the line to output.txt;
        while (node != NULL)
        {
            fprintf(outPtr, "%s %s %s %s \n", node->visitor.SrNo, node->visitor.ticketNo, node->visitor.Name, node->visitor.ID); // print the line to output.txt;
            node = node->nextPtr;
        }
    }
    fclose(outPtr); // close the file
}
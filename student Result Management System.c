#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50], id[20];
    float m[5], total, avg;
    char grade;
} Student;

// total mark , average and the grade
Student calc(Student s){
    s.total = 0;

    for(int i = 0; i < 5; i++)
        s.total += s.m[i];

    s.avg = s.total / 5;

    if(s.avg >= 80) s.grade = 'A';
    else if(s.avg >= 70) s.grade = 'B';
    else if(s.avg >= 60) s.grade = 'C';
    else if(s.avg >= 50) s.grade = 'D';
    else s.grade = 'F';

    return s;
}

// Save information
void save(Student *list, int n){
    FILE *f = fopen("data.bin", "wb");
    fwrite(&n, sizeof(int), 1, f);
    fwrite(list, sizeof(Student), n, f);
    fclose(f);
}

// Load information
int load(Student **list, int *n){
    FILE *f = fopen("data.bin", "rb");
    if(!f) return 0;

    fread(n, sizeof(int), 1, f);
    *list = malloc(*n * sizeof(Student));
    fread(*list, sizeof(Student), *n, f);
    fclose(f);
    return 1;
}

int main(){
    Student *list = NULL;
    int n = 0, ch;

    load(&list, &n);

    do{
        printf("\n1. Add Student\n2. Show Students\n3. Search by ID\n4. Edit Student\n5. Delete Student\n6. Save & Exit\nChoice: ");
        scanf("%d", &ch);

        // Add
        if(ch == 1){
            list = realloc(list, (n + 1) * sizeof(Student));
            Student s;

            printf("Name: ");
            scanf(" %[^\n]", s.name);

            printf("ID: ");
            scanf("%s", s.id);

            for(int i = 0; i < 5; i++){
                printf("Mark %d: ", i + 1);
                scanf("%f", &s.m[i]);
            }

            s = calc(s);
            list[n] = s;
            n++;

            printf("Student Added!\n");
        }

        // Show
        else if(ch == 2){
            if(n == 0) printf("\nNo records.\n");
            else{
                printf("\n%-4s %-20s %-12s %-10s %-10s %-6s\n",
                    "No", "Name", "ID", "Total", "Average", "Grade");
                printf("--------------------------------------------------------------------\n");

                for(int i=0; i<n; i++){
                    printf("%-4d %-20s %-12s %-10.1f %-10.1f %-6c\n",
                        i+1,
                        list[i].name,
                        list[i].id,
                        list[i].total,
                        list[i].avg,
                        list[i].grade
                    );
                }
            }
        }

        // Search
        else if(ch == 3){
            if(n == 0){ printf("No data.\n"); continue; }

            char sid[20];
            printf("Enter ID: ");
            scanf("%s", sid);

            int found = 0;
            for(int i = 0; i < n; i++){
                if(strcmp(list[i].id, sid) == 0){
                    printf("\nStudent Found:\n");
                    printf("Name: %s\nID: %s\nTotal: %.1f\nAverage: %.1f\nGrade: %c\n",
                        list[i].name,
                        list[i].id,
                        list[i].total,
                        list[i].avg,
                        list[i].grade
                    );
                    found = 1;
                    break;
                }
            }
            if(!found) printf("Not Found!\n");
        }

        // Edit
        else if(ch == 4){
            if(n == 0){ printf("No data.\n"); continue; }

            int idx;
            printf("Enter student number to edit: ");
            scanf("%d", &idx);
            idx--;

            if(idx < 0 || idx >= n){
                printf("Invalid number!\n");
                continue;
            }

            Student s = list[idx];

            printf("New Name: ");
            scanf(" %[^\n]", s.name);

            printf("New ID: ");
            scanf("%s", s.id);

            for(int i=0; i<5; i++){
                printf("New Mark %d: ", i+1);
                scanf("%f", &s.m[i]);
            }

            s = calc(s);   // recalc
            list[idx] = s;

            printf("Updated!\n");
        }

        // Delete
        else if(ch == 5){
            if(n == 0){ printf("No data.\n"); continue; }

            int idx;
            printf("Enter number to delete: ");
            scanf("%d", &idx);
            idx--;

            if(idx < 0 || idx >= n){
                printf("Invalid number!\n");
                continue;
            }

            for(int i = idx; i < n - 1; i++)
                list[i] = list[i + 1];

            n--;
            list = realloc(list, n * sizeof(Student));
            printf("Deleted!\n");
        }

    } while(ch != 6);

    save(list, n);
    free(list);

    printf("Saved & Exit\n");
    return 0;
}

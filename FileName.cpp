#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 100
#define MAX_USERS 100
#define MAX_ADMINS 10
#define MAX_RECORDS 100

// ͼ����Ϣ�ṹ��
typedef struct {
    int id;
    char title[100];
    char author[100];
    int available; // 1: �ɽ� 0: �ѽ�
} Book;

// �û���Ϣ�ṹ��
typedef struct {
    int id;
    char username[50];
    char password[50];
} User;

// ����Ա��Ϣ�ṹ��
typedef struct {
    int id;
    char username[50];
    char password[50];
} Admin;

// �����¼�ṹ��
typedef struct {
    int book_id;
    int user_id;
    char borrow_date[20];
} BorrowRecord;

Book books[MAX_BOOKS];
User users[MAX_USERS];
Admin admins[MAX_ADMINS];
BorrowRecord borrow_records[MAX_RECORDS];

// ȫ�ֱ���
int num_books = 0, num_users = 0, num_admins = 0, num_borrow_records = 0;

// ��������
void load_data();
void save_data();
void login_user();
void login_admin();
void user_menu(User* user);
void admin_menu();
void search_book();
void display_books();
void borrow_book(User* user);
void return_book(User* user);
void add_book();
void remove_book();
void modify_book();
void view_borrow_records();
void register_user();  // ע���û�
void exit_program();   // �˳�����

// ������
int main() {
    load_data();

    int choice;
    while (1) {
        printf("1. �û���¼\n");
        printf("2. ����Ա��¼\n");
        printf("3. �û�ע��\n");
        printf("0. �˳�ϵͳ\n");
        printf("��ѡ�����: ");
        scanf("%d", &choice);

        if (choice == 1) {
            login_user();
        }
        else if (choice == 2) {
            login_admin();
        }
        else if (choice == 3) {
            register_user();
        }
        else if (choice == 0) {
            exit_program();
            break;  // �˳�ϵͳ
        }
        else {
            printf("��Ч��ѡ��\n");
        }
    }

    save_data();
    return 0;
}

// ��������
void load_data() {
    FILE* file;

    // ��ȡͼ����Ϣ
    file = fopen("books.txt", "r");
    if (file == NULL) {
        printf("�޷���ͼ���ļ���\n");
        // ����޷�����ͼ����Ϣ����Ȼ�������������������
    }
    else {
        while (fscanf(file, "%d,%99[^,],%99[^,],%d\n", &books[num_books].id, books[num_books].title, books[num_books].author, &books[num_books].available) != EOF) {
            num_books++;
        }
        fclose(file);
    }

    // ��ȡ�û���Ϣ
    file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("�޷����û��ļ���\n");
        return;
    }
    while (fscanf(file, "%d,%49[^,],%49[^\n]\n", &users[num_users].id, users[num_users].username, users[num_users].password) != EOF) {
        num_users++;
    }
    fclose(file);

    // ��ȡ����Ա��Ϣ
    file = fopen("admins.txt", "r");
    if (file == NULL) {
        printf("�޷��򿪹���Ա�ļ���\n");
        return;
    }
    while (fscanf(file, "%d,%49[^,],%49[^\n]\n", &admins[num_admins].id, admins[num_admins].username, admins[num_admins].password) != EOF) {
        num_admins++;
    }
    fclose(file);

    // ��ȡ�����¼
    file = fopen("borrow_records.txt", "r");
    if (file == NULL) {
        printf("�޷��򿪽����¼�ļ���\n");
        return;
    }
    while (fscanf(file, "%d,%d,%d,%19[^\n]\n", &borrow_records[num_borrow_records].book_id, &borrow_records[num_borrow_records].user_id, &borrow_records[num_borrow_records].user_id, borrow_records[num_borrow_records].borrow_date) != EOF) {
        num_borrow_records++;
    }
    fclose(file);
}


// ��������
void save_data() {
    FILE* file;

    // ����ͼ����Ϣ
    file = fopen("books.txt", "w");
    if (file == NULL) {
        printf("�޷���ͼ���ļ��Ա������ݣ�\n");
        return;
    }
    for (int i = 0; i < num_books; i++) {
        fprintf(file, "%d,%s,%s,%d\n", books[i].id, books[i].title, books[i].author, books[i].available);
    }
    fclose(file);

    // �����û���Ϣ
    file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("�޷����û��ļ��Ա������ݣ�\n");
        return;
    }
    for (int i = 0; i < num_users; i++) {
        fprintf(file, "%d,%s,%s\n", users[i].id, users[i].username, users[i].password);
    }
    fclose(file);

    // �������Ա��Ϣ
    file = fopen("admins.txt", "w");
    if (file == NULL) {
        printf("�޷��򿪹���Ա�ļ��Ա������ݣ�\n");
        return;
    }
    for (int i = 0; i < num_admins; i++) {
        fprintf(file, "%d,%s,%s\n", admins[i].id, admins[i].username, admins[i].password);
    }
    fclose(file);

    // ��������¼
    file = fopen("borrow_records.txt", "w");
    if (file == NULL) {
        printf("�޷��򿪽����¼�ļ��Ա������ݣ�\n");
        return;
    }
    for (int i = 0; i < num_borrow_records; i++) {
        fprintf(file, "%d,%d,%d,%s\n", borrow_records[i].book_id, borrow_records[i].user_id, borrow_records[i].book_id, borrow_records[i].borrow_date);
    }
    fclose(file);
}

// �û���¼
void login_user() {
    char username[50] = { 0 }, password[50] = { 0 };
    printf("�������û���: ");
    scanf("%s", username);
    printf("����������: ");
    scanf("%s", password);

    User* user = NULL;
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            user = &users[i];
            break;
        }
    }

    if (user) {
        user_menu(user);
    }
    else {
        printf("�û������������\n");
    }
}

// ����Ա��¼
void login_admin() {
    char username[50] = { 0 }, password[50] = { 0 };
    printf("���������Ա�û���: ");
    scanf("%s", username);
    printf("���������Ա����: ");
    scanf("%s", password);

    Admin* admin = NULL;
    for (int i = 0; i < num_admins; i++) {
        if (strcmp(admins[i].username, username) == 0 && strcmp(admins[i].password, password) == 0) {
            admin = &admins[i];
            break;
        }
    }

    if (admin) {
        admin_menu();
    }
    else {
        printf("����Ա�û������������\n");
    }
}

// �û��˵�
void user_menu(User* user) {
    int choice;
    while (1) {
        printf("\n�û��˵�:\n");
        printf("1. ����ͼ��\n");
        printf("2. ��ʾ����ͼ��\n");
        printf("3. ����\n");
        printf("4. ����\n");
        printf("0. ע����¼\n");
        printf("��ѡ�����: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("ע����¼���������˵���\n");
            break;  // �������˵�
        }

        switch (choice) {
        case 1: search_book(); break;
        case 2: display_books(); break;
        case 3: borrow_book(user); break;
        case 4: return_book(user); break;
        default: printf("��Ч��ѡ��\n"); break;
        }
    }
}

// ����Ա�˵�
void admin_menu() {
    int choice;
    while (1) {
        printf("\n����Ա�˵�:\n");
        printf("1. ���ͼ��\n");
        printf("2. ɾ��ͼ��\n");
        printf("3. �޸�ͼ����Ϣ\n");
        printf("4. �鿴�����¼\n");
        printf("5. ����ͼ��\n");   // ��Ӳ���ͼ��ѡ��
        printf("6. ��ʾ����ͼ��\n"); // �����ʾ����ͼ��ѡ��
        printf("0. ע����¼\n");
        printf("��ѡ�����: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("ע����¼���������˵���\n");
            break;  // �������˵�
        }

        switch (choice) {
        case 1: add_book(); break;
        case 2: remove_book(); break;
        case 3: modify_book(); break;
        case 4: view_borrow_records(); break;
        case 5: search_book(); break;  // ���ò���ͼ�麯��
        case 6: display_books(); break; // ������ʾ����ͼ�麯��
        default: printf("��Ч��ѡ��\n"); break;
        }
    }
}


// ע���û�
void register_user() {
    char username[50] = { 0 }, password[50] = { 0 };
    printf("�������û���: ");
    scanf("%s", username);

    // ����û����Ƿ��Ѵ���
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("�û����Ѵ��ڣ�\n");
            return;
        }
    }

    printf("����������: ");
    scanf("%s", password);

    // ���û���Ϣ
    users[num_users].id = num_users + 1; // �û�ID��1��ʼ
    strcpy(users[num_users].username, username);
    strcpy(users[num_users].password, password);

    num_users++;

    printf("ע��ɹ���\n");
}

// ����ͼ��
// ����ͼ��
void search_book() {
    char title[100] = { 0 };
    printf("������ͼ����: ");
    scanf("%s", title);

    int found = 0;

    // ��ӡ���ͷ��
    printf("\n���ҽ����\n");
    printf("+---------+------------------------+------------------------+---------+\n");
    printf("| ͼ��ID  | ����                   | ����                   | ״̬    |\n");
    printf("+---------+------------------------+------------------------+---------+\n");

    for (int i = 0; i < num_books; i++) {
        if (strstr(books[i].title, title)) {
            printf("| %-7d | %-22s | %-22s | %-7s |\n",
                books[i].id,
                books[i].title,
                books[i].author,
                books[i].available ? "�ɽ�" : "�ѽ�");
            found = 1;
        }
    }

    if (!found) {
        printf("δ�ҵ�ƥ���ͼ�顣\n");
    }
    else {
        printf("+---------+------------------------+------------------------+---------+\n");
    }
}


// ��ʾ����ͼ��
void display_books() {
    printf("\nͼ����Ϣ��\n");
    printf("+---------+------------------------+------------------------+---------+\n");
    printf("| ͼ��ID  | ����                   | ����                   | ״̬    |\n");
    printf("+---------+------------------------+------------------------+---------+\n");

    for (int i = 0; i < num_books; i++) {
        printf("| %-7d | %-22s | %-22s | %-7s |\n",
            books[i].id,
            books[i].title,
            books[i].author,
            books[i].available ? "�ɽ�" : "�ѽ�");
    }

    printf("+---------+------------------------+------------------------+---------+\n");
}


// ����
void borrow_book(User* user) {
    int book_id;
    printf("������Ҫ���ĵ�ͼ��ID: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > num_books) {
        printf("��Ч��ͼ��ID��\n");
        return;
    }

    if (books[book_id - 1].available == 0) {
        printf("��ͼ���ѱ������\n");
        return;
    }

    // ��ȡ��ǰ����
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // ��ʽ������Ϊ YYYY-MM-DD
    char borrow_date[20];
    snprintf(borrow_date, sizeof(borrow_date), "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    books[book_id - 1].available = 0; // ����ͼ��Ϊ�ѽ�
    borrow_records[num_borrow_records].book_id = book_id;
    borrow_records[num_borrow_records].user_id = user->id;
    strcpy(borrow_records[num_borrow_records].borrow_date, borrow_date);
    num_borrow_records++;

    printf("����ɹ�����������: %s\n", borrow_date);
}

// ����
void return_book(User* user) {
    int book_id;
    printf("������Ҫ����ͼ��ID: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > num_books) {
        printf("��Ч��ͼ��ID��\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < num_borrow_records; i++) {
        if (borrow_records[i].book_id == book_id && borrow_records[i].user_id == user->id) {
            books[book_id - 1].available = 1; // ����ͼ��Ϊ�ɽ�
            // ɾ�������¼
            for (int j = i; j < num_borrow_records - 1; j++) {
                borrow_records[j] = borrow_records[j + 1];
            }
            num_borrow_records--;
            printf("����ɹ���\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("��δ�������ѹ黹��\n");
    }
}

// ���ͼ��
void add_book() {
    Book book;
    printf("������ͼ��ID: ");
    scanf("%d", &book.id);

    // ���ͼ��ID�Ƿ��Ѿ�����
    for (int i = 0; i < num_books; i++) {
        if (books[i].id == book.id) {
            printf("��ͼ��ID�Ѵ��ڣ�\n");
            return;
        }
    }

    printf("������ͼ������: ");
    scanf(" %[^\n]", book.title);
    printf("������ͼ������: ");
    scanf(" %[^\n]", book.author);
    book.available = 1; // Ĭ�Ͽɽ�

    books[num_books] = book;
    num_books++;

    printf("ͼ����ӳɹ���\n");
}

// ɾ��ͼ��
void remove_book() {
    int book_id;
    printf("������Ҫɾ����ͼ��ID: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > num_books) {
        printf("��Ч��ͼ��ID��\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < num_books; i++) {
        if (books[i].id == book_id) {
            found = 1;
            // ɾ��ͼ��
            for (int j = i; j < num_books - 1; j++) {
                books[j] = books[j + 1];
            }
            num_books--;
            printf("ͼ��ɾ���ɹ���\n");
            break;
        }
    }

    if (!found) {
        printf("δ�ҵ�ͼ��ID��\n");
    }
}

// �޸�ͼ����Ϣ
void modify_book() {
    int book_id;
    printf("������Ҫ�޸ĵ�ͼ��ID: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > num_books) {
        printf("��Ч��ͼ��ID��\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < num_books; i++) {
        if (books[i].id == book_id) {
            found = 1;
            printf("�������µ�����: ");
            scanf(" %[^\n]", books[i].title);
            printf("�������µ�����: ");
            scanf(" %[^\n]", books[i].author);
            printf("ͼ����Ϣ�޸ĳɹ���\n");
            break;
        }
    }

    if (!found) {
        printf("δ�ҵ�ͼ��ID��\n");
    }
}


// �鿴�����¼
void view_borrow_records() {
    printf("\n�����¼��\n");
    printf("+---------+------------+--------------+------------+--------------------------+--------------+------------+\n");
    printf("| ����ID  | �û�ID     | �û���       | ͼ��ID     | ����                     | ����         | ��������   |\n");
    printf("+---------+------------+--------------+------------+--------------------------+--------------+------------+\n");

    // ���������¼����ӡ���
    for (int i = 0; i < num_borrow_records; i++) {
        int user_id = borrow_records[i].user_id;
        int book_id = borrow_records[i].book_id;

        // �����û���Ϣ
        char username[50] = { 0 };
        strcpy(username, users[user_id - 1].username);  // �û�ID��1��ʼ������� user_id - 1

        // ����ͼ����Ϣ
        char book_title[100] = { 0 }, book_author[100] = { 0 };
        strcpy(book_title, books[book_id - 1].title);    // ͼ��ID��1��ʼ������� book_id - 1
        strcpy(book_author, books[book_id - 1].author);

        // ��ӡ�����
        printf("| %-7d | %-10d | %-12s | %-10d | %-24s | %-12s | %-10s |\n",
            i + 1,  // ����ID
            user_id,  // �û�ID
            username,  // �û���
            book_id,  // ͼ��ID
            book_title,  // ����
            book_author,  // ����
            borrow_records[i].borrow_date);  // ��������
    }

    printf("+---------+------------+--------------+------------+--------------------------+--------------+------------+\n");
}


// �˳�ϵͳ
void exit_program() {
    printf("��лʹ��ͼ�����ϵͳ��\n");
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 100
#define MAX_USERS 100
#define MAX_ADMINS 10
#define MAX_RECORDS 100

// 图书信息结构体
typedef struct {
    int id;
    char title[100];
    char author[100];
    int available; // 1: 可借 0: 已借
} Book;

// 用户信息结构体
typedef struct {
    int id;
    char username[50];
    char password[50];
} User;

// 管理员信息结构体
typedef struct {
    int id;
    char username[50];
    char password[50];
} Admin;

// 借书记录结构体
typedef struct {
    int book_id;
    int user_id;
    char borrow_date[20];
} BorrowRecord;

Book books[MAX_BOOKS];
User users[MAX_USERS];
Admin admins[MAX_ADMINS];
BorrowRecord borrow_records[MAX_RECORDS];

// 全局变量
int num_books = 0, num_users = 0, num_admins = 0, num_borrow_records = 0;

// 函数声明
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
void register_user();  // 注册用户
void exit_program();   // 退出程序

// 主函数
int main() {
    load_data();

    int choice;
    while (1) {
        printf("1. 用户登录\n");
        printf("2. 管理员登录\n");
        printf("3. 用户注册\n");
        printf("0. 退出系统\n");
        printf("请选择操作: ");
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
            break;  // 退出系统
        }
        else {
            printf("无效的选择！\n");
        }
    }

    save_data();
    return 0;
}

// 加载数据
void load_data() {
    FILE* file;

    // 读取图书信息
    file = fopen("books.txt", "r");
    if (file == NULL) {
        printf("无法打开图书文件！\n");
        // 如果无法加载图书信息，依然允许继续加载其他数据
    }
    else {
        while (fscanf(file, "%d,%99[^,],%99[^,],%d\n", &books[num_books].id, books[num_books].title, books[num_books].author, &books[num_books].available) != EOF) {
            num_books++;
        }
        fclose(file);
    }

    // 读取用户信息
    file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("无法打开用户文件！\n");
        return;
    }
    while (fscanf(file, "%d,%49[^,],%49[^\n]\n", &users[num_users].id, users[num_users].username, users[num_users].password) != EOF) {
        num_users++;
    }
    fclose(file);

    // 读取管理员信息
    file = fopen("admins.txt", "r");
    if (file == NULL) {
        printf("无法打开管理员文件！\n");
        return;
    }
    while (fscanf(file, "%d,%49[^,],%49[^\n]\n", &admins[num_admins].id, admins[num_admins].username, admins[num_admins].password) != EOF) {
        num_admins++;
    }
    fclose(file);

    // 读取借书记录
    file = fopen("borrow_records.txt", "r");
    if (file == NULL) {
        printf("无法打开借书记录文件！\n");
        return;
    }
    while (fscanf(file, "%d,%d,%d,%19[^\n]\n", &borrow_records[num_borrow_records].book_id, &borrow_records[num_borrow_records].user_id, &borrow_records[num_borrow_records].user_id, borrow_records[num_borrow_records].borrow_date) != EOF) {
        num_borrow_records++;
    }
    fclose(file);
}


// 保存数据
void save_data() {
    FILE* file;

    // 保存图书信息
    file = fopen("books.txt", "w");
    if (file == NULL) {
        printf("无法打开图书文件以保存数据！\n");
        return;
    }
    for (int i = 0; i < num_books; i++) {
        fprintf(file, "%d,%s,%s,%d\n", books[i].id, books[i].title, books[i].author, books[i].available);
    }
    fclose(file);

    // 保存用户信息
    file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("无法打开用户文件以保存数据！\n");
        return;
    }
    for (int i = 0; i < num_users; i++) {
        fprintf(file, "%d,%s,%s\n", users[i].id, users[i].username, users[i].password);
    }
    fclose(file);

    // 保存管理员信息
    file = fopen("admins.txt", "w");
    if (file == NULL) {
        printf("无法打开管理员文件以保存数据！\n");
        return;
    }
    for (int i = 0; i < num_admins; i++) {
        fprintf(file, "%d,%s,%s\n", admins[i].id, admins[i].username, admins[i].password);
    }
    fclose(file);

    // 保存借书记录
    file = fopen("borrow_records.txt", "w");
    if (file == NULL) {
        printf("无法打开借书记录文件以保存数据！\n");
        return;
    }
    for (int i = 0; i < num_borrow_records; i++) {
        fprintf(file, "%d,%d,%d,%s\n", borrow_records[i].book_id, borrow_records[i].user_id, borrow_records[i].book_id, borrow_records[i].borrow_date);
    }
    fclose(file);
}

// 用户登录
void login_user() {
    char username[50] = { 0 }, password[50] = { 0 };
    printf("请输入用户名: ");
    scanf("%s", username);
    printf("请输入密码: ");
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
        printf("用户名或密码错误！\n");
    }
}

// 管理员登录
void login_admin() {
    char username[50] = { 0 }, password[50] = { 0 };
    printf("请输入管理员用户名: ");
    scanf("%s", username);
    printf("请输入管理员密码: ");
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
        printf("管理员用户名或密码错误！\n");
    }
}

// 用户菜单
void user_menu(User* user) {
    int choice;
    while (1) {
        printf("\n用户菜单:\n");
        printf("1. 查找图书\n");
        printf("2. 显示所有图书\n");
        printf("3. 借书\n");
        printf("4. 还书\n");
        printf("0. 注销登录\n");
        printf("请选择操作: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("注销登录，返回主菜单。\n");
            break;  // 返回主菜单
        }

        switch (choice) {
        case 1: search_book(); break;
        case 2: display_books(); break;
        case 3: borrow_book(user); break;
        case 4: return_book(user); break;
        default: printf("无效的选择！\n"); break;
        }
    }
}

// 管理员菜单
void admin_menu() {
    int choice;
    while (1) {
        printf("\n管理员菜单:\n");
        printf("1. 添加图书\n");
        printf("2. 删除图书\n");
        printf("3. 修改图书信息\n");
        printf("4. 查看借书记录\n");
        printf("5. 查找图书\n");   // 添加查找图书选项
        printf("6. 显示所有图书\n"); // 添加显示所有图书选项
        printf("0. 注销登录\n");
        printf("请选择操作: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("注销登录，返回主菜单。\n");
            break;  // 返回主菜单
        }

        switch (choice) {
        case 1: add_book(); break;
        case 2: remove_book(); break;
        case 3: modify_book(); break;
        case 4: view_borrow_records(); break;
        case 5: search_book(); break;  // 调用查找图书函数
        case 6: display_books(); break; // 调用显示所有图书函数
        default: printf("无效的选择！\n"); break;
        }
    }
}


// 注册用户
void register_user() {
    char username[50] = { 0 }, password[50] = { 0 };
    printf("请输入用户名: ");
    scanf("%s", username);

    // 检查用户名是否已存在
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("用户名已存在！\n");
            return;
        }
    }

    printf("请输入密码: ");
    scanf("%s", password);

    // 新用户信息
    users[num_users].id = num_users + 1; // 用户ID从1开始
    strcpy(users[num_users].username, username);
    strcpy(users[num_users].password, password);

    num_users++;

    printf("注册成功！\n");
}

// 查找图书
// 查找图书
void search_book() {
    char title[100] = { 0 };
    printf("请输入图书名: ");
    scanf("%s", title);

    int found = 0;

    // 打印表格头部
    printf("\n查找结果：\n");
    printf("+---------+------------------------+------------------------+---------+\n");
    printf("| 图书ID  | 书名                   | 作者                   | 状态    |\n");
    printf("+---------+------------------------+------------------------+---------+\n");

    for (int i = 0; i < num_books; i++) {
        if (strstr(books[i].title, title)) {
            printf("| %-7d | %-22s | %-22s | %-7s |\n",
                books[i].id,
                books[i].title,
                books[i].author,
                books[i].available ? "可借" : "已借");
            found = 1;
        }
    }

    if (!found) {
        printf("未找到匹配的图书。\n");
    }
    else {
        printf("+---------+------------------------+------------------------+---------+\n");
    }
}


// 显示所有图书
void display_books() {
    printf("\n图书信息：\n");
    printf("+---------+------------------------+------------------------+---------+\n");
    printf("| 图书ID  | 书名                   | 作者                   | 状态    |\n");
    printf("+---------+------------------------+------------------------+---------+\n");

    for (int i = 0; i < num_books; i++) {
        printf("| %-7d | %-22s | %-22s | %-7s |\n",
            books[i].id,
            books[i].title,
            books[i].author,
            books[i].available ? "可借" : "已借");
    }

    printf("+---------+------------------------+------------------------+---------+\n");
}


// 借书
void borrow_book(User* user) {
    int book_id;
    printf("请输入要借阅的图书ID: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > num_books) {
        printf("无效的图书ID。\n");
        return;
    }

    if (books[book_id - 1].available == 0) {
        printf("该图书已被借出。\n");
        return;
    }

    // 获取当前日期
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // 格式化日期为 YYYY-MM-DD
    char borrow_date[20];
    snprintf(borrow_date, sizeof(borrow_date), "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    books[book_id - 1].available = 0; // 设置图书为已借
    borrow_records[num_borrow_records].book_id = book_id;
    borrow_records[num_borrow_records].user_id = user->id;
    strcpy(borrow_records[num_borrow_records].borrow_date, borrow_date);
    num_borrow_records++;

    printf("借书成功！借书日期: %s\n", borrow_date);
}

// 还书
void return_book(User* user) {
    int book_id;
    printf("请输入要还的图书ID: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > num_books) {
        printf("无效的图书ID。\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < num_borrow_records; i++) {
        if (borrow_records[i].book_id == book_id && borrow_records[i].user_id == user->id) {
            books[book_id - 1].available = 1; // 设置图书为可借
            // 删除借书记录
            for (int j = i; j < num_borrow_records - 1; j++) {
                borrow_records[j] = borrow_records[j + 1];
            }
            num_borrow_records--;
            printf("还书成功！\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("您未借此书或已归还。\n");
    }
}

// 添加图书
void add_book() {
    Book book;
    printf("请输入图书ID: ");
    scanf("%d", &book.id);

    // 检查图书ID是否已经存在
    for (int i = 0; i < num_books; i++) {
        if (books[i].id == book.id) {
            printf("该图书ID已存在！\n");
            return;
        }
    }

    printf("请输入图书书名: ");
    scanf(" %[^\n]", book.title);
    printf("请输入图书作者: ");
    scanf(" %[^\n]", book.author);
    book.available = 1; // 默认可借

    books[num_books] = book;
    num_books++;

    printf("图书添加成功！\n");
}

// 删除图书
void remove_book() {
    int book_id;
    printf("请输入要删除的图书ID: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > num_books) {
        printf("无效的图书ID。\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < num_books; i++) {
        if (books[i].id == book_id) {
            found = 1;
            // 删除图书
            for (int j = i; j < num_books - 1; j++) {
                books[j] = books[j + 1];
            }
            num_books--;
            printf("图书删除成功！\n");
            break;
        }
    }

    if (!found) {
        printf("未找到图书ID！\n");
    }
}

// 修改图书信息
void modify_book() {
    int book_id;
    printf("请输入要修改的图书ID: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > num_books) {
        printf("无效的图书ID。\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < num_books; i++) {
        if (books[i].id == book_id) {
            found = 1;
            printf("请输入新的书名: ");
            scanf(" %[^\n]", books[i].title);
            printf("请输入新的作者: ");
            scanf(" %[^\n]", books[i].author);
            printf("图书信息修改成功！\n");
            break;
        }
    }

    if (!found) {
        printf("未找到图书ID！\n");
    }
}


// 查看借书记录
void view_borrow_records() {
    printf("\n借书记录：\n");
    printf("+---------+------------+--------------+------------+--------------------------+--------------+------------+\n");
    printf("| 借书ID  | 用户ID     | 用户名       | 图书ID     | 书名                     | 作者         | 借书日期   |\n");
    printf("+---------+------------+--------------+------------+--------------------------+--------------+------------+\n");

    // 遍历借书记录并打印表格
    for (int i = 0; i < num_borrow_records; i++) {
        int user_id = borrow_records[i].user_id;
        int book_id = borrow_records[i].book_id;

        // 查找用户信息
        char username[50] = { 0 };
        strcpy(username, users[user_id - 1].username);  // 用户ID从1开始，因此是 user_id - 1

        // 查找图书信息
        char book_title[100] = { 0 }, book_author[100] = { 0 };
        strcpy(book_title, books[book_id - 1].title);    // 图书ID从1开始，因此是 book_id - 1
        strcpy(book_author, books[book_id - 1].author);

        // 打印表格行
        printf("| %-7d | %-10d | %-12s | %-10d | %-24s | %-12s | %-10s |\n",
            i + 1,  // 借书ID
            user_id,  // 用户ID
            username,  // 用户名
            book_id,  // 图书ID
            book_title,  // 书名
            book_author,  // 作者
            borrow_records[i].borrow_date);  // 借书日期
    }

    printf("+---------+------------+--------------+------------+--------------------------+--------------+------------+\n");
}


// 退出系统
void exit_program() {
    printf("感谢使用图书管理系统！\n");
}
